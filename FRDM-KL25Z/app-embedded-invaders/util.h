#ifndef __ALBERTO_TONTONI_UTIL__
#define __ALBERTO_TONTONI_UTIL__

#include "SSD1306/SSD1306.h"

/// \param [in] timeoutMs The event frequency.
/// \param [in] lastTimeoutMs The last time that the event triggered, expressed in system time.
struct PeriodicEvent {
	uint32_t timeoutMs;
	uint32_t lastTimeoutMs;
};
typedef struct PeriodicEvent PeriodicEvent;

/// \brief Write positive integer on the buffer, 
/// 	given the (x,y) coordinate of the least important digit.
/// \param [in] value Value to be displayed.
/// \param [in] x Rightmost X coordinate of the number bitmap.
/// \param [in] y Upper Y coordinate of the number bitmap.
void SSD1306_WriteUnsignedInt(
	SSD1306_Data *data, 
	int32_t value, 
	uint8_t x, 
	uint8_t y);

#endif
