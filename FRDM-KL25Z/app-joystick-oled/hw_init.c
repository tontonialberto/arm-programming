#include "hw_init.h"

void ADC_Init(ADC_Type *adc, uint8_t channel, PORT_Type *port, uint32_t pin) {
	
	// Software trigger mode.
	adc->SC2 &= ~ADC_SC2_ADTRG_MASK;
	
	// Single-ended 16-bit conversion.
	adc->CFG1 = ADC_CFG1_MODE(3);
	
	// Enable hardware average, 32 samples.
	adc->SC3 |= ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(3);
	
	// Enable ADC interrupts.
	adc->SC1[0] = ADC_SC1_ADCH(channel) 
		| ADC_SC1_AIEN_MASK;
	
	NVIC_EnableIRQ(ADC0_IRQn);
	
	port->PCR[pin] &= ~PORT_PCR_MUX_MASK;
}
