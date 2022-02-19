#ifndef __ALBERTO_TONTONI_TIMERIMPL__
#define __ALBERTO_TONTONI_TIMERIMPL__

#include <stdint.h>

uint32_t TimerImpl_GetElapsedMs(void);

uint32_t TimerImpl_GetElapsedMsSince(uint32_t ms);

#endif
