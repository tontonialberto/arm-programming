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

int main() {
	SSD1306_Data oledData;
	
	HardwareInit();
	
	oledData.buffer = oledBuffer;
	oledData.I2C_Write_BufferSize = 64;
	oledData.I2C_Write = I2C_Write_Adapter;
	oledData.DelayUs = delayUs;
	
	oledInit = SSD1306_Init(&oledData);
	
	GameContext ctx;
	ctx.playerBulletVerticalStep = -3;
	ctx.gameAreaMinX = 0;
	ctx.gameAreaMaxX = 127;
	ctx.spawnPlayerBullet = true;
	
	GameObject player;
	player.x = 56;
	player.y = 24;
	player.width = 6;
	player.height = 6;
	player.ctx = &ctx;
	
	GameObject bullet;
	bullet.width = 2;
	bullet.height = 3;
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
		
		if(analogY > 50) {
			ctx.playerHorizontalStep = +3;
		}
		else if(analogY < -50) {
			ctx.playerHorizontalStep = -3;
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
		if(bullet.y < 0) {
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
