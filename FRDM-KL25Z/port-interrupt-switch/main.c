#include <MKL25Z4.h>

// External switch on PTA1
const static uint32_t PIN_SW1 = 1;

// External switch on PTA2
const static uint32_t PIN_SW2 = 2;

// Built-in red LED on PTB18
const static uint32_t PIN_LED_RED = 18;

// Built-in green LED on PTB19
const static uint32_t PIN_LED_GREEN = 19;

static uint8_t swPressed1 = 0;

volatile static uint8_t blinkGreen = 0;

void delayMs(uint32_t ms);

int main() {
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK;
	
	// Enable PTA1 interrupt on both edges.
	// Enable PTA2 interrupt on falling edge.
	// Configure PTA1, PTA2 pullup resistor.
	__disable_irq();
	
	PORTA->PCR[PIN_SW1] &= ~PORT_PCR_MUX_MASK & 
		~PORT_PCR_PE_MASK &
		~PORT_PCR_PS_MASK &
		~PORT_PCR_IRQC_MASK;
	PORTA->PCR[PIN_SW1] |= PORT_PCR_MUX(1) | // Mode = GPIO
		PORT_PCR_PE(1) | // Pull resistor enabled
		PORT_PCR_PS(1) | // Choose pullup resistor
		PORT_PCR_IRQC(0xB); // Interrupt on both edges
	
	PORTA->PCR[PIN_SW2] &= ~PORT_PCR_MUX_MASK & 
		~PORT_PCR_PE_MASK &
		~PORT_PCR_PS_MASK &
		~PORT_PCR_IRQC_MASK;
	PORTA->PCR[PIN_SW2] |= PORT_PCR_MUX(1) | // Mode = GPIO
		PORT_PCR_PE(1) | // Pull resistor enabled
		PORT_PCR_PS(1) | // Choose pullup resistor
		PORT_PCR_IRQC(0xA); // Interrupt on falling edge
	
	GPIOA->PDDR &= ~(uint32_t)(1 << PIN_SW1);
	GPIOA->PDDR &= ~(uint32_t)(1 << PIN_SW2);
	PORTB->PCR[PIN_LED_RED] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PIN_LED_RED] |= PORT_PCR_MUX(1);
	PORTB->PCR[PIN_LED_GREEN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PIN_LED_GREEN] |= PORT_PCR_MUX(1);
	GPIOB->PDDR |= (1 << PIN_LED_RED) | (1 << PIN_LED_GREEN);
	GPIOB->PSOR = (1 << PIN_LED_RED) | (1 << PIN_LED_GREEN);
	
	NVIC_EnableIRQ(PORTA_IRQn);
	__enable_irq();
	
	while(1) {
		if(blinkGreen) {
			GPIOB->PCOR = (1 << PIN_LED_GREEN);
			delayMs(1000);
			GPIOB->PSOR = (1 << PIN_LED_GREEN);
			delayMs(1000);
			GPIOB->PCOR = (1 << PIN_LED_GREEN);
			delayMs(1000);
			GPIOB->PSOR = (1 << PIN_LED_GREEN);
			delayMs(1000);
			blinkGreen = 0;
		}
		else {
			GPIOB->PSOR = (1 << PIN_LED_GREEN);
		}
	}
}

void delayMs(uint32_t ms) {
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
	
	// Timeout after 1ms.
	// Assuming clock at 20.97152MHz.
	SysTick->LOAD = 20971;
	
	for(uint32_t i=0; i<ms; i++) {
		while( !(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) );
	}
	
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void PORTA_IRQHandler(void) {
	static uint8_t pressedNow;
	
	// Detect whether PTA1 or PTA2 generated the interrupt
	if(PORTA->ISFR & (1 << PIN_SW1)) {
		pressedNow = !(GPIOA->PDIR & (1 << PIN_SW1));
		
		// Software debouncing for the external switch.
		// LED is toggled when the switch gets pressed.
		if((!swPressed1) &&  pressedNow) {
			swPressed1 = 1;
			GPIOB->PTOR = (1 << PIN_LED_RED);
		}
		else if(swPressed1 && (!pressedNow)) {
			swPressed1 = 0;
		}
		
		// Clear the interrupt
		PORTA->ISFR = (1 << PIN_SW1);
	}
	
	if(PORTA->ISFR & (1 << PIN_SW2)) {
		
		blinkGreen = 1;
		
		// Clear the interrupt
		PORTA->ISFR = (1 << PIN_SW2);
	}
}
