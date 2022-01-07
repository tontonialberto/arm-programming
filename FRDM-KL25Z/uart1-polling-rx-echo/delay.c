#include "delay.h"
#include <MKL25Z4.h>

void delayMs(uint32_t ms) {
	uint32_t i;
	uint32_t j;

	for(i=0; i<ms; i++) {
		for(j=0; j<7000; j++) {
			__ASM("nop");
		}
	}
}
