Author:
    Alberto Tontoni

Description:
	This program shows how to receive data to UART0 and set/clean built-in RGB leds accordingly.
	Moreover, the status of LED pins are shown on HD44780 LCD display.

Usage:
	Connect the FRDM board to PC and open a serial terminal (eg. PuTTY) to communicate with the board.
	Send a digit from 0 to 7 to change the RGB color accordingly:
	0: White
	1: Yellow
	2: Purple
	3: Red
	4: Azure
	5: Green
	6: Blue
	7: Black

Requirements:
    -Freescale FRDM-KL25Z development board with CMSIS-DAP debug/programming app.
	-Since my laptop didn't manage to recognize the OpenSDA COM port, I've connected PTA1,2 
		to a FTDI232 chip, and then used PuTTY to visualize the characters sent by the mcu. 
    -HD44780 LCD display
	-10K Potentiometer
	-220 ohmm resistor
	-The schematic is as follows:
		HD44780
		---------------------------
		VSS	-> mcu GND 
		VDD -> mcu 5V
		V0  -> Pot. control
		RS  -> PTE20
		RNW -> PTE21
		E   -> PTE22
		D0	-> PTB8
		D1  -> PTB9
		D2  -> PTB10
		D3  -> PTB11
		D4  -> PTB0
		D5  -> PTB1
		D6  -> PTB2
		D7  -> PTB3
		A   -> mcu 5V (220 ohm R)
		K   -> mcu GND
		
		FTDI232 chip
		---------------------------
		TX -> PTA1
		RX -> PTA2

Credits:
    Chen et al. - "Freescale ARM Cortex-M Embedded Programming Using C Language"
	HD44780 Instruction Set - https://mil.ufl.edu/3744/docs/lcdmanual/commands.html