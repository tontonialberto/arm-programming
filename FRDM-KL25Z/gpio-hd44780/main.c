#include <MKL25Z4.h>

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

void delayMs(uint32_t ms);

void LCD_Command(uint8_t command);

// WriteDigit(uint8_t digit)
// WriteChar(uint8_t char)
// WriteString(const char *str)

int main() {
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
	
	// Set blinking cursor
	LCD_Command(0xf);
	
	while(1) {
		__ASM("nop");
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
	delayMs(10);
}
