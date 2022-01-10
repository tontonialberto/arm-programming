#include <MKL25Z4.h>

// System clock = 48MHz (ie. CLOCK_SETUP == 1)

// External switch on PTE29
const static uint32_t PIN_SWITCH = 29;

int main() {
	SIM->SCGC6 |= SIM_SCGC6_TPM2_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	
	// Set PTA1 to FTM_CLKIN0 mode, pullup enable
	PORTE->PCR[PIN_SWITCH] &= ~PORT_PCR_MUX_MASK & ~PORT_PCR_PE_MASK & ~PORT_PCR_PS_MASK;
	PORTE->PCR[PIN_SWITCH] |= PORT_PCR_MUX(4) | PORT_PCR_PE_MASK | PORT_PCR_PS(1);
	
	// Set PLL (48MHz) as TPMx clock source
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1) | SIM_SOPT2_PLLFLLSEL_MASK;
	
	// Set TPM2 using FTM_CLKIN0
	SIM->SOPT4 &= ~SIM_SOPT4_TPM2CLKSEL_MASK;
	
	TPM2->SC &= ~TPM_SC_CMOD_MASK;
	
	// Clock divider = 1 gives a sampling frequency of 8MHz
	TPM2->SC = TPM_SC_TOF_MASK | TPM_SC_PS(0);
	
	// Set TPM2 overflow value
	TPM2->MOD = 0xffff;
	
	// Enable TPM2
	TPM2->SC |= TPM_SC_CMOD(2);
	
	uint16_t count = 0;
	
	while(1) {
		count = (uint16_t)TPM2->CNT;
		
		__ASM("nop");
	}
}
