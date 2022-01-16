#include <MKL25Z4.h>
#include "I2C_Master.h"

void delayMs(uint32_t ms);

// Note: may block forever, and message must be a zero-ended string,
// otherwise buffer overflows may occur.
void UART0_Write(UART0_Type *uart, const char *message);

// I2C1 SDA, SCL pins on PTE0, PTE1
const static uint32_t PIN_SDA = 0;
const static uint32_t PIN_SCL = 1;

// UART0 RX, TX pins on PTA1, PTA2
const static uint32_t PIN_TX = 2;

const static uint8_t ADDR_MPU6050 = 0x68;
const static uint8_t REG_WHOAMI = 0x75;
const static uint8_t REG_ACCEL_BASE = 0x3b;
const static uint8_t REG_PWR_MGMT_1 = 0x6b;
const static uint8_t REG_SIGNAL_PATH_RESET = 0x68;

static uint8_t buffer[20];
static uint8_t readByte;
static volatile I2C_Result i2cresult;

int main() {
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTE_MASK;
	SIM->SCGC4 |= SIM_SCGC4_I2C1_MASK | SIM_SCGC4_UART0_MASK;
	
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
	
	// Enable transmitter
	UART0->C2 |= UART_C2_TE_MASK;
	
	/** UART0 configuration end **/
	
	delayMs(10);
	
	// Reset MPU6050
	do {
		buffer[0] = REG_PWR_MGMT_1;
		buffer[1] = 0x00; // bit7 = 1 means "DEVICE_RESET"
		i2cresult = I2C_Write(I2C1, ADDR_MPU6050, buffer, 2, 1); 
		
		if(SUCCESS != i2cresult) continue;
		
		buffer[0] = REG_SIGNAL_PATH_RESET;
		buffer[1] = 0x2; // Reset accelerometer
		i2cresult = I2C_Write(I2C1, ADDR_MPU6050, buffer, 2, 0);
	} while(SUCCESS != i2cresult);
	
	while(1) {
		i2cresult = I2C_ReadBurst(I2C1, ADDR_MPU6050, REG_WHOAMI, buffer, 1);
		UART0_Write(UART0, "Hello world\r\n");
		delayMs(1000);

		// Read accel and temperature
		//i2cresult = I2C_ReadBurst(I2C1, ADDR_MPU6050, REG_ACCEL_BASE, buffer, 8);
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

void UART0_Write(UART0_Type *uart, const char *message) {
	
	while(*message) {
		while(! (uart->S1 & UART_S1_TDRE_MASK) );
		uart->D = *message;
		++message;
	}
	
	while(! (uart->S1 & UART_S1_TC_MASK) );
}
