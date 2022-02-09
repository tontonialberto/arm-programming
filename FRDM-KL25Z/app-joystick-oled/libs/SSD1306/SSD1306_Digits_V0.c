#include "SSD1306_Digits_V0.h"

const uint8_t SSD1306_DIGITS_V0_ROWS = 5;
const uint8_t SSD1306_DIGITS_V0_COLS = 3;
const uint8_t SSD1306_DIGITS_V0_PIXELS = SSD1306_DIGITS_V0_ROWS * SSD1306_DIGITS_V0_COLS;

const static uint8_t DIGIT_V0_0[] = { 
    1, 1, 1,
    1, 0, 1,
    1, 0, 1,
    1, 0, 1,
    1, 1, 1
};

const static uint8_t DIGIT_V0_1[] = {
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1
};

const static uint8_t DIGIT_V0_2[] = {
    1, 1, 1,
    0, 0, 1,
    1, 1, 1,
    1, 0, 0,
    1, 1, 1
};

const static uint8_t DIGIT_V0_3[] = {
    1, 1, 1,
    0, 0, 1,
    1, 1, 1,
    0, 0, 1,
    1, 1, 1
};

const static uint8_t DIGIT_V0_4[] = {
    1, 0, 1,
    1, 0, 1,
    1, 1, 1,
    0, 0, 1,
    0, 0, 1
};

const static uint8_t DIGIT_V0_5[] = {
    1, 1, 1,
    1, 0, 0,
    1, 1, 1,
    0, 0, 1,
    1, 1, 1
};

const static uint8_t DIGIT_V0_6[] = {
    1, 1, 1,
    1, 0, 0,
    1, 1, 1,
    1, 0, 1,
    1, 1, 1
};

const static uint8_t DIGIT_V0_7[] = {
    1, 1, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1
};

const static uint8_t DIGIT_V0_8[] = {
    1, 1, 1,
    1, 0, 1,
    1, 1, 1,
    1, 0, 1,
    1, 1, 1
};

const static uint8_t DIGIT_V0_9[] = {
    1, 1, 1,
    1, 0, 1,
    1, 1, 1,
    0, 0, 1,
    1, 1, 1
};

const uint8_t *SSD1306_DIGITS_V0[] = {
    DIGIT_V0_0,
    DIGIT_V0_1,
    DIGIT_V0_2,
    DIGIT_V0_3,
    DIGIT_V0_4,
    DIGIT_V0_5,
    DIGIT_V0_6,
    DIGIT_V0_7,
    DIGIT_V0_8,
    DIGIT_V0_9
};
