#include <MKL25Z4.h>

/**
	PORTA pin 1 is the "heartbeat" pin.
*/


const static uint32_t PIN = 1;

int main() {
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
	
	PORTA->PCR[PIN] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[PIN] |= PORT_PCR_MUX(1);
	
	GPIOA->PDDR |= (1 << PIN);
	
	for(int i=0; i>-1; i++) {
		GPIOA->PTOR = (1 << PIN);
	}
}
