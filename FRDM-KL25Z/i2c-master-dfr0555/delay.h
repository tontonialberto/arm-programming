#ifndef _MY_DELAY_H
#define _MY_DELAY_H

#include <MKL25Z4.h>

// Use SysTick timer to generate delays
void delayMs(uint32_t ms);

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

#endif
