#include <MKL25Z4.h>
#include <string.h>
#include <stdio.h>
#include "I2C_Master.h"
#include "delay.h"
#include "DFR0555.h"

// I2C1 SDA, SCL pins on PTE0, PTE1
const static uint32_t PIN_SDA = 0;
const static uint32_t PIN_SCL = 1;

//void bounceLineLCD(const char *str, uint8_t line);

int main() {
	char buffer[50];
	I2C_Result i2cres;
	uint32_t count = 0;
	
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTE_MASK;
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK | SIM_SCGC4_I2C1_MASK;
	
	// pins on MUX=6 (that is, I2C1)
	// Enable pull-up resistors
	PORTE->PCR[PIN_SDA] = PORT_PCR_MUX(6) | PORT_PCR_PS(1) | PORT_PCR_PE(1);
	PORTE->PCR[PIN_SCL] = PORT_PCR_MUX(6) | PORT_PCR_PS(1) | PORT_PCR_PE(1);
	
	// Disable I2C module
	I2C1->C1 &= ~I2C_C1_IICEN_MASK;
	
	// Setting I2C1 baud rate at approx 200 kHz
	// Assuming bus clock at 24 MHz (ie. CLOCK_SETUP == 1)
	// Thus the SCL divider must be approx 120 => ICR = 0x1b
	I2C1->F = I2C_F_MULT(0) | I2C_F_ICR(0x1b);
	
	// Enable I2C0, interrupts disabled
	I2C1->C1 |= I2C_C1_IICEN_MASK;

	delayMs(100);
	
	i2cres = DFR0555_Init(I2C1);
	delayMs(1);
	
	strcpy(buffer, " Hello world");
	
	DFR0555_SetCursor(I2C1, 1, 0);
	delayMs(1);
	
	DFR0555_WriteString(I2C1,  buffer);
	
	while(1) {
		delayMs(1000);
		
		// Write the remainder of count/10 on line 0
		DFR0555_SetCursor(I2C1, 0, 0);
		delayMs(1);
		DFR0555_WriteChar(I2C1, (char)(count % 10) + '0');
		
		count ++;
	}
}
