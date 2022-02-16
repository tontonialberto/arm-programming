#include <MKL25Z4.h>
#include "SSD1306/SSD1306.h"
#include "I2C_Master.h"
#include "delay.h"
#include "hw_init.h"
#include "util.h"
#include "GameObject.h"
#include "GameUtils.h"

void ADC0_IRQHandler(void);

I2C_Result I2C_Write_Adapter(
	uint8_t address, 
	uint8_t *data, 
	uint32_t len, 
	bool restart);
	
int32_t lowPassFilter(int32_t prevFiltered, int32_t currMeasurement, double alpha);

const static uint8_t ADC_CHANNEL_JOY_Y = 13;

static volatile int analogY = 0;
static volatile double tmp = 0;
static volatile bool oledInit = false;
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
	ctx.playerHorizontalStep = +2;
	ctx.playerBulletVerticalStep = -3;
	ctx.gameAreaMinX = 0;
	ctx.gameAreaMaxX = 127;
	ctx.spawnPlayerBullet = true;
	
	GameObject player;
	player.x = 50;
	player.y = 20;
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

void ADC0_IRQHandler(void) {
	uint16_t adcVal = (uint16_t)ADC0->R[0];
	int32_t currAnalogY = 0;
	
	// Convert to value in interval [-0.5, +0.5]
	tmp = (((double)adcVal) / 65536.0) - 0.525;
	
	// Convert to value in interval [-100, +100]
	currAnalogY = (int32_t)(2 * 100 * tmp);
	
	analogY = lowPassFilter(analogY, currAnalogY, 0.9);
	
	// Input check: put the value back to its bounds
	if(analogY < -100)
		analogY = -100;
	else if(analogY > 100)
		analogY = 100;
	
	ADC0->SC1[0] |= ADC_SC1_ADCH(ADC_CHANNEL_JOY_Y);
}

I2C_Result I2C_Write_Adapter(
		uint8_t address, 
		uint8_t *data, 
		uint32_t len, 
		bool restart) {
	
	return I2C_Write(I2C0, address, data, len, restart);
}

int32_t lowPassFilter(int32_t prevFiltered, int32_t currMeasurement, double alpha) {
	return currMeasurement + (int32_t)(alpha * (prevFiltered - currMeasurement));
}
