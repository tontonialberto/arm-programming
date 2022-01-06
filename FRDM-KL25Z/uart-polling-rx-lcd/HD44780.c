#include "HD44780.h"
#include <MKL25Z4.h>
#include "delay.h"

// Command pins on PTE
const static uint32_t PIN_RS 	= 20;
const static uint32_t PIN_RW 	= 21;
const static uint32_t PIN_E 	= 22;

// Data pins on PTB
const static uint32_t PIN_D0	=	8;
const static uint32_t PIN_D1 	= 9;
const static uint32_t PIN_D2 	= 10;
const static uint32_t PIN_D3 = 	11;
const static uint32_t PIN_D4	=	0;
const static uint32_t PIN_D5 	= 1;
const static uint32_t PIN_D6 	= 2;
const static uint32_t PIN_D7 = 	3;

// D0...D3 on PTB8...11
const static uint32_t MASK_D03 = 0xf00;
const static uint32_t SHIFT_D03 = 8;

// D4...D7 on PTB0...3
const static uint32_t MASK_D47 = 0xf;
const static uint32_t SHIFT_D47 = 0;

void LCD_Init() {
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTE_MASK;
	
	PORTE->PCR[PIN_RS] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[PIN_RS] |= PORT_PCR_MUX(1);
	
	PORTE->PCR[PIN_RW] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[PIN_RW] |= PORT_PCR_MUX(1);
	
	PORTE->PCR[PIN_E] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[PIN_E] |= PORT_PCR_MUX(1);
	
	PORTB->PCR[PIN_D0] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PIN_D0] |= PORT_PCR_MUX(1);
	
	PORTB->PCR[PIN_D1] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PIN_D1] |= PORT_PCR_MUX(1);
	
	PORTB->PCR[PIN_D2] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PIN_D2] |= PORT_PCR_MUX(1);
	
	PORTB->PCR[PIN_D3] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PIN_D3] |= PORT_PCR_MUX(1);
	
	PORTB->PCR[PIN_D4] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PIN_D4] |= PORT_PCR_MUX(1);
	
	PORTB->PCR[PIN_D5] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PIN_D5] |= PORT_PCR_MUX(1);
	
	PORTB->PCR[PIN_D6] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PIN_D6] |= PORT_PCR_MUX(1);
	
	PORTB->PCR[PIN_D7] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PIN_D7] |= PORT_PCR_MUX(1);
	
	GPIOE->PDDR |= (1 << PIN_RS) | (1 << PIN_RW) | (1 << PIN_E);
	GPIOB->PDDR |= MASK_D03 | MASK_D47;
	
	delayMs(30);
	
	// Set 8-bit mode
	LCD_Command(0x30);
	
	// 8-bit mode, 2 display lines
	LCD_Command(0x38);
}

void LCD_Command(uint8_t command) {
	// 1. Write on Cmd register
	GPIOE->PCOR = (1 << PIN_RS) | (1 << PIN_RW);
	// 2. Write command on data bits
	uint8_t highNibble = (command & 0xf0) >> 4;
	uint8_t lowNibble = command & 0x0f;
	GPIOB->PDOR = (uint32_t)(lowNibble << SHIFT_D03) | (uint32_t)(highNibble << SHIFT_D47);
	// 3. Send pulse
	GPIOE->PCOR = (1 << PIN_E);
	delayMs(0);
	GPIOE->PSOR = (1 << PIN_E);
	delayMs(0);
	GPIOE->PCOR = (1 << PIN_E);
	// 4. Wait to ensure the LCD has processed the command
	LCD_WaitBusy();
}

void LCD_Data(uint8_t data) {
	// Write on data register
	GPIOE->PSOR = (1 << PIN_RS);
	GPIOE->PCOR = (1 << PIN_RW);
	
	// Write on data bits
	GPIOB->PDOR = ((uint32_t)(data & 0xf) << SHIFT_D03) | ((uint32_t)((data & 0xf0) >> 4) << SHIFT_D47);

	// Send pulse
	GPIOE->PCOR = (1 << PIN_E);
	delayMs(0);
	GPIOE->PSOR = (1 << PIN_E);
	delayMs(0);
	GPIOE->PCOR = (1 << PIN_E);
	
	// Wait to complete
	LCD_WaitBusy();
}

void LCD_WriteDigit(int8_t digit) {
	if(digit < 0 || digit > 9) {
		return;
	}
	
	if(digit == 0) LCD_Data('0');
	else if (digit == 1) LCD_Data('1');
	else if (digit == 2) LCD_Data('2');
	else if (digit == 3) LCD_Data('3');
	else if (digit == 4) LCD_Data('4');
	else if (digit == 5) LCD_Data('5');
	else if (digit == 6) LCD_Data('6');
	else if (digit == 7) LCD_Data('7');
	else if (digit == 8) LCD_Data('8');
	else if (digit == 9) LCD_Data('9');
}

void LCD_WriteString(const char *str) {
	while(*str != 0) {
		LCD_Data((uint8_t)*str);
		++str;
	}
}

void LCD_SetCursor(uint8_t line, uint8_t pos) {
	if(line > 1) return;
	
	uint8_t base;
	
	if(line == 0) base = 0x80;
	else base = 0xC0;
	
	LCD_Command(base + pos);
}

void LCD_WriteNumber(uint32_t number) {
	if(number == 0) return;
	
	LCD_WriteNumber(number / 10U);
	
	LCD_WriteDigit(number % 10);
}

void LCD_WaitBusy() {
	uint8_t status, highNibble, lowNibble;
	
	// Set data pins to input mode
	GPIOB->PDDR &= ~MASK_D03;
	GPIOB->PDDR &= ~MASK_D47;
	
	// Read on Cmd register
	GPIOE->PSOR = (1 << PIN_RW); // RW = 1 (read)
	GPIOE->PCOR = (1 << PIN_RS); // RS = 0 (cmd register)
	
	do {
		// Start pulse
		GPIOE->PCOR = (1 << PIN_E);
		delayMs(0);
		GPIOE->PSOR = (1 << PIN_E);
		delayMs(0);
		
		// Store D7...D0 into the status variable
		highNibble = (GPIOB->PDIR & MASK_D47) >> SHIFT_D47;
		lowNibble = (uint8_t)(GPIOB->PDIR & MASK_D03) >> SHIFT_D03;
		status = (uint8_t)(highNibble << 4) | (uint8_t)lowNibble; 
		
		// End pulse
		GPIOE->PCOR = (1 << PIN_E);
	} while( status & (1 << 7) ); // D7 is the Busy flag
	
	// Set data pins back to output mode
	GPIOB->PDDR |= MASK_D03 | MASK_D47;
}
