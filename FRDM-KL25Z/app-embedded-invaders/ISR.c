#include <stdint.h>
#include <MKL25Z4.h>
#include <stdbool.h>
#include "ISR.h"
#include "util.h"

volatile int32_t analogY = 0;
volatile double tmp = 0;
volatile bool spawnPlayerBullet = false;

const static uint8_t ADC_CHANNEL_JOY_Y = 13;
const static uint32_t PIN_JOY_SW = 7;

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

void PORTD_IRQHandler(void) {
	if(PORTD->ISFR & (1 << PIN_JOY_SW)) { 
		spawnPlayerBullet = true;
		PORTD->ISFR |= (1 << PIN_JOY_SW);
	}
}
