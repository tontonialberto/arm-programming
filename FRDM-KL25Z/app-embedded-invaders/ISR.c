#include <stdint.h>
#include <MKL25Z4.h>
#include <stdbool.h>
#include "ISR.h"
#include "PinConfig.h"

volatile int32_t analogY = 0;
static volatile double tmp = 0;

void ADC0_IRQHandler(void) {
	uint16_t adcVal = (uint16_t)ADC0->R[0];
	int32_t currAnalogY = 0;
	
	// Convert to value in interval [-0.5, +0.5]
	tmp = (((double)adcVal) / 65536.0) - 0.525;
	
	// Convert to value in interval [-100, +100]
	currAnalogY = (int32_t)(2 * 100 * tmp);
	
	analogY = currAnalogY;
	
	// Input check: put the value back to its bounds
	if(analogY < -100)
		analogY = -100;
	else if(analogY > 100)
		analogY = 100;
	
	ADC0->SC1[0] |= ADC_SC1_ADCH(ADC_CHANNEL_JOY_Y);
}

volatile bool spawnPlayerBullet = false;

void PORTD_IRQHandler(void) {
	if(PORTD->ISFR & (1 << PIN_JOY_SW)) { 
		spawnPlayerBullet = true;
		PORTD->ISFR |= (1 << PIN_JOY_SW);
	}
}

volatile uint32_t elapsedMs = 0;

void TPM0_IRQHandler(void) {
	elapsedMs ++;
	TPM0->SC |= TPM_SC_TOF_MASK;
}

