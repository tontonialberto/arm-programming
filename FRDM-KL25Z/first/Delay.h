#ifndef _DELAY_H
#define _DELAY_H

#include <MKL25Z4.h>

void Delay(uint32_t ms);

extern volatile uint32_t iters;

void Delay(uint32_t ms) {
	iters = SystemCoreClock / 1000;
	iters *= ms;
	uint32_t i = 0;
	
	for(i=0; i<SystemCoreClock*3/1000; i++);
}

#endif
