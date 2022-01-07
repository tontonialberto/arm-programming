#include <system_MKL25Z4.h>
#include <MKL25Z4.h>
#include "delay.h"

// UART1 RX on PTC3
const static uint32_t UART1_RX = 3;

// UART1 TX on PTC4
const static uint32_t UART1_TX = 4;

void UART1_Init(void);

int main() {
	SystemInit();
	
	uint8_t byte = 0;
	
	UART1_Init();

	while(1) {
		
		if( !(UART1->S1 & UART_S1_RDRF_MASK) ) continue;
		
		byte = UART1->D;
		
		while( !(UART1->S1 & UART_S1_TDRE_MASK) );
		UART1->D = byte;
	}
}

void UART1_Init() {
	
	// Enable clock on UART1
	SIM->SCGC4 |= SIM_SCGC4_UART1_MASK;
	
	// Disable UART1 peripheral
	UART1->C2 = 0;
	
	// I've set the bus clock to 20.971MHz, via the CLOCK_SETUP macro.
	// Set UART1 baud rate to 9600
	UART1->BDH = 0;  // SBR = 136
	UART1->BDL = 136;
	
	// Set 8-bit mode, plus basic configuration of UART peripheral
	UART1->C1 = 0;
	
	// Disable fault interrupts for UART1
	UART1->C3 = 0;
	
	// Enable UART1 receiver and transmitter
	UART1->C2 |= UART_C2_RE_MASK | UART_C2_TE_MASK;
	
	// Enable clock on PTA
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
	
	// Set ALT3 = 'UART1 mode' on RX pin
	PORTC->PCR[UART1_RX] &= ~ PORT_PCR_MUX_MASK;
	PORTC->PCR[UART1_RX] |= PORT_PCR_MUX(3);
	
	PORTC->PCR[UART1_TX] &= ~ PORT_PCR_MUX_MASK;
	PORTC->PCR[UART1_TX] |= PORT_PCR_MUX(3);
}
