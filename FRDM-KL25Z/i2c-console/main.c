#include <MKL25Z4.h>
#include <string.h>
#include <stdio.h>
#include "I2C_Master.h"
#include "Console.h"

void delayMs(uint32_t ms);
void Console_CommandEcho(UART0_Type *uart, uint8_t *buffer, uint32_t len);
_Bool isHexChar(char c);
uint8_t hexCharToBase10(uint8_t c);

// I2C1 SDA, SCL pins on PTE0, PTE1
const static uint32_t PIN_SDA = 0;
const static uint32_t PIN_SCL = 1;

// UART0 RX, TX pins on PTA1, PTA2
const static uint32_t PIN_RX = 1;
const static uint32_t PIN_TX = 2;

int main() {
	char buffer[50];
	char uartBuffer[100];
	I2C_Result i2cres;
	uint8_t i2cAddr = 0;
	
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTE_MASK;
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK | SIM_SCGC4_I2C1_MASK;
	
	PORTA->PCR[PIN_RX] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[PIN_RX] |= PORT_PCR_MUX(2);
	PORTA->PCR[PIN_TX] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[PIN_TX] |= PORT_PCR_MUX(2);
	
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
	
	/** UART0 configuration begin **/
	
	// Using PLL as clock source
	SIM->SOPT2 &= ~SIM_SOPT2_PLLFLLSEL_MASK & ~SIM_SOPT2_UART0SRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL(1) | SIM_SOPT2_UART0SRC(1);
	
	UART0->C2 = 0;
	
	// 115200 baud rate (OSR=15, SBR=26)
	UART0->BDH = 0;
	UART0->BDL = 26;
	UART0->C4 = 15;
	
	UART0->C1 = 0;
	UART0->C3 = 0;
	
	// Enable transmitter and receiver
	UART0->C2 |= UART_C2_TE_MASK | UART_C2_RE_MASK;
	
	/** UART0 configuration end **/

	delayMs(1);
	
	while(1) {
		// Read command from console
		Console_CommandEcho(UART0, uartBuffer, 100);
		
		// Check if recognized command
		if( (0 == strncmp("slave ", uartBuffer, strlen("slave ")))
				&& isHexChar(uartBuffer[6])
				&& isHexChar(uartBuffer[7])) {
			
			// Go to next line
			UART0_Write(UART0, "\r\n");
					
			// Convert the received address in hex
			i2cAddr = (hexCharToBase10(uartBuffer[6]) * 16) + hexCharToBase10(uartBuffer[7]);
			
			// Check ACK from I2C device.
			i2cres = I2C_Write(I2C1, i2cAddr, buffer, 0, 0);
			
			// Copy the i2c address inside the buffer		
			buffer[0] = uartBuffer[6]; 
			buffer[1] = uartBuffer[7];
			buffer[2] = 0;
			
			if(SUCCESS == i2cres) {
				UART0_Write(UART0, "Ok, device ");
				UART0_Write(UART0, buffer);
				UART0_Write(UART0, " is connected\r\n");
			}
			else {
				UART0_Write(UART0, "Error: could not find device ");
				UART0_Write(UART0, buffer);
				UART0_Write(UART0, "\r\n");
			}
		}
		else if(0 == strncmp("write ", uartBuffer, strlen("write "))) {
			// Parse pairs of characters and convert to base 10.
			// Each pair is separated from the others with a space.
			int i = 0;
			for(char *ptr = uartBuffer + 6; *ptr != '.'; ptr += 3) {
				buffer[i] = (hexCharToBase10(*ptr) * 16) + hexCharToBase10( *(ptr+1) );
				i++;
			}
			
			i2cres = I2C_Write(I2C1, i2cAddr, buffer, (uint32_t)i, 0);
			
			if(SUCCESS == i2cres) {
				UART0_Write(UART0, "\r\nData sent successfully\r\n");
			}
			else {
				UART0_Write(UART0, "\r\nError\r\n");
			}
		}
		else if(0 == strncmp("discover.", uartBuffer, strlen("discover."))) {
			_Bool foundSomething = 0;
			// Try all possible 7-bit addresses.
			// Show those which answer with ACK.
			for(uint8_t addr=1; addr < 128; addr++) {
				i2cres = I2C_Write(I2C1, addr, buffer, 0, 0);
				delayMs(1);
				if(SUCCESS == i2cres) {
					foundSomething = 1;
					snprintf(uartBuffer, 100, "\r\nFound device with address 0x%x\r\n", addr);
					UART0_Write(UART0, uartBuffer);
				}
			}
			
			if(!foundSomething) {
				UART0_Write(UART0, "\r\nNo device found\r\n");
			}
		}
		else {
			UART0_Write(UART0, "\r\nError: unrecognized command\r\n");
		}
	}
}

void delayMs(uint32_t ms) {
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	
	// Timeout after 1 ms
	SysTick->LOAD = (SystemCoreClock / 1000) - 1;
	
	for(uint32_t i=0; i<ms; i++) {
		while(! (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) );
	}
	
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void Console_CommandEcho(UART0_Type *uart, uint8_t *buffer, uint32_t len) {
	uint32_t i = 0;
	char c = 0;
	
	memset(buffer, 0, len);
	// The dot is the command terminator
	while((c != '.') && (i < len - 1)) {
		while(! (uart->S1 & UART_S1_RDRF_MASK) );
		c = uart->D;
		buffer[i++] = c;
		
		// Echo the received character
		uart->D = c;
		while(! (uart->S1 & UART_S1_TC_MASK) );
	}
	buffer[i] = 0;
}

_Bool isHexChar(char c) {
	return ((c >= '0') && (c <= '9')) 
		|| ((c >= 'a') && (c <= 'f'))
	  || ((c >= 'A') && (c <= 'F'));
}

uint8_t hexCharToBase10(uint8_t c) {

	if((c >= '0') && (c <= '9'))
		return c - '0';
	else if((c >= 'a') && (c <= 'f'))
		return c - 'a' + 10;
	else if((c >= 'A') && (c <= 'F')) 
		return c - 'A' + 10;
	else
		return 0;
} 
