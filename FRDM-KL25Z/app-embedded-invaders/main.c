#include <MKL25Z4.h>
#include "SSD1306/SSD1306.h"
#include "I2C_Master.h"
#include "delay.h"
#include "hw_init.h"
#include "util.h"
#include "GameObject.h"
#include "GameUtils.h"
#include "ISR.h"

I2C_Result I2C_Write_Adapter(
	uint8_t address, 
	uint8_t *data, 
	uint32_t len, 
	bool restart);

static bool oledInit = false;
static uint8_t oledBuffer[SSD1306_BUFFER_SIZE];

const static int16_t PLAYER_RIGHT_STEP = 3;
const static int16_t PLAYER_BULLET_UP_STEP = -3;

const static int32_t THRESHOLD_JOYSTICK = 50;

const static uint16_t PLAYER_WIDTH = 6;
const static uint16_t PLAYER_HEIGHT = 6;

const static uint16_t PLAYER_BULLET_WIDTH = 2;
const static uint16_t PLAYER_BULLET_HEIGHT = 3;

const static int16_t SCREEN_MIN_X = 0;
const static int16_t SCREEN_MAX_X = 127;
const static int16_t SCREEN_MIN_Y = 0;

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
		
		// Player bullet render
		if(bullet.active) {
			SSD1306_WriteRectangle(&oledData, bullet.x, bullet.y, bullet.width, bullet.height);
		}
		
		// Player render
		SSD1306_WriteRectangle(&oledData, player.x, player.y, player.width, player.height);
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
