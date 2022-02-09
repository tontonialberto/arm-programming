#include <MKL25Z4.h>
#include "SSD1306/SSD1306.h"
#include "I2C_Master.h"
#include "delay.h"
#include "hw_init.h"

void ADC0_IRQHandler(void);

I2C_Result I2C_Write_Adapter(
	uint8_t address, 
	uint8_t *data, 
	uint32_t len, 
	bool restart);

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
	
	delayMs(100);
	
	while(1) {
		delayMs(100);
		SSD1306_Clear(&oledData);
		
		if(analogY > 0) {
			SSD1306_WriteDigit(&oledData, 1, 0, 0);
		}
		else {
			SSD1306_WriteDigit(&oledData, 0, 0, 0);
		}
		SSD1306_Update(&oledData);
	}
}

void ADC0_IRQHandler(void) {
	//static uint16_t adcVal = 0;
	
	uint16_t adcVal = (uint16_t)ADC0->R[0];
	
	// Convert to value in interval [-0.5, +0.5]
	tmp = (((double)adcVal) / 65536.0) - 0.5;
	
	// Convert to value in interval [-100, +100]
	analogY = (int32_t)(2 * 100 * tmp);
	
	ADC0->SC1[0] |= ADC_SC1_ADCH(ADC_CHANNEL_JOY_Y);
}

I2C_Result I2C_Write_Adapter(
		uint8_t address, 
		uint8_t *data, 
		uint32_t len, 
		bool restart) {
	
	return I2C_Write(I2C0, address, data, len, restart);
}
