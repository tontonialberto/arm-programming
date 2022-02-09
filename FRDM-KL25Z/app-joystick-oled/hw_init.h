#ifndef __ALBERTO_TONTONI_HW_INIT__
#define __ALBERTO_TONTONI_HW_INIT__

#include <MKL25Z4.h>

void ADC_Init(ADC_Type *adc, uint8_t channel, PORT_Type *port, uint32_t pin);

#endif
