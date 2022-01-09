#include <MKL25Z4.h>
#include <stdlib.h>

// Clock at 4MHz (CLOCK_SETUP == 2), MCGIRCLK = 4MHz

// Built-in blue LED on PTD1
const static uint32_t PIN_LED_BLUE = 1;

// Built-in red LED on PTB18
const static uint32_t PIN_LED_RED = 18;

// MOD value for 1 second timeouts with the current clock settings
const static uint16_t TPM_MOD_1_SECOND = 31249;

typedef struct {
	uint8_t tpm_mode; // 0x1 = Output Compare
	uint8_t output_action; // 0x1 = Toggle
	uint16_t CnV;
} TPM_ChannelOptions;

/**
Helper function to configure a TPM peripheral.
Warning: it has been tested only for Output Compare mode.
	
Parameters:
-PS: prescaler bits [0-7]. Clock divider = 2^(PS)
-opts is an array of 6 pointers, one for each TPMx channel.
	If i-th element is NULL, it means that i-th channel won't be configured.
*/
void TPM_Init(TPM_Type *TPM, uint8_t PS, uint16_t MOD, const TPM_ChannelOptions **opts);

const static TPM_ChannelOptions TPM0_CH1_opts = {
	.tpm_mode = 0x1,
	.output_action = 0x1,
	.CnV = TPM_MOD_1_SECOND / 2
};

const static TPM_ChannelOptions *TPM0_opts[] = {
	NULL,
	&TPM0_CH1_opts,
	NULL,
	NULL,
	NULL,
	NULL
};

const static TPM_ChannelOptions TPM2_CH0_opts = {
	.tpm_mode = 0x1,
	.output_action = 0x1,
	.CnV = TPM_MOD_1_SECOND / 4
};

const static TPM_ChannelOptions *TPM2_opts[] = {
	&TPM2_CH0_opts,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

int main() {
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK | SIM_SCGC6_TPM2_MASK;
	
	// SET MCGIRCLK at TPM clock source
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(3);
	
	TPM_Init(TPM0, 7, TPM_MOD_1_SECOND, TPM0_opts);
	
	TPM_Init(TPM2, 7, TPM_MOD_1_SECOND, TPM2_opts);
	
	// Setup built-in blue LED for FTM0_CH1 alternate function
	PORTD->PCR[PIN_LED_BLUE] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[PIN_LED_BLUE] |= PORT_PCR_MUX(4);
	GPIOD->PCOR = (1 << PIN_LED_BLUE);
	
	// Setup built-in red LED form FTM2_CH0 alternate function
	PORTB->PCR[PIN_LED_RED] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PIN_LED_RED] |= PORT_PCR_MUX(3);
	GPIOB->PCOR = (1 << PIN_LED_RED);
	
	// Enable counter on TPM0
	TPM0->SC |= TPM_SC_CMOD(1);
	
	// Enable counter on TPM2
	TPM2->SC |= TPM_SC_CMOD(1);
	
	// A single loop depicts what happens in 1 second of execution.
	// What you eventually visualize on your board is the following:
	// 1) For t=[0 ... 250ms]: No LED is active
	// 2) For t=[250ms ... 500ms]: Violet LED (eg. both red and blue)
	// 3) For t=[500ms ... 750ms]: Blue LED
	// 4) For t=[750ms ... 1s]: Red LED
	while(1) {
		while(! (TPM2->CONTROLS[0].CnSC & TPM_CnSC_CHF_MASK) );
		TPM2->CONTROLS[0].CnSC |= TPM_CnSC_CHF_MASK;
		
		TPM2->CONTROLS[0].CnV = 2 * (TPM_MOD_1_SECOND / 4);
		
		// --------
		
		while(! (TPM2->CONTROLS[0].CnSC & TPM_CnSC_CHF_MASK) );
		TPM2->CONTROLS[0].CnSC |= TPM_CnSC_CHF_MASK;
		
		TPM2->CONTROLS[0].CnV = 3 * (TPM_MOD_1_SECOND / 4);
		
		// Once the TPMx counter has reached CnV of channel_1,
		// 500ms have passed and the LED toggles.
		// In order to toggle again after 500ms (ie. 1 second passed in total),
		// you can either reset TPM counter or change the value of CnV.
		// The latter is more flexible, since it doesn't interfere with 
		// the other TPMx channels.
		TPM0->CONTROLS[1].CnV = TPM_MOD_1_SECOND;
		
		// --------
		
		while(! (TPM2->CONTROLS[0].CnSC & TPM_CnSC_CHF_MASK) );
		TPM2->CONTROLS[0].CnSC |= TPM_CnSC_CHF_MASK;
		
		TPM2->CONTROLS[0].CnV = TPM_MOD_1_SECOND;
		
		// --------
		
		while(! (TPM2->CONTROLS[0].CnSC & TPM_CnSC_CHF_MASK) );
		TPM2->CONTROLS[0].CnSC |= TPM_CnSC_CHF_MASK;
		
		TPM2->CONTROLS[0].CnV = TPM_MOD_1_SECOND / 4;
		
		TPM0->CONTROLS[1].CnV = TPM_MOD_1_SECOND / 2;
	}
}

void TPM_Init(TPM_Type *TPM, uint8_t PS, uint16_t MOD, const TPM_ChannelOptions **opts) {
	int i = 0;
	
	TPM->SC &= ~TPM_SC_CMOD_MASK;
	
	TPM->SC |= TPM_SC_PS(PS) | TPM_SC_TOF_MASK;
	
	TPM->MOD = MOD;
	
	// Set up every TPM channel
	for(i=0; i<6; i++) {
		if(!opts[i]) continue;
		
		TPM->CONTROLS[i].CnSC = 
			TPM_CnSC_CHF_MASK 
			| (uint32_t)(opts[i]->tpm_mode << TPM_CnSC_MSA_SHIFT) 
			| (uint32_t)(opts[i]->output_action << TPM_CnSC_ELSA_SHIFT);
		
		TPM->CONTROLS[i].CnV = opts[i]->CnV;
	}
}
