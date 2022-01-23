#include <MKL25Z4.h>

// Core clock at 48MHz, using PLL

// Built-in green LED on PTB19
const static uint32_t PIN_LED_GREEN = 19;

int main() {
	SIM->SCGC6 |= SIM_SCGC6_TPM2_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	
	// Use OSCERCLK (8MHz) as TPM clock
	SIM->SOPT2 = SIM_SOPT2_PLLFLLSEL(1) | SIM_SOPT2_TPMSRC(2);
	
	// Configure PTB19 as alternate function TPM2_CH0
	PORTB->PCR[PIN_LED_GREEN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PIN_LED_GREEN] |= PORT_PCR_MUX(3);
	
	TPM2->SC &= ~TPM_SC_CMOD_MASK;
	TPM2->SC |= TPM_SC_TOF_MASK;
	TPM2->SC |= TPM_SC_PS(7); // Prescaler = 128
	TPM2->SC &= ~TPM_SC_CPWMS_MASK; // PWM up counting mode
	
	// PWM edge-aligned mode, Low-true pulses
	TPM2->CONTROLS[1].CnSC = 
		TPM_CnSC_CHF_MASK | 
		(0x2 << TPM_CnSC_MSA_SHIFT) | 
		(0x1 << TPM_CnSC_ELSA_SHIFT);
	
	// Set PWM period to 10 ms
	TPM2->MOD = 625 - 1;
	
	// Set duty cycle to the minimum
	TPM2->CONTROLS[1].CnV = 1;
	
	// Enable timer
	TPM2->SC |= TPM_SC_CMOD(1);
	
	while(1) {
	}
}
