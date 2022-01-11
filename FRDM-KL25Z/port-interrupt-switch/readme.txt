Author:
    Alberto Tontoni

Description:
	Drive the led and green built-in LEDs using two external switches, 
	using falling-edge, both-edges interrupt modes.
	Software debouncing is implemented for one of the two switches,
	whereas the other one doesn't need bouncing prevention.
	
Usage:
	Press the switch on PTA1 to toggle red led.
	Press the switch on PTA2 to make green led blink two times.
	The two events can happen simultaneously.
	
Requirements:
    -Freescale FRDM-KL25Z development board with CMSIS-DAP debug/programming app.
    -2x Pushbutton Switch
	-The schematic is as follows:
		SW1 -> PTA1
		SW1 -> GND
		SW2 -> PTA2
		SW2 -> GND

Credits:
    Chen et al. - "Freescale ARM Cortex-M Embedded Programming Using C Language"