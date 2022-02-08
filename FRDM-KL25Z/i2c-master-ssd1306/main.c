#include <MKL25Z4.h>
#include "I2C_Master.h"
#include "SSD1306/SSD1306.h"
#include "delay.h"

I2C_Result I2C0_Write_Adapter(
	uint8_t address, 
	uint8_t *data, 
	uint32_t len, 
	bool restart);
	
volatile static bool oledInit = false;

static uint8_t oledBuffer[SSD1306_BUFFER_SIZE];

int main() {
	// OLED SDA,SCL pins on PTB1,0
	const uint32_t PIN_SDA = 1;
	const uint32_t PIN_SCL = 0;
	PORT_Type *PORT_I2C = PORTB;
	SSD1306_Data oledData;
	
	oledData.buffer = oledBuffer;
	oledData.I2C_Write_BufferSize = 64;
	oledData.I2C_Write = I2C0_Write_Adapter;
	oledData.DelayUs = delayUs;
	
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;
	
	PORT_I2C->PCR[PIN_SDA] = PORT_PCR_MUX(2) | PORT_PCR_PS(1) | PORT_PCR_PE(1);
	PORT_I2C->PCR[PIN_SCL] = PORT_PCR_MUX(2) | PORT_PCR_PS(1) | PORT_PCR_PE(1);
	
	// Disable I2C module
	I2C0->C1 &= ~I2C_C1_IICEN_MASK;
	
	// Setting I2C baud rate at approx 200 kHz
	// Assuming bus clock at 24 MHz (ie. CLOCK_SETUP == 1)
	// Thus the SCL divider must be approx 120 => ICR = 0x1b
	I2C0->F = I2C_F_MULT(0) | I2C_F_ICR(0x1b);
	
	// Enable I2C0, interrupts disabled
	I2C0->C1 |= I2C_C1_IICEN_MASK;
	
	delayMs(100);
	
	oledInit = SSD1306_Init(&oledData);
	
	while(1) {
		SSD1306_Clear(&oledData);
		SSD1306_WriteLineHoriz(&oledData, 30, 10, 90);
		SSD1306_Update(&oledData);
		
		delayMs(1000);
		
		SSD1306_Clear(&oledData);
		SSD1306_WriteLineHoriz(&oledData, 30, 20, 90);
		SSD1306_Update(&oledData);
		
		delayMs(1000);
	}
}

I2C_Result I2C0_Write_Adapter(
	uint8_t address, 
	uint8_t *data, 
	uint32_t len, 
	bool restart) {
	
		return I2C_Write(I2C0, address, data, len, restart);
}
