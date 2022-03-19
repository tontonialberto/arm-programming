#include <MKL25Z4.h>
#include "SSD1306/SSD1306.h"
#include "I2C_Master.h"
#include "delay.h"
#include "hw_init.h"
#include "util.h"
#include "GameObject.h"
#include "GameUtils.h"
#include "ISR.h"
#include "AppConfig.h"
#include "Timer.h"
#include "TimerImpl.h"
#include "GameConfig.h"
#include "MoveFunctions.h"

I2C_Result I2C_Write_Adapter(
	uint8_t address, 
	uint8_t *data, 
	uint32_t len, 
	bool restart);

static bool oledInit = false;
static uint8_t oledBuffer[SSD1306_BUFFER_SIZE];
static SSD1306_Data oledData;

int main() {
	HardwareInit();
	
	Timer_GetElapsedMs = TimerImpl_GetElapsedMs;
	Timer_GetElapsedMsSince = TimerImpl_GetElapsedMsSince;
	
	oledData.buffer = oledBuffer;
	oledData.I2C_Write_BufferSize = 64;
	oledData.I2C_Write = I2C_Write_Adapter;
	oledData.DelayUs = delayUs;
	
	oledInit = SSD1306_Init(&oledData);
	
	Rect2D enemiesRect;
	Rect2D_Init(
		&enemiesRect,
		ENEMIES_RECT_INITIAL_X, 
		ENEMIES_RECT_INITIAL_Y,
		ENEMIES_RECT_WIDTH,
		ENEMY_HEIGHT); // TODO: consider vertical spacing between enemies
	
	Rect2D gameArea;
	Rect2D_Init(
		&gameArea, 
		SCREEN_MIN_X, 
		SCREEN_MIN_Y,
		SCREEN_WIDTH,
		SCREEN_HEIGHT);
	
	Enemy enemies[N_ENEMIES];
	
	GameContext ctx;
	GameContext_Init(
		&ctx, 
		1U, 
		PLAYER_BULLET_UP_STEP, 
		&enemiesRect, 
		&gameArea,
		enemies,
		N_ENEMIES);
	
	// Enemies init
	for(uint16_t i=0; i<ctx.nEnemies; i++) {
		Enemy *enemy = &enemies[i];
		Rect2D *enemyRect = &enemy->go.rect;
		
		enemy->index = (int16_t)i;
		Rect2D_Init(
			enemyRect,
			0, 0,
			ENEMY_WIDTH,
			ENEMY_HEIGHT);
		Enemy_Move(enemy);
		enemy->go.ctx = &ctx;
		enemy->go.active = true;
	}
	
	GameObject player;
	Rect2D_Init(
		&player.rect, 
		PLAYER_INITIAL_X, 
		PLAYER_INITIAL_Y, 
		PLAYER_WIDTH, 
		PLAYER_HEIGHT);
	player.ctx = &ctx;
	
	GameObject bullet;
	bullet.rect.width = PLAYER_BULLET_WIDTH;
	bullet.rect.height = PLAYER_BULLET_HEIGHT;
	bullet.active = false;
	bullet.ctx = &ctx;
	
	PeriodicEvent evtEnemyMove;
	evtEnemyMove.timeoutMs = EVT_ENEMY_MOVE_PERIOD_MS;
	evtEnemyMove.lastTimeoutMs = 0;
	
	PeriodicEvent evtSpawnEnemyBullet;
	evtSpawnEnemyBullet.timeoutMs = EVT_SPAWN_ENEMY_BULLET_PERIOD_MS;
	evtSpawnEnemyBullet.lastTimeoutMs = 0;
	
	delayMs(100);
	
	while(1) {
		delayMs(20);
		SSD1306_Clear(&oledData);
		
		// Handle user input
		if(spawnPlayerBullet) {
			spawnPlayerBullet = false;
			ctx.spawnPlayerBullet = true;
		}
		
		if(analogY > THRESHOLD_JOYSTICK) {
			ctx.playerHorizontalStep = PLAYER_RIGHT_STEP;
		}
		else if(analogY < -THRESHOLD_JOYSTICK) {
			ctx.playerHorizontalStep = -PLAYER_RIGHT_STEP;
		}
		else {
			ctx.playerHorizontalStep = 0;
		}
		
		// Periodic event processing
		if(evtEnemyMove.timeoutMs <= Timer_GetElapsedMsSince(evtEnemyMove.lastTimeoutMs)) {
			// Execute the event: update enemy horiz step
			ctx.enemyHorizontalStep = ENEMY_RIGHT_STEP * ctx.enemyHorizontalDirection;
			evtEnemyMove.lastTimeoutMs = Timer_GetElapsedMs();
		}
		else {
			// Execute the operation that must happen when
			// the event is not triggered.
			ctx.enemyHorizontalStep = 0;
		}
		
		if(evtSpawnEnemyBullet.timeoutMs <= Timer_GetElapsedMsSince(evtSpawnEnemyBullet.lastTimeoutMs)) {
			ctx.spawnEnemyBullet = true;
			evtSpawnEnemyBullet.lastTimeoutMs = Timer_GetElapsedMs();
		}
		else {
			// Do nothing.
		}
		
		// Spawn bullet if needed
		if(ctx.spawnPlayerBullet) {
			ctx.spawnPlayerBullet = false;
			bullet.active = true;
			SetSpawnPosition_PlayerBullet(&bullet, &player);
		}
		
		// Player move
		player.rect.x += ctx.playerHorizontalStep;
		RestoreInsideBoundsHoriz(&player.rect, &ctx.gameArea);
		
		// Player bullet move
		if(bullet.active) {
			PlayerBullet_Move(&bullet);
		}
		
		// Enemies rect move
		ctx.enemiesRect.x += ctx.enemyHorizontalStep;
		ctx.enemiesRect.y += ctx.enemyVerticalStep;
		if(RestoreInsideBoundsHoriz(&ctx.enemiesRect, &ctx.gameArea)) {
			ctx.hasEnemyHitBoundary = true;
		}
		
		// Enemies move
		for(uint16_t i=0; i<N_ENEMIES; i++) {
			if(enemies[i].go.active) {
				Enemy_Move(&enemies[i]);
			}
		}
		
		// Invert enemies direction and move them down
		// if any of them has hit a boundary
		if(ctx.hasEnemyHitBoundary) {
			ctx.hasEnemyHitBoundary = false;
			ctx.enemyHorizontalDirection *= -1;
			ctx.enemyVerticalStep = ENEMY_DOWN_STEP;
		}
		else {
			ctx.enemyVerticalStep = 0;
		}
		
		// Player bullet render
		if(bullet.active) {
			SSD1306_WriteRectangle(
				&oledData, 
				(uint8_t)bullet.rect.x, 
				(uint8_t)bullet.rect.y, 
				(uint8_t)bullet.rect.width, 
				(uint8_t)bullet.rect.height);
		}
		
		// Player render
		SSD1306_WriteRectangle(
			&oledData, 
			(uint8_t)player.rect.x, 
			(uint8_t)player.rect.y, 
			(uint8_t)player.rect.width, 
			(uint8_t)player.rect.height);
		
		// Enemy render
		for(uint16_t i=0; i<N_ENEMIES; i++) {
			if(enemies[i].go.active) {
				SSD1306_WriteRectangle(
					&oledData,
					(uint8_t)enemies[i].go.rect.x,
					(uint8_t)enemies[i].go.rect.y,
					(uint8_t)enemies[i].go.rect.width,
					(uint8_t)enemies[i].go.rect.height);
			}
		}
		
		// Score render
		SSD1306_WriteUnsignedInt(
			&oledData,
			(int32_t)ctx.score,
			100, 0);
		
		SSD1306_Update(&oledData);
	}
}

I2C_Result I2C_Write_Adapter(
		uint8_t address, 
		uint8_t *data, 
		uint32_t len, 
		bool restart) {
	
	return I2C_Write(I2C0, address, data, len, restart);
}
