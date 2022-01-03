Author:
    Alberto Tontoni

Description:
    Switch ON/OFF the built-in blue LED based on whether a pushbutton is pressed or not.
	The button GPIO pin is configured using the internal PullUp resistor.

Requirements:
    -Freescale FRDM-KL25Z development board with CMSIS-DAP debug/programming app.
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

Credits:
    Chen et al. - "Freescale ARM Cortex-M Embedded Programming Using C Language"