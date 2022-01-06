#ifndef __HD44780_H__
#define __HD44780_H__

#include <stdint.h>

void LCD_Init(void);

void LCD_Command(uint8_t command);

void LCD_Data(uint8_t data);

/**
Precondition:
	-digit is a digit between 0 and 9
	-LCD cursor at pos. N
Postcondition:
	-The character corresponding to the digit
		is printed on the screen
	-LCD cursor at pos. N+1
*/
void LCD_WriteDigit(int8_t digit);

// Write a string in a left-to-right fashion
void LCD_WriteString(const char *str);

// LCD_WriteNumber(uint32_t number)
void LCD_WriteNumber(uint32_t number);

// Sets the cursor to the given row-column config
// NB: 	line is either 0 or 1
// 			pos should be between 0 and 15
void LCD_SetCursor(uint8_t line, uint8_t pos);

// Waits until the busy flag of the LCD display
// becomes 0.
void LCD_WaitBusy(void);

#endif
