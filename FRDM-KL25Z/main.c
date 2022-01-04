#include <MKL25Z4.h>

// UART0 on PTA1,PTA2
const static uint8_t UART0_TX = 2;

void delayMs(uint32_t ms);

volatile static uint32_t freq;

int main() {
	
	freq = SystemCoreClock;
	
	// Enable clock on UART0
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;

	// Set MCGFLLCLK (41.94/2 MHz) as UART0 clock source
	SIM->SOPT2 &= ~SIM_SOPT2_PLLFLLSEL_MASK;
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);
	
	// Disable UART0 peripheral
	UART0->C2 = 0;
	
	// Set UART0 baud rate to 115200
	UART0->C4 = 15; // OSR = 15
	UART0->BDH = 0;  // SBR = 11
	UART0->BDL = 11;
	
	// Set 8-bit mode, plus basic configuration of UART peripheral
	UART0->C1 = 0;
	
	// Enable UART0 transmitter
	UART0->C2 |= UART_C2_TE_MASK;
	
	// Enable clock on PTA
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
	
	// Set ALT2 = 'UART0 mode' on TX pin
	PORTA->PCR[UART0_TX] &= ~ PORT_PCR_MUX_MASK;
	PORTA->PCR[UART0_TX] |= PORT_PCR_MUX(2);
	
	// Here, the UART0 setup is completed.
	
	const char * const message = "Hello world\r\n";
	const char *p = 0;
	
	while(1) {
		p = message;
		while(*p) {
			// Wait until there is space for a character
			while( !(UART0->S1 & UART_S1_TDRE_MASK) );
			UART0->D = *p;
			++p;
		}
		// Wait until the last byte is sent
		while( !(UART0->S1 & UART_S1_TC_MASK) );
		
		delayMs(1000);
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
