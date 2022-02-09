#include <MKL25Z4.h>
#include "SSD1306/SSD1306.h"
#include "I2C_Master.h"
#include "hw_init.h"

void ADC0_IRQHandler(void);

const static uint8_t ADC_CHANNEL_JOY_Y = 13;

static volatile int32_t analogY = 0;

static volatile double tmp = 0;

int main() {
	const uint32_t PIN_JOY_Y = 3;
	
	__disable_irq();
	
	SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
	
	ADC_Init(ADC0, ADC_CHANNEL_JOY_Y, PORTB, PIN_JOY_Y);
	
	__enable_irq();
	
	while(1) {
	}
}

void ADC0_IRQHandler(void) {
	static uint16_t adcVal = 0;
	
	adcVal = (uint16_t)ADC0->R[0];
	
	// Convert to value in interval [-0.5, +0.5]
	tmp = (((double)adcVal) / 65536.0) - 0.5;
	
	// Convert to value in interval [-100, +100]
	analogY = (int32_t)(2 * 100 * tmp);
	
	ADC0->SC1[0] |= ADC_SC1_ADCH(ADC_CHANNEL_JOY_Y);
}
