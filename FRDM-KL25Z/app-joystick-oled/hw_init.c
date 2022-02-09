#include "hw_init.h"

void ADC_Init(ADC_Type *adc, uint8_t channel, PORT_Type *port, uint32_t pin) {
	
	// Software trigger mode.
	adc->SC2 &= ~ADC_SC2_ADTRG_MASK;
	
	// Single-ended 16-bit conversion.
	adc->CFG1 = ADC_CFG1_MODE(3);
	
	// Enable hardware average, 32 samples.
	adc->SC3 |= ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(3);
	
	// Enable ADC interrupts.
	adc->SC1[0] = ADC_SC1_ADCH(channel) 
		| ADC_SC1_AIEN_MASK;
	
	NVIC_EnableIRQ(ADC0_IRQn);
	
	port->PCR[pin] &= ~PORT_PCR_MUX_MASK;
}

void I2C_Init(
		I2C_Type *i2c, 
		PORT_Type *port, 
		uint32_t pinSDA, 
		uint32_t pinSCL, 
		uint32_t alt) {
			
	// Disable I2C module
	i2c->C1 &= ~I2C_C1_IICEN_MASK;
	
	port->PCR[pinSDA] = PORT_PCR_MUX(alt) | PORT_PCR_PS(1) | PORT_PCR_PE(1);
	port->PCR[pinSCL] = PORT_PCR_MUX(alt) | PORT_PCR_PS(1) | PORT_PCR_PE(1);
	
	// Setting I2C baud rate at approx 200 kHz
	// Assuming bus clock at 24 MHz (ie. CLOCK_SETUP == 1)
	// Thus the SCL divider must be approx 120 => ICR = 0x1b
	i2c->F = I2C_F_MULT(0) | I2C_F_ICR(0x1b);
	
	// Enable I2C0, interrupts disabled
	i2c->C1 |= I2C_C1_IICEN_MASK;
}
