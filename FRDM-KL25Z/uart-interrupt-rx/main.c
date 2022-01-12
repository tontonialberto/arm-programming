#include <MKL25Z4.h>

// Using UART0 RX,TX on PTA1, PTA2
const static uint32_t PIN_RX = 1;
const static uint32_t PIN_TX = 2;

// Built-in r,g,b leds on PTB18, PTB19, PTD1
const static uint32_t PIN_LED_RED = 18;

volatile static char rxChar = 0;

int main() {
	__disable_irq();
	
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK;

	PORTB->PCR[PIN_LED_RED] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PIN_LED_RED] |= PORT_PCR_MUX(1);
	GPIOB->PDDR |= (1 << PIN_LED_RED);
	GPIOB->PSOR = (1 << PIN_LED_RED);
	
	PORTA->PCR[PIN_RX] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[PIN_RX] |= PORT_PCR_MUX(2); // ALT2 = UART0_RX
	PORTA->PCR[PIN_TX] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[PIN_TX] |= PORT_PCR_MUX(2); // ALT2 = UART0_TX
	
	// UART0 source clock will be PLL clock
	SIM->SOPT2 &= ~SIM_SOPT2_UART0SRC_MASK & ~SIM_SOPT2_PLLFLLSEL_MASK;
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1) | SIM_SOPT2_PLLFLLSEL(1);
	
	// Disable UART0 tx,rx before configuration
	UART0->C2 = 0;
	
	// Set UART0 baud rate to 115200
	// Assuming clock at 48MHz (ie. macro CLOCK_SETUP == 1)
	UART0->C4 = 15; // OSR = 15
	UART0->BDH = 0; // SBR = 26
	UART0->BDL = 26; 
	
	UART0->C1 = 0;
	
	// Enable UART0 rx, and rx interrupts
	UART0->C2 = UART0_C2_RIE_MASK | UART0_C2_RE_MASK;
	
	NVIC_EnableIRQ(UART0_IRQn);
	__enable_irq();
	
	while(1) {}
}

void UART0_IRQHandler(void) {
	
	// Read the received character.
	// If it's equal to 's' (set) -> set the red LED
	// If it's equal to 'c' (clear) -> clear it
	rxChar = UART0->D;
	if('s' == rxChar) 
		GPIOB->PCOR = (1 << PIN_LED_RED);
	else if('c' == rxChar) 
		GPIOB->PSOR = (1 << PIN_LED_RED);
}
