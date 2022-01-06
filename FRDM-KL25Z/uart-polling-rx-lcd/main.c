#include <MKL25Z4.h>
#include "HD44780.h"
#include "delay.h"

// UART0 RX on PTA1
const static uint32_t UART0_RX = 1;

void UART0_Init(void);

int main() {
	uint8_t byte = 0;
	uint8_t lineIndex = 0; // [0-1] To keep track the position on the display
	uint8_t colIndex = 0; // [0-15] Same as above
	
	LCD_Init();
	UART0_Init();
	
	// Set display on, cursor off
	LCD_Command(0xc);
	
	// Clear display, cursor home
	LCD_Command(0x1);
	
	while(1) {
		if( !(UART0->S1 & UART_S1_RDRF_MASK) ) continue;
		
		byte = UART0->D;
		
		// Switch between the two LCD lines
		// when one is filled.
		// Moreover, clear the screen when both
		// lines are filled.
		if(colIndex > 15) {
			if(lineIndex == 0) {
				lineIndex = 1;
			}
			else {
				LCD_Command(0x1);
				lineIndex = 0;
			}
			colIndex = 0;
			LCD_SetCursor(lineIndex, colIndex);
		}
		
		LCD_Data(byte);
		colIndex ++;
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
	PORTA->PCR[UART0_RX] &= ~ PORT_PCR_MUX_MASK;
	PORTA->PCR[UART0_RX] |= PORT_PCR_MUX(2);
}
