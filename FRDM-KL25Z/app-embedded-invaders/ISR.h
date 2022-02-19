#ifndef __ALBERTO_TONTONI_ISR__
#define __ALBERTO_TONTONI_ISR__

#include <stdint.h>
#include <stdbool.h>

extern volatile int32_t analogY;
extern volatile bool spawnPlayerBullet;
extern volatile uint32_t elapsedMs;

void ADC0_IRQHandler(void);

void PORTD_IRQHandler(void);

void TPM0_IRQHandler(void);

#endif
