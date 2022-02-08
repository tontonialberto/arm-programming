#include "SSD1306.h"
#include <stdlib.h>
#include "SSD1306_Digits_V0.h"

const uint32_t SSD1306_ROWS_PER_PAGE = 8;
const uint32_t SSD1306_ADDRESS_DEFAULT = 0x3c;
const uint32_t SSD1306_ROWS = SSD1306_PAGES * SSD1306_ROWS_PER_PAGE;

static uint8_t privateBuffer[SSD1306_PRIVATE_BUFFER_SIZE];

void SSD1306_Clear(SSD1306_Data *data) {

    for(uint32_t i=0; i<SSD1306_BUFFER_SIZE; i++) {
        data->buffer[i] = 0;
    }
}

void SSD1306_WritePixel(SSD1306_Data *data, uint8_t x, uint8_t y, bool color) {

    if( (x >= SSD1306_COLS) || (y >= SSD1306_ROWS) ) return;

    uint32_t index = x + (SSD1306_COLS * (y / SSD1306_ROWS_PER_PAGE));
    uint8_t pixel = y % SSD1306_ROWS_PER_PAGE;
    if(color) {
        data->buffer[index] |= (1 << pixel);
    }
    else {
        data->buffer[index] &= ~(1 << pixel);
    }
}

bool SSD1306_Init(SSD1306_Data *data) {
    I2C_Result i2c_res = I2C_RESULT_SUCCESS;
    I2C_WriteFun i2c_write = data->I2C_Write;
    const uint8_t slaveAddr = SSD1306_ADDRESS_DEFAULT;
    DelayUsFun delayUs = data->DelayUs;

    if((i2c_write == NULL) || (delayUs == NULL)) return false;

    delayUs(2);

    privateBuffer[0] = (uint8_t)0x0U;
    privateBuffer[1] = (uint8_t)0xaeU;
    privateBuffer[2] = (uint8_t)0xd5U;
    privateBuffer[3] = (uint8_t)0x80U;
    privateBuffer[4] = (uint8_t)0xa8U;
    i2c_res = i2c_write(slaveAddr, privateBuffer, 5, false);

    delayUs(2);

    if(I2C_RESULT_SUCCESS == i2c_res) {
        privateBuffer[0] = (uint8_t)0x0U;
        privateBuffer[1] = (uint8_t)0x1fU;
        i2c_res = i2c_write(slaveAddr, privateBuffer, 2, false);
    }

    delayUs(2);

    if(I2C_RESULT_SUCCESS == i2c_res) {
        privateBuffer[0] = (uint8_t)0x0U;
        privateBuffer[1] = (uint8_t)0xd3U;
        privateBuffer[2] = (uint8_t)0x0U;
        i2c_res = i2c_write(slaveAddr, privateBuffer, 3, false);
    }

    delayUs(2);

    if(I2C_RESULT_SUCCESS == i2c_res) {
        privateBuffer[0] = (uint8_t)0x0U;
        privateBuffer[1] = (uint8_t)0x40U;
        i2c_res = i2c_write(slaveAddr, privateBuffer, 2, false);
    }

    delayUs(2);

    if(I2C_RESULT_SUCCESS == i2c_res) {
        privateBuffer[0] = (uint8_t)0x0U;
        privateBuffer[1] = (uint8_t)0x8dU;
        i2c_res = i2c_write(slaveAddr, privateBuffer, 2, false);
    }

    delayUs(2);

    if(I2C_RESULT_SUCCESS == i2c_res) {
        privateBuffer[0] = (uint8_t)0x0U;
        privateBuffer[1] = (uint8_t)0x14U;
        i2c_res = i2c_write(slaveAddr, privateBuffer, 2, false);
    }

    delayUs(2);

    if(I2C_RESULT_SUCCESS == i2c_res) {
        privateBuffer[0] = (uint8_t)0x0U;
        privateBuffer[1] = (uint8_t)0x20U;
        privateBuffer[2] = (uint8_t)0x0U;
        privateBuffer[3] = (uint8_t)0xa1U;
        privateBuffer[4] = (uint8_t)0xc8U;
        i2c_res = i2c_write(slaveAddr, privateBuffer, 5, false);
    }

    delayUs(2);

    if(I2C_RESULT_SUCCESS == i2c_res) {
        privateBuffer[0] = (uint8_t)0x0U;
        privateBuffer[1] = (uint8_t)0xdaU;
        i2c_res = i2c_write(slaveAddr, privateBuffer, 2, false);
    }

    delayUs(2);

    if(I2C_RESULT_SUCCESS == i2c_res) {
        privateBuffer[0] = (uint8_t)0x0U;
        privateBuffer[1] = (uint8_t)0x2U;
        i2c_res = i2c_write(slaveAddr, privateBuffer, 2, false);
    }

    delayUs(2);

    if(I2C_RESULT_SUCCESS == i2c_res) {
        privateBuffer[0] = (uint8_t)0x0U;
        privateBuffer[1] = (uint8_t)0x81U;
        i2c_res = i2c_write(slaveAddr, privateBuffer, 2, false);
    }

    delayUs(2);

    if(I2C_RESULT_SUCCESS == i2c_res) {
        privateBuffer[0] = (uint8_t)0x0U;
        privateBuffer[1] = (uint8_t)0x8fU;
        i2c_res = i2c_write(slaveAddr, privateBuffer, 2, false);
    }

    delayUs(2);

    if(I2C_RESULT_SUCCESS == i2c_res) {
        privateBuffer[0] = (uint8_t)0x0U;
        privateBuffer[1] = (uint8_t)0xd9U;
        i2c_res = i2c_write(slaveAddr, privateBuffer, 2, false);
    }

    delayUs(2);

    if(I2C_RESULT_SUCCESS == i2c_res) {
        privateBuffer[0] = (uint8_t)0x0U;
        privateBuffer[1] = (uint8_t)0xf1U;
        i2c_res = i2c_write(slaveAddr, privateBuffer, 2, false);
    }

    delayUs(2);

    if(I2C_RESULT_SUCCESS == i2c_res) {
        privateBuffer[0] = (uint8_t)0x0U;
        privateBuffer[1] = (uint8_t)0xdbU;
        privateBuffer[2] = (uint8_t)0x40U;
        privateBuffer[3] = (uint8_t)0xa4U;
        privateBuffer[4] = (uint8_t)0xa6U;
        privateBuffer[5] = (uint8_t)0x2eU;
        privateBuffer[6] = (uint8_t)0xafU;
        i2c_res = i2c_write(slaveAddr, privateBuffer, 7, false);
    }

    return I2C_RESULT_SUCCESS == i2c_res;
}

