#include <MKL25Z4.h>

// Clock is running at 48MHz using PLL (ie. CLOCK_SETUP == 1).

// Built-in green LED on PTB19
const static uint32_t PIN_LED_GREEN = 19;

void delayMs(uint32_t ms);

int main() {
	// Enable clock on TPM0 and PORTB
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	
	// Set MCGPLLCLK/2 as clock for all TPM modules
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK & ~SIM_SOPT2_PLLFLLSEL_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1) | SIM_SOPT2_PLLFLLSEL_MASK;
	
	PORTB->PCR[PIN_LED_GREEN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PIN_LED_GREEN] |= PORT_PCR_MUX(1);
	GPIOB->PDDR |= (1 << PIN_LED_GREEN);
	GPIOB->PSOR = (1 << PIN_LED_GREEN);
	
	while(1) {
		delayMs(1000);
		
		GPIOB->PTOR = (1 << PIN_LED_GREEN);
		
		delayMs(1000);
		
		GPIOB->PTOR = (1 << PIN_LED_GREEN);
	}
}

void delayMs(uint32_t ms) {
	// Disable TPM0 before doing configuration 
	TPM0->SC &= ~TPM_SC_CMOD_MASK;
	
	// Reset TOF flag
	TPM0->SC |= TPM_SC_TOF_MASK;
	
	// Set prescaler = 128 = 2^7
	TPM0->SC |= TPM_SC_PS(7);
	
	// Set timeout to 1ms
	TPM0->MOD = 374;
	
	// Enable TPM0 as Timer
	TPM0->SC |= TPM_SC_CMOD(1);
	
	for(uint32_t i=0; i<ms; i++) {
		while( !(TPM0->SC & TPM_SC_TOF_MASK) );
		TPM0->SC |= TPM_SC_TOF_MASK;
	}
	
	// Disable TPM0
	TPM0->SC &= ~TPM_SC_CMOD_MASK;
}
