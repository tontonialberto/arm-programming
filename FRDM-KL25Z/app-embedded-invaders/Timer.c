#include "Timer.h"

static uint32_t TimerStub_GetElapsedMs(void);

static uint32_t TimerStub_GetElapsedMsSince(uint32_t);

uint32_t (* Timer_GetElapsedMs)(void) = TimerStub_GetElapsedMs;

uint32_t (* Timer_GetElapsedMsSince)(uint32_t) = TimerStub_GetElapsedMsSince;

uint32_t TimerStub_GetElapsedMs(void) {
	return 0;
}

uint32_t TimerStub_GetElapsedMsSince(uint32_t ms) {
	(void)ms;
	return 0;
}
