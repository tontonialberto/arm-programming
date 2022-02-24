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

int main() {
	SSD1306_Data oledData;
	
	HardwareInit();
	
	Timer_GetElapsedMs = TimerImpl_GetElapsedMs;
	Timer_GetElapsedMsSince = TimerImpl_GetElapsedMsSince;
	
	oledData.buffer = oledBuffer;
	oledData.I2C_Write_BufferSize = 64;
	oledData.I2C_Write = I2C_Write_Adapter;
	oledData.DelayUs = delayUs;
	
	oledInit = SSD1306_Init(&oledData);
	
	GameContext ctx;
	ctx.playerBulletVerticalStep = PLAYER_BULLET_UP_STEP;
	ctx.playerHorizontalStep = 0;
	ctx.enemyHorizontalStep = 0;
	ctx.enemyHorizontalDirection = 1;
	ctx.enemyVerticalStep = 0;
	ctx.enemiesRect.x = 30;
	ctx.enemiesRect.y = 5;
	ctx.enemiesRect.width = ENEMY_WIDTH * ENEMY_COLUMNS; // TODO: consider spacing
	ctx.enemiesRect.height = ENEMY_HEIGHT;
	ctx.hasEnemyHitBoundary = false;
	ctx.gameArea.x = SCREEN_MIN_X;
	ctx.gameArea.width = (uint16_t)(SCREEN_MAX_X - SCREEN_MIN_X);
	ctx.gameArea.y = SCREEN_MIN_Y;
	ctx.gameArea.height = (uint16_t)(32 - SCREEN_MIN_Y); // TODO: extract constant
	ctx.gameAreaMinX = SCREEN_MIN_X;
	ctx.gameAreaMaxX = SCREEN_MAX_X;
	ctx.gameAreaMinY = SCREEN_MIN_Y;
	ctx.spawnPlayerBullet = false;
	
	GameObject player;
	player.x = 56;
	player.y = 24;
	player.width = PLAYER_WIDTH;
	player.height = PLAYER_HEIGHT;
	player.ctx = &ctx;
	
	GameObject bullet;
	bullet.width = PLAYER_BULLET_WIDTH;
	bullet.height = PLAYER_BULLET_HEIGHT;
	bullet.active = false;
	bullet.ctx = &ctx;
	
	Enemy enemy;
	enemy.index = 0;
	enemy.go.x = 30;
	enemy.go.y = 5;
	enemy.go.width = ENEMY_WIDTH;
	enemy.go.height = ENEMY_HEIGHT;
	enemy.go.ctx = &ctx;
	
	Enemy otherEnemy;
	otherEnemy.index = 1;
	otherEnemy.go.x = 30 + (int16_t)ENEMY_WIDTH;
	otherEnemy.go.y = 5;
	otherEnemy.go.width = ENEMY_WIDTH;
	otherEnemy.go.height = ENEMY_HEIGHT;
	otherEnemy.go.ctx = &ctx;
	
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
		player.x += ctx.playerHorizontalStep;
		RestoreInsideBoundsHoriz(&player);
		
		// Player bullet move
		if(bullet.active) {
			bullet.y += ctx.playerBulletVerticalStep;
		}
		if(bullet.y < ctx.gameAreaMinY) {
			bullet.active = false;
		}
		
		// Enemies rect move
		ctx.enemiesRect.x += ctx.enemyHorizontalStep;
		ctx.enemiesRect.y += ctx.enemyVerticalStep;
		if(RestoreInsideBoundsHoriz2(&ctx.enemiesRect, &ctx.gameArea)) {
			ctx.hasEnemyHitBoundary = true;
		}
		
		// Enemies move
		Enemy_Move(&enemy);
		Enemy_Move(&otherEnemy);
		
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
				(uint8_t)bullet.x, 
				(uint8_t)bullet.y, 
				(uint8_t)bullet.width, 
				(uint8_t)bullet.height);
		}
		
		// Player render
		SSD1306_WriteRectangle(
			&oledData, 
			(uint8_t)player.x, 
			(uint8_t)player.y, 
			(uint8_t)player.width, 
			(uint8_t)player.height);
		
		// Enemy render
		SSD1306_WriteRectangle(
			&oledData,
			(uint8_t)enemy.go.x,
			(uint8_t)enemy.go.y,
			(uint8_t)enemy.go.width,
			(uint8_t)enemy.go.height);
		
		SSD1306_WriteRectangle(
			&oledData,
			(uint8_t)otherEnemy.go.x,
			(uint8_t)otherEnemy.go.y,
			(uint8_t)otherEnemy.go.width,
			(uint8_t)otherEnemy.go.height);
		
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
