#include <MKL25Z4.h>
#include "Delay.h"

const static uint32_t IDX_LED_BLUE = 1;
const static uint32_t MASK_LED_BLUE = 1 << 1;
const static uint32_t PORT_PCR_MUX_GPIO = 1;

volatile uint32_t iters;

int main() {
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
	PORTD->PCR[IDX_LED_BLUE] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[IDX_LED_BLUE] |= PORT_PCR_MUX(PORT_PCR_MUX_GPIO);
	GPIOD->PDDR |= MASK_LED_BLUE;
	GPIOD->PCOR = MASK_LED_BLUE;
	
	while(1) {
		Delay(500);
		GPIOD->PSOR = MASK_LED_BLUE;
		Delay(500);
		GPIOD->PCOR = MASK_LED_BLUE;
	}
	
}
