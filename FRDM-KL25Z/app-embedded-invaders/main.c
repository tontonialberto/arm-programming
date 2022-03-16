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
	// TODO: consider vertical spacing between enemies
	Rect2D_Init(
		&enemiesRect,
		30, 
		5, 
		(ENEMY_WIDTH * ENEMY_COLUMNS) + (uint16_t)(ENEMY_HORIZ_SPACING * (ENEMY_COLUMNS - 1)), 
		ENEMY_HEIGHT);
	
	Rect2D gameArea;
	Rect2D_Init(
		&gameArea, 
		SCREEN_MIN_X, 
		SCREEN_MIN_Y,
		(uint16_t)(SCREEN_MAX_X - SCREEN_MIN_X),
		(uint16_t)(32 - SCREEN_MIN_Y)); // TODO: extract constant
	
	GameContext ctx;
	GameContext_Init(&ctx, 1U, PLAYER_BULLET_UP_STEP, &enemiesRect, &gameArea);
	
	GameObject player;
	Rect2D_Init(&player.rect, 56, 24, PLAYER_WIDTH, PLAYER_HEIGHT);
	player.ctx = &ctx;
	
	GameObject bullet;
	bullet.rect.width = PLAYER_BULLET_WIDTH;
	bullet.rect.height = PLAYER_BULLET_HEIGHT;
	bullet.active = false;
	bullet.ctx = &ctx;
	
	Enemy enemy;
	enemy.index = 0;
	Rect2D_Init(&enemy.go.rect, ctx.enemiesRect.x, ctx.enemiesRect.y, ENEMY_WIDTH, ENEMY_HEIGHT);
	enemy.go.ctx = &ctx;
	
	Enemy otherEnemy;
	otherEnemy.index = 1;
	Rect2D_Init(
		&otherEnemy.go.rect, 
		ctx.enemiesRect.x + (int16_t)ENEMY_WIDTH + ENEMY_HORIZ_SPACING, 
		ctx.enemiesRect.y, 
		ENEMY_WIDTH, 
		ENEMY_HEIGHT);
	otherEnemy.go.ctx = &ctx;
	
	Enemy yetAnotherEnemy;
	yetAnotherEnemy.index = 2;
	Rect2D_Init(
		&yetAnotherEnemy.go.rect, 
		ctx.enemiesRect.x,
		ctx.enemiesRect.y + (int16_t)ENEMY_HEIGHT + ENEMY_VERTICAL_SPACING,
		ENEMY_WIDTH,
		ENEMY_HEIGHT);
	yetAnotherEnemy.go.ctx = &ctx;
	
	PeriodicEvent evtEnemyMove;
	evtEnemyMove.timeoutMs = EVT_ENEMY_MOVE_PERIOD_MS;
	evtEnemyMove.lastTimeoutMs = 0;
	
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
			bullet.rect.y += ctx.playerBulletVerticalStep;
		}
		if(bullet.rect.y < ctx.gameArea.y) {
			bullet.active = false;
		}
		
		// Enemies rect move
		ctx.enemiesRect.x += ctx.enemyHorizontalStep;
		ctx.enemiesRect.y += ctx.enemyVerticalStep;
		if(RestoreInsideBoundsHoriz(&ctx.enemiesRect, &ctx.gameArea)) {
			ctx.hasEnemyHitBoundary = true;
		}
		
		// Enemies move
		Enemy_Move(&enemy);
		Enemy_Move(&otherEnemy);
		Enemy_Move(&yetAnotherEnemy);
		
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
		SSD1306_WriteRectangle(
			&oledData,
			(uint8_t)enemy.go.rect.x,
			(uint8_t)enemy.go.rect.y,
			(uint8_t)enemy.go.rect.width,
			(uint8_t)enemy.go.rect.height);
		
		SSD1306_WriteRectangle(
			&oledData,
			(uint8_t)otherEnemy.go.rect.x,
			(uint8_t)otherEnemy.go.rect.y,
			(uint8_t)otherEnemy.go.rect.width,
			(uint8_t)otherEnemy.go.rect.height);
			
		SSD1306_WriteRectangle(
			&oledData,
			(uint8_t)yetAnotherEnemy.go.rect.x,
			(uint8_t)yetAnotherEnemy.go.rect.y,
			(uint8_t)yetAnotherEnemy.go.rect.width,
			(uint8_t)yetAnotherEnemy.go.rect.height);
		
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
