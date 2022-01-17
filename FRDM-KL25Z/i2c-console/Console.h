#ifndef _CONSOLE_H
#define _CONSOLE_H

#include <MKL25Z4.h>

// Note: may block forever, and message must be a zero-ended string,
// otherwise buffer overflows may occur.
void UART0_Write(UART0_Type *uart, const char *message);

void UART0_Write(UART0_Type *uart, const char *message) {
	
	while(*message) {
		while(! (uart->S1 & UART_S1_TDRE_MASK) );
		uart->D = *message;
		++message;
	}
	
	while(! (uart->S1 & UART_S1_TC_MASK) );
}

#endif
