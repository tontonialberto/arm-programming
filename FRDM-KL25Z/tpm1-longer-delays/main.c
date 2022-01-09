#include <MKL25Z4.h>

// Clock is running at 48MHz using PLL (ie. CLOCK_SETUP == 1).

// Built-in green LED on PTB19
const static uint32_t PIN_LED_GREEN = 19;

// Delay 1,048576 seconds using TPM1, OSCERCLK
void delay_1s(void);

int main() {
	// Enable clock on TPM0 and PORTB
	SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	
	// Set OSCERCLK (ie. 8MHz) as clock for all TPM modules
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK & ~SIM_SOPT2_PLLFLLSEL_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(2) | SIM_SOPT2_PLLFLLSEL_MASK;
	
	PORTB->PCR[PIN_LED_GREEN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PIN_LED_GREEN] |= PORT_PCR_MUX(1);
	GPIOB->PDDR |= (1 << PIN_LED_GREEN);
	GPIOB->PSOR = (1 << PIN_LED_GREEN);
	
	while(1) {
		delay_1s();
		
		GPIOB->PTOR = (1 << PIN_LED_GREEN);
		
		delay_1s();
		
		GPIOB->PTOR = (1 << PIN_LED_GREEN);
	}
}

void delay_1s() {
	// Disable TPM before doing configuration 
	TPM1->SC &= ~TPM_SC_CMOD_MASK;
	
	// Reset TOF flag
	TPM1->SC |= TPM_SC_TOF_MASK;
	
	// Set prescaler = 128 = 2^7
	TPM1->SC |= TPM_SC_PS(7);
	
	TPM1->MOD = 0xffff;
	
	// Enable TPM as Timer
	TPM1->SC |= TPM_SC_CMOD(1);
	
	while( !(TPM1->SC & TPM_SC_TOF_MASK) );
	TPM1->SC |= TPM_SC_TOF_MASK;
	
	// Disable TPM
	TPM1->SC &= ~TPM_SC_CMOD_MASK;
}
