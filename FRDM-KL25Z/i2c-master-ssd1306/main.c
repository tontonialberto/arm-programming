#include <MKL25Z4.h>
#include "I2C_Master.h"
#include "SSD1306/SSD1306.h"
#include "delay.h"

I2C_Result I2C0_Write_Adapter(
	uint8_t address, 
	uint8_t *data, 
	uint32_t len, 
	bool restart);
	
/// \brief Initialize an I2C peripheral (configure PCR pins, set baud rate, enable peripheral).
/// \note I2C is supplied with bus clock.
/// \note Bus clock speed is assumed to be 24MHz.
/// \note Baud rate is set to 200kHz.
/// \param [in] i2c I2C data structure: the peripheral to be enabled.
/// \param [in] port PORT data structure. Contains SCL, SDA pins.
/// \param [in] pinSDA
/// \param [in] pinSCL
/// \param [in] alt Alternate mode, corresponding to I2C function for the chosen pins.
void I2C_Init(
	I2C_Type *i2c, 
	PORT_Type *port, 
	uint32_t pinSDA, 
	uint32_t pinSCL, 
	uint32_t alt);

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
	
	I2C_Init(I2C0, PORT_I2C, PIN_SDA, PIN_SCL, 2);
	
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

void I2C_Init(
		I2C_Type *i2c, 
		PORT_Type *port, 
		uint32_t pinSDA, 
		uint32_t pinSCL, 
		uint32_t alt) {
			
	port->PCR[pinSDA] = PORT_PCR_MUX(alt) | PORT_PCR_PS(1) | PORT_PCR_PE(1);
	port->PCR[pinSCL] = PORT_PCR_MUX(alt) | PORT_PCR_PS(1) | PORT_PCR_PE(1);
	
	// Disable I2C module
	i2c->C1 &= ~I2C_C1_IICEN_MASK;
	
	// Setting I2C baud rate at approx 200 kHz
	// Assuming bus clock at 24 MHz (ie. CLOCK_SETUP == 1)
	// Thus the SCL divider must be approx 120 => ICR = 0x1b
	i2c->F = I2C_F_MULT(0) | I2C_F_ICR(0x1b);
	
	// Enable I2C0, interrupts disabled
	i2c->C1 |= I2C_C1_IICEN_MASK;
}
