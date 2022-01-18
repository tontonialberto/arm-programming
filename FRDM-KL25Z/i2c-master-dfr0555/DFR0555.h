#ifndef _DFR0555_H
#define _DFR0555_H

#include <MKL25Z4.h>
#include <string.h>
#include "I2C_Master.h"
#include "delay.h"

#define ADDR_LCD 0x3e
#define ADDR_RGB 0x60

#define CONTROL_BYTE 0x80
#define RS_BYTE 0x40

// Function Set LCD command
#define FS_2_LINES_5x8_FONT 0x28

#define DISPLAY_CONTROL_ON 0xc

#define CLEAR_DISPLAY 0x1

#define ENTRY_MODE_CURSOR_INC 0x6

// Base addresses for LCD lines
#define LINE_1_BASE 0x80
#define LINE_2_BASE 0xc0

// Executes the initialization sequence.
// It also turns on the backlight.
I2C_Result DFR0555_Init(I2C_Type *i2c);

I2C_Result DFR0555_WriteChar(I2C_Type *i2c, char c);

I2C_Result DFR0555_SetCursor(I2C_Type *i2c, uint8_t line, uint8_t pos);

// Param str must be a null-terminated string.
// Notice: for efficiency reason, the first character of the string must be unused.
// It will be overwritten by the function implementation.
// Therefore, if you want to send str = "Hello", consider sending str = " Hello" instead.
I2C_Result DFR0555_WriteString(I2C_Type *i2c, char *str);

// After LCD power up, you have to delay in your code.
// Initialization may take some time (less than 1 second by the way).
I2C_Result DFR0555_Init(I2C_Type *i2c) {
	I2C_Result result = SUCCESS;
	uint8_t commands[2];
	
	for(int i=0; i<3; i++) {
		delayMs(10);
		if(SUCCESS == result) {
			commands[0] = CONTROL_BYTE;
			commands[1] = FS_2_LINES_5x8_FONT;
			result = I2C_Write(i2c, ADDR_LCD, commands, 2, 0);
		}
	}
	
	if(SUCCESS == result) {
		delayMs(10);
		commands[0] = CONTROL_BYTE;
		commands[1] = DISPLAY_CONTROL_ON;
		result = I2C_Write(i2c, ADDR_LCD, commands, 2, 0);
	}
	
	if(SUCCESS == result) {
		delayMs(10);
		commands[0] = CONTROL_BYTE;
		commands[1] = CLEAR_DISPLAY;
		result = I2C_Write(i2c, ADDR_LCD, commands, 2, 0);
	}
	
	if(SUCCESS == result) {
		delayMs(10);
		commands[0] = CONTROL_BYTE;
		commands[1] = ENTRY_MODE_CURSOR_INC;
		result = I2C_Write(i2c, ADDR_LCD, commands, 2, 0);
	}
	
	if(SUCCESS == result) {
		delayMs(10);
		commands[0] = 0x0;
		commands[1] = 0x0;
		result = I2C_Write(i2c, ADDR_RGB, commands, 2, 0);
	}
	
	if(SUCCESS == result) {
		delayMs(10);
		commands[0] = 0x8;
		commands[1] = 0xff;
		result = I2C_Write(i2c, ADDR_RGB, commands, 2, 0);
	}
	
	if(SUCCESS == result) {
		delayMs(10);
		commands[0] = 0x1;
		commands[1] = 0x20;
		result = I2C_Write(i2c, ADDR_RGB, commands, 2, 0);
	}
	
	if(SUCCESS == result) {
		delayMs(10);
		commands[0] = 0x2;
		commands[1] = 0xff;
		result = I2C_Write(i2c, ADDR_RGB, commands, 2, 0);
	}
	
	return result;
}

I2C_Result DFR0555_WriteChar(I2C_Type *i2c, char c) {
	I2C_Result result = SUCCESS;
	uint8_t buffer[2] = { RS_BYTE, c };
	
	result = I2C_Write(i2c, ADDR_LCD, buffer, 2, 0);
	return result;
}

I2C_Result DFR0555_SetCursor(I2C_Type *i2c, uint8_t line, uint8_t pos) {
	I2C_Result result = SUCCESS;
	uint8_t buffer[2];
	
	buffer[0] = CONTROL_BYTE;
	buffer[1] = ( (line == 0) ? (LINE_1_BASE + pos) : (LINE_2_BASE + pos) );
	result = I2C_Write(i2c, ADDR_LCD, buffer, 2, 0);
	
	return result;
}

I2C_Result DFR0555_WriteString(I2C_Type *i2c, char *str) {
	I2C_Result result = SUCCESS;
	str[0] = RS_BYTE;
	
	result = I2C_Write(i2c, ADDR_LCD, (uint8_t*)str, strlen(str), 0);
	
	return result;
}

#endif
