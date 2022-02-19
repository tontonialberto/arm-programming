#include "TimerImpl.h"

extern volatile uint32_t elapsedMs;

uint32_t TimerImpl_GetElapsedMs(void) {
	return elapsedMs;
}

uint32_t TimerImpl_GetElapsedMsSince(uint32_t ms) {
	return elapsedMs - ms;
}
