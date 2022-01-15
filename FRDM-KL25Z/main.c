#include <MKL25Z4.h>
#include "I2C_Master.h"

// I2C1 SDA, SCL pins on PTE0, PTE1
const static uint32_t PIN_SDA = 0;
const static uint32_t PIN_SCL = 1;

const static uint8_t ADDR_MPU6050 = 0x68;
const static uint8_t REG_WHOAMI = 0x75;
const static uint8_t REG_TEMP_OUT = 0x41;

static uint8_t buffer[20];
static uint8_t readByte;
static volatile I2C_Result i2cresult;

int main() {
	SIM->SCGC4 |= SIM_SCGC4_I2C1_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	
	// pins on MUX=6 (that is, I2C1)
	// Enable pull-up resistors
	/*
	PORTE->PCR[PIN_SDA] &= ~PORT_PCR_MUX_MASK 
		& ~PORT_PCR_PS_MASK 
		& ~PORT_PCR_PE_MASK;
	*/
	PORTE->PCR[PIN_SDA] = PORT_PCR_MUX(6) | PORT_PCR_PS(1) | PORT_PCR_PE(1);
	/*
	PORTE->PCR[PIN_SCL] &= ~PORT_PCR_MUX_MASK 
		& ~PORT_PCR_PS_MASK 
		& ~PORT_PCR_PE_MASK;
	*/
	PORTE->PCR[PIN_SCL] = PORT_PCR_MUX(6) | PORT_PCR_PS(1) | PORT_PCR_PE(1);
	
	// Disable I2C module
	I2C1->C1 &= ~I2C_C1_IICEN_MASK;
	
	// Setting I2C1 baud rate at approx 200 kHz
	// Assuming bus clock at 24 MHz (ie. CLOCK_SETUP == 1)
	// Thus the SCL divider must be approx 120 => ICR = 0x1b
	I2C1->F = I2C_F_MULT(0) | I2C_F_ICR(0x1b);
	
	// Enable I2C0, interrupts disabled
	I2C1->C1 |= I2C_C1_IICEN_MASK;
	
	while(1) {
		/*
		buffer[0] = REG_WHOAMI;
		
		i2cresult = I2C_Write(I2C1, ADDR_MPU6050, buffer, 1, 0);
		
		i2cresult = I2C_Read(I2C1, ADDR_MPU6050, &readByte);
		*/
		
		// Read TEMP_OUT_H and TEMP_OUT_L 8-bit registers
		i2cresult = I2C_ReadBurst(I2C1, ADDR_MPU6050, REG_TEMP_OUT, buffer, 2);
	}
}

