#include <MKL25Z4.h>
#include "SSD1306/SSD1306.h"
#include "I2C_Master.h"
#include "delay.h"
#include "hw_init.h"
#include "util.h"
#include "GameObject.h"

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
	const uint32_t PIN_JOY_Y = 3;
	const uint32_t PIN_OLED_SDA = 1;
	const uint32_t PIN_OLED_SCL = 0;
	SSD1306_Data oledData;
	
	oledData.buffer = oledBuffer;
	oledData.I2C_Write_BufferSize = 64;
	oledData.I2C_Write = I2C_Write_Adapter;
	oledData.DelayUs = delayUs;
	
	__disable_irq();

	SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	
	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;
	
	ADC_Init(ADC0, ADC_CHANNEL_JOY_Y, PORTB, PIN_JOY_Y);

	I2C_Init(I2C0, PORTB, PIN_OLED_SDA, PIN_OLED_SCL, 2);
	
	__enable_irq();
	
	oledInit = SSD1306_Init(&oledData);
	
	GameContext ctx;
	ctx.playerHorizontalStep = +2;
	ctx.gameAreaMinX = 0;
	ctx.gameAreaMaxX = 127;
	GameObject player;
	player.x = 50;
	player.y = 20;
	player.width = 6;
	player.height = 6;
	player.ctx = &ctx;
	
	delayMs(100);
	
	while(1) {
		delayMs(100);
		SSD1306_Clear(&oledData);
		player.x += ctx.playerHorizontalStep;
		if(player.x < ctx.gameAreaMinX) {
			player.x = ctx.gameAreaMinX;
		}
		else if(player.x + player.width > ctx.gameAreaMaxX) {
			player.x = ctx.gameAreaMaxX - (int16_t)player.width - (int16_t)1;
		}
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
