#ifndef __ALBERTO_TONTONI_DELAY_H__
#define __ALBERTO_TONTONI_DELAY_H__

#include <MKL25Z4.h>

// Use SysTick timer to generate delays
void delayMs(uint32_t ms);

void delayUs(uint32_t us);

static void SysTick_Enable() {
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

static void SysTick_WaitTimeout() {
	while(! (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) );
}

static void SysTick_Disable() {
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void delayMs(uint32_t ms) {
	SysTick_Enable();
	
	// Timeout after 1 ms
	SysTick->LOAD = (SystemCoreClock / 1000) - 1;
	
	for(uint32_t i=0; i<ms; i++) {
		SysTick_WaitTimeout();
	}
	
	SysTick_Disable();
}

void delayUs(uint32_t us) {
	SysTick_Enable();
	
	// Timeout after 1 microsecond
	SysTick->LOAD = (SystemCoreClock / 1000000) - 1;
	
	for(uint32_t i=0; i<us; i++) {
		SysTick_WaitTimeout();
	}
	
	SysTick_Disable();
}

#endif
