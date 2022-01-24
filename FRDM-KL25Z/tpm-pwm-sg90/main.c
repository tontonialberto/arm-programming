#include <MKL25Z4.h>
#include "delay.h"

// Core clock at 48MHz, using PLL

// SG90 servomotor on PTE22
const static uint32_t PIN_SERVO = 22;

const static uint32_t PWM_DIVIDER_LOG = 4;
const static uint32_t PWM_DIVIDER = 1 << PWM_DIVIDER_LOG;
const static uint32_t PWM_FREQ_HZ = 50;

// deg: degrees in range [0, 180]
uint16_t degreesToCnV(uint8_t deg);

typedef struct {
	double minDutyMs;
	double maxDutyMs;
	TPM_Type *tpm;
	uint32_t pwmFreqHz;
	uint8_t channel;
} Servo;

void Servo_Write(Servo *s, uint8_t degrees) {
	double dutyMs = s->minDutyMs + (s->maxDutyMs * (double)degrees / 180);
	double dutyRatio = s->pwmFreqHz * (double)dutyMs / 1000;
	uint16_t result = (uint16_t)(dutyRatio * (TPM2->MOD + 1));
	s->tpm->CONTROLS[s->channel].CnV = result;
}

int main() {
	SystemInit();
	
	uint8_t degrees = 0;
	
	SIM->SCGC6 |= SIM_SCGC6_TPM2_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	
	// System clock (48MHz) as TPM clock
	SIM->SOPT2 = SIM_SOPT2_PLLFLLSEL(1) | SIM_SOPT2_TPMSRC(1);
	
	// Configure PTE22 as alternate function TPM2_CH0
	PORTE->PCR[PIN_SERVO] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[PIN_SERVO] |= PORT_PCR_MUX(3);
	
	TPM2->SC &= ~TPM_SC_CMOD_MASK;
	TPM2->SC |= TPM_SC_TOF_MASK;
	TPM2->SC |= TPM_SC_PS(PWM_DIVIDER_LOG);
	TPM2->SC &= ~TPM_SC_CPWMS_MASK; // PWM up counting mode
	
	// PWM edge-aligned mode, High-true pulses
	TPM2->CONTROLS[0].CnSC = 
		TPM_CnSC_CHF_MASK | 
		(0x2 << TPM_CnSC_MSA_SHIFT) | 
		(0x2 << TPM_CnSC_ELSA_SHIFT);
	
	// Set PWM frequency
	TPM2->MOD = (SystemCoreClock / (PWM_DIVIDER * PWM_FREQ_HZ)) - 1;
	
	Servo servo;
	servo.channel = 0;
	servo.tpm = TPM2;
	servo.minDutyMs = 0.4;
	servo.maxDutyMs = 1.8;
	servo.pwmFreqHz = PWM_FREQ_HZ;
	
	Servo_Write(&servo, 0);
	
	// Enable timer
	TPM2->SC |= TPM_SC_CMOD(1);
	
	while(1) {
		delayMs(1000);
		degrees += 45;
		if(degrees > 180) degrees = 0;
		Servo_Write(&servo, degrees);
	}
}