void SSD1306_Update(SSD1306_Data *data) {
    I2C_Result i2c_res = I2C_RESULT_SUCCESS;
    I2C_WriteFun i2c_write = data->I2C_Write;
    const uint8_t slaveAddr = SSD1306_ADDRESS_DEFAULT;
    DelayUsFun delayUs = data->DelayUs;

    if(data->I2C_Write_BufferSize >= SSD1306_PRIVATE_BUFFER_SIZE) return;

    delayUs(2);

    // Set page start address = 0, end address = 7.
    // Set column start address = 0, end address = 127.
    // Issue data write command.
    privateBuffer[0] = (uint8_t)0x0U;
    privateBuffer[1] = (uint8_t)0x22U;
    privateBuffer[2] = (uint8_t)0x0U;
    privateBuffer[3] = (uint8_t)0x7U;
    privateBuffer[4] = (uint8_t)0x21U;
    privateBuffer[5] = (uint8_t)0x0U;
    privateBuffer[6] = (uint8_t)0x7fU;
    do {
        i2c_res = i2c_write(slaveAddr, privateBuffer, 7, true);
    } while(I2C_RESULT_SUCCESS != i2c_res);

    delayUs(2);

    // Write pixels.
    // Display is updated in chunks, according to the provided I2C buffer size.
    // This means that a whole display update potentially requires multiple I2C transactions. 
    uint32_t dataBytesSent = 0;
    while(dataBytesSent < SSD1306_BUFFER_SIZE) {
        
        const uint32_t remainingDataBytes = SSD1306_BUFFER_SIZE - dataBytesSent;
        const uint16_t availableBufferSize = data->I2C_Write_BufferSize - 1; // 1 byte is needed for the control byte 0x40
        // Compute how many data bytes have to be sent in the next transaction.
        const uint32_t transactionLength = (remainingDataBytes < availableBufferSize) ? remainingDataBytes : availableBufferSize;

        // Put control + data bytes inside the private buffer.
        privateBuffer[0] = (uint8_t)0x40;
        for(uint32_t i=0; i<transactionLength; i++) {
            privateBuffer[i+1] = data->buffer[dataBytesSent + i];  
        }

        dataBytesSent += transactionLength;
        bool restart = dataBytesSent != SSD1306_BUFFER_SIZE;

        // Start the transaction on I2C bus.
        i2c_write(slaveAddr, privateBuffer, transactionLength + 1, restart);
    }
}

void SSD1306_WriteLineHoriz(SSD1306_Data *data, uint8_t x, uint8_t y, uint8_t len) {
    for(uint8_t i=0; i<len; i++) {
        SSD1306_WritePixel(data, x+i, y, true);
    }
}

void SSD1306_WriteLineVert(SSD1306_Data *data, uint8_t x, uint8_t y, uint8_t len) {
    for(uint8_t i=0; i<len; i++) {
        SSD1306_WritePixel(data, x, y+i, true);
    }
}

void SSD1306_WriteRectangle(SSD1306_Data *data, uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
    if((w == 0) || (h == 0)) return;

    for(uint16_t i=x; i<x+w; i++) {
        SSD1306_WritePixel(data, i, y, true);
        SSD1306_WritePixel(data, i, y + h - 1, true);
    }
    for(uint16_t j=y+1; j<y+h-1; j++) {
        SSD1306_WritePixel(data, x, j, true);
        SSD1306_WritePixel(data, x + w - 1, j, true);
    }
}

void SSD1306_WriteDigit(SSD1306_Data *data, uint8_t digit, uint8_t x, uint8_t y) {
    
    if(digit > 9) return;

    for(uint8_t i=0; i<SSD1306_DIGITS_V0_ROWS; i++) {
        for(uint8_t j=0; j<SSD1306_DIGITS_V0_COLS; j++) {
            bool color = SSD1306_DIGITS_V0[digit][j + (i * SSD1306_DIGITS_V0_COLS)];
            SSD1306_WritePixel(data, x + j, y + i, color);
        }
    }
}
