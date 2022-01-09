#include <MKL25Z4.h>
#include <etl/vector.h>
#include <etl/numeric.h>

int main() {
	
	etl::vector<uint8_t, 10> v(10);
	etl::iota(v.begin(), v.end(), 0);
	
	for(uint32_t i=0; i<10; i++) {
		v[i] = (i % 2) ? 1 : 0;
	}
	
	// Configure built-in red LED in GPIO output mode
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	PORTB->PCR[18] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[18] |= PORT_PCR_MUX(1);
	GPIOB->PDDR |= (1 << 18);
	
	// Enable SysTick, 1ms resolution
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
	SysTick->LOAD = 48000 - 1;
	
	while(1) {
			
		for(const auto &n: v) {
			// Delay 1s
			for(int i=0; i<1000; i++)
				while( !(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) );
			
			if(n == 1)
				GPIOB->PCOR = (uint32_t)(1 << 18);
			else
				GPIOB->PSOR = (uint32_t)(1 << 18);
		}
	}
}
