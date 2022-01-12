#include <MKL25Z4.h>

// System clock at 48MHz (ie. CLOCK_SETUP == 1)

// Built-in green LED on PTB19
const static uint32_t PIN_LED_GREEN = 19;

extern void SysTick_Handler(void);

int main() {
	__disable_irq();
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	PORTB->PCR[PIN_LED_GREEN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PIN_LED_GREEN] |= PORT_PCR_MUX(1);
	GPIOB->PDDR |= (1 << PIN_LED_GREEN);
	GPIOB->PSOR = (1 << PIN_LED_GREEN);
	
	// SysTick clock source = 3MHz = SysClock / 16
	SysTick->CTRL &= ~SysTick_CTRL_CLKSOURCE_Msk;

	// Enable SysTick interrupt
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	
	// SysTick underflow after 1000 ms
	SysTick->LOAD = 3000000 - 1;
	
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	__enable_irq();
	while(1) {
	}
}

void SysTick_Handler(void) {
		GPIOB->PTOR = (1 << PIN_LED_GREEN);
}
