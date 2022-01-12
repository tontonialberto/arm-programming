#include <MKL25Z4.h>

// System clock configured as using PLL, 48MHz
// (ie. CLOCK_SETUP == 1)

// Built-in blue LED on PTD1
const static uint32_t PIN_LED_BLUE = 1;

// Built-in green LED on PTB19
const static uint32_t PIN_LED_GREEN = 19;

int main() {
	__disable_irq();
	
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
	
	// Set OSCERCLK (8MHz) as TPM clock source
	SIM->SOPT2 &= ~SIM_SOPT2_PLLFLLSEL_MASK & ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL(1) | SIM_SOPT2_TPMSRC(2);
	
	TPM0->SC &= ~TPM_SC_CMOD_MASK;
	TPM0->SC |= TPM_SC_TOF_MASK;
	
	// TPM0 clock divider = 128
	TPM0->SC |= TPM_SC_PS(7);

	// Set TPM0 overflow after 1 second
	TPM0->MOD = 62500 - 1;
	
	// Enable TPM0, enable overflow interrupts
	TPM0->SC |= TPM_SC_CMOD(1) | TPM_SC_TOIE_MASK;
	
	PORTD->PCR[PIN_LED_BLUE] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[PIN_LED_BLUE] |= PORT_PCR_MUX(1);
	GPIOD->PDDR |= (1 << PIN_LED_BLUE);
	GPIOD->PSOR = (1 << PIN_LED_BLUE);
	
	NVIC_EnableIRQ(TPM0_IRQn);
	__enable_irq();
	
	while(1) {}
}

void TPM0_IRQHandler(void) {
	// Clear interrupt
	TPM0->SC |= TPM_SC_TOF_MASK;
	
	GPIOD->PTOR = (1 << PIN_LED_BLUE);
}
