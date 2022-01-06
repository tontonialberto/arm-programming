#include <MKL25Z4.h>
#include <stdlib.h>
#include "HD44780.h"
#include "delay.h"

// UART0 RX on PTA1
const static uint32_t UART0_RX = 1;

// Red LED on PTB18
const static uint32_t MASK_LED_RED = (1 << 18);
const static uint32_t PIN_LED_RED = 18;

// Green LED on PTB19
const static uint32_t MASK_LED_GREEN = (1 << 19);
const static uint32_t PIN_LED_GREEN = 19;

// Blue LED on PTD1
const static uint32_t MASK_LED_BLUE = (1 << 1);
const static uint32_t PIN_LED_BLUE = 1;

void UART0_Init(void);

void LED_Init(void);

/**
Precondition:
	-status is a value within [0-7] ie. represents 3 bits (b2, b1, b0)
Postcondition:
	-RED = b2
	-GREEN = b1
	-BLUE = b0
*/
void LED_Set(uint8_t status);

int main() {
	uint8_t byte = 0;
	uint8_t ledStatus = 0;
	
	LCD_Init();
	UART0_Init();
	LED_Init();
	
	// Turn off all LEDs
	GPIOB->PSOR = MASK_LED_RED | MASK_LED_GREEN;
	GPIOD->PSOR = MASK_LED_BLUE;
	
	// Set display on, cursor off
	LCD_Command(0xc);
	
	// Clear display, cursor home
	LCD_Command(0x1);
	
	while(1) {
		if( !(UART0->S1 & UART_S1_RDRF_MASK) ) continue;
		
		byte = UART0->D;
		
		// Only command from '0' to '7' is valid
		if((byte < (uint8_t)'0') || (byte > (uint8_t)'7')) continue;
		
		ledStatus = byte - (uint8_t)'0';
		LED_Set(ledStatus);
		
		LCD_Command(0x1);
		LCD_WriteString("R: ");
		LCD_WriteDigit((ledStatus & 0x4) ? 1 : 0);
		LCD_WriteString(", G: ");
		LCD_WriteDigit((ledStatus & 0x2) ? 1 : 0);
		LCD_WriteString(", B: ");
		LCD_WriteDigit((ledStatus & 0x1) ? 1 : 0);
	}
}

void UART0_Init() {
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
	
	// Enable UART0 receiver
	UART0->C2 |= UART_C2_RE_MASK;
	
	// Enable clock on PTA
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
	
	// Set ALT2 = 'UART0 mode' on RX pin
	PORTA->PCR[UART0_RX] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[UART0_RX] |= PORT_PCR_MUX(2);
}

void LED_Init() {
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;
	PORTB->PCR[PIN_LED_RED] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PIN_LED_RED] |= PORT_PCR_MUX(1);
	PORTB->PCR[PIN_LED_GREEN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PIN_LED_GREEN] |= PORT_PCR_MUX(1);
	PORTD->PCR[PIN_LED_BLUE] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[PIN_LED_BLUE] |= PORT_PCR_MUX(1);
	GPIOB->PDDR |= MASK_LED_RED | MASK_LED_GREEN;
	GPIOD->PDDR |= MASK_LED_BLUE;
}

void LED_Set(uint8_t status) {
	if(status & 0x1) {
		GPIOD->PSOR = MASK_LED_BLUE;
	}
	else {
		GPIOD->PCOR = MASK_LED_BLUE;
	}
	
	if(status & 0x2) {
		GPIOB->PSOR = MASK_LED_GREEN;
	}
	else {
		GPIOB->PCOR = MASK_LED_GREEN;
	}
	
	if(status & 0x4) {
		GPIOB->PSOR = MASK_LED_RED;
	}
	else {
		GPIOB->PCOR = MASK_LED_RED;
	}
}
