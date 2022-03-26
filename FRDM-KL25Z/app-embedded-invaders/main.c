#include <MKL25Z4.h>
#include <stdlib.h>
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
		1U, // TODO: extract constant
		PLAYER_BULLET_UP_STEP,
		ENEMY_BULLET_DOWN_STEP,
		&enemiesRect, 
		&gameArea,
		enemies,
		N_ENEMIES,
		PLAYER_INITIAL_HEALTH);
	
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
	
	GameObject playerBullet;
	playerBullet.rect.width = PLAYER_BULLET_WIDTH;
	playerBullet.rect.height = PLAYER_BULLET_HEIGHT;
	playerBullet.active = false;
	playerBullet.ctx = &ctx;
	
	GameObject enemyBullet;
	enemyBullet.rect.width = ENEMY_BULLET_WIDTH;
	enemyBullet.rect.height = ENEMY_BULLET_HEIGHT;
	enemyBullet.active = false;
	enemyBullet.ctx = &ctx;
	
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
		
		// Spawn player bullet if needed
		if(ctx.spawnPlayerBullet) {
			ctx.spawnPlayerBullet = false;
			playerBullet.active = true;
			SetSpawnPosition_PlayerBullet(&playerBullet, &player);
		}
		
		// Spawn enemy bullet if needed
		if(ctx.spawnEnemyBullet) {
			ctx.spawnEnemyBullet = false;
			enemyBullet.active = true;
			
			// Find the position of the shooter enemy:
			// The shooter is the one with maximum y pos,
			// and the closest x pos with respect to the player.
			int16_t spawnX = 0; 
			int16_t spawnY = 0;
			
			// Find, among active enemies,
			// the x pos of the closest one to the player.
			uint16_t xMinDistanceFromPlayer = 65535;
			for(uint16_t i=0; i<ctx.nEnemies; i++) {
				if(ctx.enemies[i].go.active) {
						
					int16_t xEnemy = ctx.enemies[i].go.rect.x;
						
					// Distance between player and the current enemy.
					uint16_t xDistance = (uint16_t)abs(xEnemy - player.rect.x);
					
					if(xDistance < xMinDistanceFromPlayer) {
						xMinDistanceFromPlayer = xDistance;
						spawnX = xEnemy + (ctx.enemies[i].go.rect.width / 2);
					}
				}
			}
			
			// Find, among active enemies, the one 
			// having maximum y pos.
			spawnY = -1; // Initialized to the minimum value.
			for(uint16_t i=0; i<ctx.nEnemies; i++) {
				
				uint16_t xDistance = (uint16_t)abs(ctx.enemies[i].go.rect.x - player.rect.x);
				
				if(ctx.enemies[i].go.active && (xDistance == xMinDistanceFromPlayer)) {
					int16_t yEnemy = ctx.enemies[i].go.rect.y;
					if(spawnY < yEnemy) {
						spawnY = yEnemy;
					}
				}
			}
			
			// Set spawn position near to the shooter
			enemyBullet.rect.x = spawnX;
			enemyBullet.rect.y = spawnY;
		}
		
		// Player move
		player.rect.x += ctx.playerHorizontalStep;
		RestoreInsideBoundsHoriz(&player.rect, &ctx.gameArea);
		
		// Player bullet move
		if(playerBullet.active) {
			PlayerBullet_Move(&playerBullet);
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
		
		// Enemy bullet move
		if(enemyBullet.active) {
			enemyBullet.rect.y += ctx.enemyBulletVerticalStep;
			
			// Boundary check
			if(enemyBullet.rect.y + enemyBullet.rect.height > gameArea.y + gameArea.height) {
				enemyBullet.active = false;
			}
			
			// Check collision with player
			if(Rect2D_Overlaps(enemyBullet.rect, player.rect)) {
				enemyBullet.active = false;
				ctx.playerHealth -= ENEMY_BULLET_DAMAGE;
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
		if(playerBullet.active) {
			SSD1306_WriteRectangle(
				&oledData, 
				(uint8_t)playerBullet.rect.x, 
				(uint8_t)playerBullet.rect.y, 
				(uint8_t)playerBullet.rect.width, 
				(uint8_t)playerBullet.rect.height);
		}
		
		// Player render
		SSD1306_WriteRectangle(
			&oledData, 
			(uint8_t)player.rect.x, 
			(uint8_t)player.rect.y, 
			(uint8_t)player.rect.width, 
			(uint8_t)player.rect.height
		);
		
		// Enemy render
		for(uint16_t i=0; i<N_ENEMIES; i++) {
			if(enemies[i].go.active) {
				SSD1306_WriteRectangle(
					&oledData,
					(uint8_t)enemies[i].go.rect.x,
					(uint8_t)enemies[i].go.rect.y,
					(uint8_t)enemies[i].go.rect.width,
					(uint8_t)enemies[i].go.rect.height
				);
			}
		}
		
		// Enemy bullet render
		if(enemyBullet.active) {
			SSD1306_WriteRectangle(
				&oledData,
				(uint8_t)enemyBullet.rect.x, 
				(uint8_t)enemyBullet.rect.y, 
				(uint8_t)enemyBullet.rect.width, 
				(uint8_t)enemyBullet.rect.height
			);
		}
		
		// Score render
		SSD1306_WriteUnsignedInt(
			&oledData,
			(int32_t)ctx.score,
			SCORE_DISPLAY_X,
			SCORE_DISPLAY_Y
		);
		
		// Player's health render
		SSD1306_WriteUnsignedInt(
			&oledData,
			(int32_t)ctx.playerHealth,
			PLAYER_HEALTH_DISPLAY_X,
			PLAYER_HEALTH_DISPLAY_Y
		);
		
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
