#include <MKL25Z4.h>

// Built-in red LED on PTB18
const static uint32_t PIN_LED_RED = 18;

void delayMs(uint32_t ms);

void delayUs(uint32_t us);

int main() {
	SystemInit();
	SystemCoreClockUpdate();
	
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	PORTB->PCR[PIN_LED_RED] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PIN_LED_RED] |= PORT_PCR_MUX(1);
	GPIOB->PDDR |= (1 << PIN_LED_RED);
	GPIOB->PSOR = (1 << PIN_LED_RED);
	
	// Set SysTick clock = System clock
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk;
	
	while(1) {
		
		delayMs(1000);
		
		// Turn on red LED
		GPIOB->PCOR = (1 << PIN_LED_RED);
		
		delayUs(1000000);
		
		// turn off red LED
		GPIOB->PSOR = (1 << PIN_LED_RED);
	}
}

void delayMs(uint32_t ms) {
	
	// Set the down counter to 1ms
	SysTick->LOAD = (SystemCoreClock / 1000) - 1;
	
	// Enable SysTick down counting.
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	
	for(uint32_t i=0; i<ms; i++) {
		// Wait until the timer underflows.
		// Once the underflow is reached, the timer
		// will be automatically reloaded with the value contained inside SysTick->LOAD.
		while( !(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) );
	}
	
	// Disable SysTick counter.
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void delayUs(uint32_t us) {
	
	// Set the down counter to 1 microsecond
	// Note that, for some clock freq. settings, 
	// this may be not accurate.
	SysTick->LOAD = (SystemCoreClock / 1000000) - 1;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	
	for(uint32_t i=0; i<us; i++)
		while( !(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) );
	
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}
