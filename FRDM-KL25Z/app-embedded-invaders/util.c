#include "util.h"

void SSD1306_WriteUnsignedInt(
		SSD1306_Data *data, 
		int32_t value, 
		uint8_t x, 
		uint8_t y) {
	
	if(value == 0) {
			SSD1306_WriteDigit(data, 0, x, y);
	}
	else {
		if((value / 10) > 0) {
			SSD1306_WriteUnsignedInt(data, value / 10, x - 4, y);
		}
		SSD1306_WriteDigit(data, (uint8_t)(value % 10), x, y);
	}
}
		
int32_t lowPassFilter(int32_t prevFiltered, int32_t currMeasurement, double alpha) {
	return currMeasurement + (int32_t)(alpha * (prevFiltered - currMeasurement));
}

