#include <MKL25Z4.h>

// System clock = 48MHz (ie. CLOCK_SETUP == 1)

// External switch on PTA1
const static uint32_t PIN_SWITCH = 1;

volatile static uint16_t now = 0, then = 0, diff = 0;

int main() {
	SIM->SCGC6 |= SIM_SCGC6_TPM2_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
	
	// Set PTA1 to FTM2_CH0 mode, pullup enable
	PORTA->PCR[PIN_SWITCH] &= ~PORT_PCR_MUX_MASK & ~PORT_PCR_PE_MASK & ~PORT_PCR_PS_MASK;
	PORTA->PCR[PIN_SWITCH] |= PORT_PCR_MUX(3) | PORT_PCR_PE(1) | PORT_PCR_PS(1);
	
	// Set OSCERCLK (8MHz) as TPMx clock source
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(2);
	
	TPM2->SC &= ~TPM_SC_CMOD_MASK;
	
	// Clock divider = 128
	TPM2->SC |= TPM_SC_TOF_MASK | TPM_SC_PS(7);
	
	// Set TPM2 counter overflow after approx 1,04 seconds
	TPM2->MOD = 0xffff;
	
	// Set TPM2_CH0 as Input Capture on Falling Edge
	TPM2->CONTROLS[0].CnSC = TPM_CnSC_CHF_MASK | (0x2 << TPM_CnSC_ELSA_SHIFT);
	
	// Enable TPM2
	TPM2->SC |= TPM_SC_CMOD(1);
	
	while(1) {
		while(! (TPM2->CONTROLS[0].CnSC & TPM_CnSC_CHF_MASK) );
		TPM2->CONTROLS[0].CnSC |= TPM_CnSC_CHF_MASK;
		
		now = (uint16_t)TPM2->CONTROLS[0].CnV;
		diff = now - then;
		
		then = now;
	}
}
