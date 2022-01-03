#include <MKL25Z4.h>

/**
	Peripherals:
	-Switch on PTE4
	-LED on PTD1
*/

const static uint32_t PIN_SWITCH = 4;
const static uint32_t PIN_LED = 1;

void delayMs(uint32_t ms);

int main() {
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;
	
	PORTD->PCR[PIN_LED] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[PIN_LED] |= PORT_PCR_MUX(1);
	
	// Mux = GPIO, PE = Enable, PS = Pullup
	PORTE->PCR[PIN_SWITCH] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[PIN_SWITCH] |= PORT_PCR_MUX(1) | PORT_PCR_PE(1) | PORT_PCR_PS(1);
	
	GPIOD->PDDR |= (1 << PIN_LED);
	GPIOE->PDDR &= ~(uint32_t)(1 << PIN_SWITCH);
	
	GPIOD->PSOR = (1 << PIN_LED);
	
	while(1) {
		if(GPIOE->PDIR & (1 << PIN_SWITCH)) {
			GPIOD->PSOR = (1 << PIN_LED);
		}
		else {
			GPIOD->PCOR = (1 << PIN_LED);
		}
	}
}

void delayMs(uint32_t ms) {
	uint32_t i;
	uint32_t j;

	for(i=0; i<ms; i++) {
		for(j=0; j<7000; j++) {
			__ASM("nop");
		}
	}
}
