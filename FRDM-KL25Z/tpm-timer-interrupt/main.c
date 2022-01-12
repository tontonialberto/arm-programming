#include <MKL25Z4.h>

// System clock configured as using PLL, 48MHz
// (ie. CLOCK_SETUP == 1)

// Built-in blue LED on PTD1
const static uint32_t PIN_LED_BLUE = 1;

// Built-in red LED on PTB18
const static uint32_t PIN_LED_RED = 18;

const static uint16_t TPM_OVF_AFTER_1_SECOND = 62500 - 1;

// Configure TPMx with prescaler = 128,
// and the given overflow value for the MOD register.
// Enable interrupt on overflow.
// Note that you have to enable TPMx manually
// after executing this function.
void TPM_Init(TPM_Type *TPM, uint16_t overflow);

int main() {
	__disable_irq();
	
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK | SIM_SCGC6_TPM1_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;
	
	// Set OSCERCLK (8MHz) as TPM clock source
	SIM->SOPT2 &= ~SIM_SOPT2_PLLFLLSEL_MASK & ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL(1) | SIM_SOPT2_TPMSRC(2);

	// Set TPM0 overflow after 1 second
	TPM_Init(TPM0, TPM_OVF_AFTER_1_SECOND);
	
	// Set TPM1 overflow after 500 ms
	TPM_Init(TPM1, TPM_OVF_AFTER_1_SECOND / 2);
	
	// Enable TPM0,1
	TPM0->SC |= TPM_SC_CMOD(1);
	TPM1->SC |= TPM_SC_CMOD(1);
	
	PORTD->PCR[PIN_LED_BLUE] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[PIN_LED_BLUE] |= PORT_PCR_MUX(1);
	GPIOD->PDDR |= (1 << PIN_LED_BLUE);
	GPIOD->PSOR = (1 << PIN_LED_BLUE);
	
	PORTB->PCR[PIN_LED_RED] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PIN_LED_RED] |= PORT_PCR_MUX(1);
	GPIOB->PDDR |= (1 << PIN_LED_RED);
	GPIOB->PSOR = (1 << PIN_LED_RED);
	
	NVIC_EnableIRQ(TPM0_IRQn);
	NVIC_EnableIRQ(TPM1_IRQn);
	__enable_irq();
	
	while(1) {}
}

void TPM0_IRQHandler(void) {
	// Clear interrupt
	TPM0->SC |= TPM_SC_TOF_MASK;
	
	GPIOD->PTOR = (1 << PIN_LED_BLUE);
}

void TPM1_IRQHandler(void) {
	TPM1->SC |= TPM_SC_TOF_MASK;
	
	GPIOB->PTOR = (1 << PIN_LED_RED);
}

void TPM_Init(TPM_Type *TPM, uint16_t overflow) {
	TPM->SC &= ~TPM_SC_CMOD_MASK;
	TPM->SC |= TPM_SC_TOF_MASK;
	
	// TPM clock divider = 128
	TPM->SC |= TPM_SC_PS(7);

	// Set TPM overflow 
	TPM->MOD = overflow;
	
	// Enable overflow interrupts
	TPM->SC |= TPM_SC_TOIE_MASK;
}
