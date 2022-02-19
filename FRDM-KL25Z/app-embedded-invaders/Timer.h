#ifndef __ALBERTO_TONTONI_TIMER__
#define __ALBERTO_TONTONI_TIMER__

#include <stdint.h>

extern uint32_t (* Timer_GetElapsedMs)(void);

extern uint32_t (* Timer_GetElapsedMsSince)(uint32_t);

#endif
