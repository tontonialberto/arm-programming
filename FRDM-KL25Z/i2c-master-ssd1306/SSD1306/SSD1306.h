#ifdef __cplusplus
extern "C" {
#endif

#ifndef __ALBERTO_TONTONI_SSD1306__
#define __ALBERTO_TONTONI_SSD1306__

#include <stdint.h>
#include <stdbool.h>
#include "SSD1306_I2C_Interface.h"

/// \file SSD1306.h
/// \brief Minimalist I2C driver for SSD1306 128x32 OLED display.

// This library uses a private byte array to manipulate I2C operation in an easier way.
// You can override the default buffer size by providing the following macro,
// either in your code or as a build flag.
// Note: the size of this buffer should be bigger than every SSD1306_Data::I2C_Write_BufferSize
// that you will instantiate inside your application code.
#ifndef SSD1306_PRIVATE_BUFFER_SIZE
#define SSD1306_PRIVATE_BUFFER_SIZE (uint32_t)100U
#endif

const uint32_t SSD1306_COLS = 128;
const uint32_t SSD1306_PAGES = 4;
const uint32_t SSD1306_BUFFER_SIZE = SSD1306_COLS * SSD1306_PAGES;
const uint32_t SSD1306_ROWS_PER_PAGE = 8;
const uint32_t SSD1306_ADDRESS_DEFAULT = 0x3c;
const uint32_t SSD1306_ROWS = SSD1306_PAGES * SSD1306_ROWS_PER_PAGE;

typedef I2C_Result (* I2C_WriteFun)(uint8_t address, uint8_t *data, uint32_t len, bool restart);
typedef void (*DelayUsFun)(uint32_t us);

/// \param [in] buffer User-allocated buffer (size must be at least SSD1306_BUFFER_SIZE), used to write pixels
/// before sending them to the display.
/// \param [in] I2C_Write Function pointer, used to write data on I2C bus, in master mode.
/// \param [in] I2C_Write_BufferSize How many bytes can be sent in a single transaction by the I2C Master.
/// \param [in] DelayUs Function pointer, used to delay microseconds.
typedef struct SSD1306_Data {
    uint8_t *buffer;
    I2C_WriteFun I2C_Write;
    uint16_t I2C_Write_BufferSize;
    DelayUsFun DelayUs;
} SSD1306_Data;


/// \brief Clears the buffer.
void SSD1306_Clear(SSD1306_Data *data);

/// \brief Write a pixel to the buffer.
void SSD1306_WritePixel(SSD1306_Data *data, uint8_t x, uint8_t y, bool color);

/// \brief Send buffer contents to the display.
void SSD1306_Update(SSD1306_Data *data);

/// \brief Initialize the display on the I2C bus, in horizontal mode.
/// \return true if I2C communication was successfull, false otherwise.
bool SSD1306_Init(SSD1306_Data *data);

/// \brief Write horizontal line on the buffer, given line length, and (x,y) coordinates of the upper leftmost pixel. 
/// \param [in] x X coordinate of the leftmost pixel.
/// \param [in] y Y coordinate of the line.
void SSD1306_WriteLineHoriz(SSD1306_Data *data, uint8_t x, uint8_t y, uint8_t len);

/// \brief Write vertical line on the buffer, given line length, and (x,y) coordinates of the upper leftmost pixel.
/// \param [in] x X coordinate of the line.
/// \param [in] y Y coordinate of the upper pixel.
void SSD1306_WriteLineVert(SSD1306_Data *data, uint8_t x, uint8_t y, uint8_t len);

/// \brief Write rectangle on the buffer, given width, height, and (x,y) coordinates of the top-left pixel.
/// \param [in] x X coordinate of the top-left pixel.
/// \param [in] y Y coordinate of the top-left pixel.
/// \param [in] w Rectangle width.
/// \param [in] h Rectangle height.
void SSD1306_WriteRectangle(SSD1306_Data *data, uint8_t x, uint8_t y, uint8_t w, uint8_t h);

/// \brief Write digit on the buffer, given the (x,y) coordinate of its bitmap.
/// \param [in] digit Digit from 0 to 9.
/// \param [in] x Leftmost X coordinate of the digit bitmap.
/// \param [in] y Upper Y coordinate of the digit bitmap.
void SSD1306_WriteDigit(SSD1306_Data *data, uint8_t digit, uint8_t x, uint8_t y);

#endif

#ifdef __cplusplus
}
#endif