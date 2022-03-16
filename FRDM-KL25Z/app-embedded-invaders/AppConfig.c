#include "AppConfig.h"

const int32_t THRESHOLD_JOYSTICK = 50;

const int16_t SCREEN_MIN_X = 0;

const int16_t SCREEN_MAX_X = 127;

const int16_t SCREEN_MIN_Y = 0;

const uint16_t SCREEN_WIDTH = (uint16_t)(SCREEN_MAX_X - SCREEN_MIN_X);

const uint16_t SCREEN_HEIGHT = (uint16_t)(32 - SCREEN_MIN_Y); // TODO: extract constant
