#ifndef __ALBERTO_TONTONI_ISR__
#define __ALBERTO_TONTONI_ISR__

#include <stdint.h>
#include <stdbool.h>

extern volatile int32_t analogY;
extern volatile bool spawnPlayerBullet;

void ADC0_IRQHandler(void);

void PORTD_IRQHandler(void);

#endif
