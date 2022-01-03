#include <MKL25Z4.h>

/**
	LED Red: PTB18
	LED Green: PTB19
	LED Blue: PTD1
*/

const static uint32_t IDX_LED_RED = 18;
const static uint32_t IDX_LED_GREEN = 19;
const static uint32_t IDX_LED_BLUE = 1;
const static uint32_t PORT_PCR_MUX_GPIO = 1;

/**
	bit 0: red
	bit 1: green
	bit 2: blue
*/
static uint8_t color = 0;

void delayMs(uint32_t ms);

void ledsAlternate(void);

void ledsMixed(void);

int main() {
	// Enable clock on PORTB and PORTD
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;
	
	// Set Blue LED's pin MUX = GPIO
	PORTD->PCR[IDX_LED_BLUE] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[IDX_LED_BLUE] |= PORT_PCR_MUX(PORT_PCR_MUX_GPIO);
	
	// Set Red LED's pin MUX = GPIO
	PORTB->PCR[IDX_LED_RED] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[IDX_LED_RED] |= PORT_PCR_MUX(PORT_PCR_MUX_GPIO);
	
	// Set Green LED's pin MUX = GPIO
	PORTB->PCR[IDX_LED_GREEN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[IDX_LED_GREEN] |= PORT_PCR_MUX(PORT_PCR_MUX_GPIO);
	
	// Set LED pins direction as output
	GPIOD->PDDR |= (1 << IDX_LED_BLUE);
	GPIOB->PDDR |= (1 << IDX_LED_RED) | (1 << IDX_LED_GREEN);
	
	GPIOD->PSOR = (1 << IDX_LED_BLUE);
	GPIOB->PSOR = (1 << IDX_LED_RED) | (1 << IDX_LED_GREEN);
	
	while(1) {
		ledsAlternate();
		ledsMixed();
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

void ledsAlternate() {
	// Red on
	GPIOB->PCOR = (1 << IDX_LED_RED);
	GPIOB->PSOR = (1 << IDX_LED_GREEN);
	GPIOD->PSOR = (1 << IDX_LED_BLUE);
	
	delayMs(500);
	
	// Green on
	GPIOB->PSOR = (1 << IDX_LED_RED);
	GPIOB->PCOR = (1 << IDX_LED_GREEN);
	GPIOD->PSOR = (1 << IDX_LED_BLUE);
	
	delayMs(500);
	
	// Blue on
	GPIOB->PSOR = (1 << IDX_LED_RED);
	GPIOB->PSOR = (1 << IDX_LED_GREEN);
	GPIOD->PCOR = (1 << IDX_LED_BLUE);
	
	delayMs(500);
}

void ledsMixed() {
	
	for(color = 0; color<8; color++) {
	
		if(color & (1 << 0)) {
			GPIOB->PCOR = (1 << IDX_LED_RED);
		}
		else {
			GPIOB->PSOR = (1 << IDX_LED_RED); 
		}
		
		if(color & (1 << 1)) {
			GPIOB->PCOR = (1 << IDX_LED_GREEN);
		}
		else {
			GPIOB->PSOR = (1 << IDX_LED_GREEN); 
		}
		
		if(color & (1 << 2)) {
			GPIOD->PCOR = (1 << IDX_LED_BLUE);
		}
		else {
			GPIOD->PSOR = (1 << IDX_LED_BLUE); 
		}
		
		delayMs(500);
	}
}
