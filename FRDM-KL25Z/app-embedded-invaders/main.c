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
	
	oledData.buffer = oledBuffer;
	oledData.I2C_Write_BufferSize = 64;
	oledData.I2C_Write = I2C_Write_Adapter;
	oledData.DelayUs = delayUs;
	
	oledInit = SSD1306_Init(&oledData);
	
	GameContext ctx;
	ctx.playerBulletVerticalStep = PLAYER_BULLET_UP_STEP;
	ctx.playerHorizontalStep = 0;
	ctx.enemyHorizontalStep = 0;
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
	
	GameObject enemy;
	enemy.x = 30;
	enemy.y = 5;
	enemy.width = ENEMY_WIDTH;
	enemy.height = ENEMY_HEIGHT;
	enemy.ctx = &ctx;
	
	PeriodicEvent evtEnemyMove;
	evtEnemyMove.timeoutMs = 1000;
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
			ctx.enemyHorizontalStep = 10;
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
		if(player.x < ctx.gameAreaMinX) {
			player.x = ctx.gameAreaMinX;
		}
		else if(player.x + player.width > ctx.gameAreaMaxX) {
			player.x = ctx.gameAreaMaxX - (int16_t)player.width - (int16_t)1;
		}
		
		// Player bullet move
		if(bullet.active) {
			bullet.y += ctx.playerBulletVerticalStep;
		}
		if(bullet.y < ctx.gameAreaMinY) {
			bullet.active = false;
		}
		
		// Enemy move
		enemy.x += ctx.enemyHorizontalStep;
		
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
			(uint8_t)enemy.x,
			(uint8_t)enemy.y,
			(uint8_t)enemy.width,
			(uint8_t)enemy.height);
		
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
