#ifndef __ALBERTO_TONTONI_HW_INIT__
#define __ALBERTO_TONTONI_HW_INIT__

#include <MKL25Z4.h>

/// \brief Init ADC peripheral. Also configure PCR pins.
/// \note Resolution is set to 16 bit.
/// \note Hardware averaging with 32 samples.
/// \note Interrupts are enabled.
/// \param [in] adc ADC data structure: the peripheral to be configured.
/// \param [in] channel ADC Channel.
/// \param [in] port PORT data structure.
/// \param [in] pin 
void ADC_Init(ADC_Type *adc, uint8_t channel, PORT_Type *port, uint32_t pin);

/// \brief Initialize and enable an I2C peripheral. Also configure PCR pins.
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

#endif
