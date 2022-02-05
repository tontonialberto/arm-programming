#include <MKL25Z4.h>

const static uint32_t ADC_CHANNEL_JOY_Y = 13;
const static uint32_t PIN_JOY_Y = 3;

static volatile uint16_t adcVal = 0;

void ADC0_IRQHandler(void);

int main() {
	__disable_irq();
	
	SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;
	
	ADC0->SC2 &= ~ADC_SC2_ADTRG_MASK;
	
	ADC0->CFG1 = ADC_CFG1_MODE(3);
	
	ADC0->SC3 |= ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(3);
	
	ADC0->SC1[0] = ADC_SC1_ADCH(ADC_CHANNEL_JOY_Y) 
		| ADC_SC1_AIEN_MASK;
	
	NVIC_EnableIRQ(ADC0_IRQn);
	
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
	PORTB->PCR[PIN_JOY_Y] &= ~PORT_PCR_MUX_MASK;
	
	__enable_irq();
	
	while(1) {
	}
}

void ADC0_IRQHandler(void) {
	adcVal = (uint16_t)ADC0->R[0];
	ADC0->SC1[0] |= ADC_SC1_ADCH(ADC_CHANNEL_JOY_Y);
}
