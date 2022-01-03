Author:
    Alberto Tontoni

Description:
    Switch ON/OFF the built-in blue LED based on whether a pushbutton is pressed or not.
	The button GPIO pin is configured using the internal PullUp resistor.

Requirements:
    -Freescale FRDM-KL25Z development board with CMSIS-DAP debug/programming app.
    -Pushbutton Switch
	-The schematic is as follows:
		SW -> PTE4
		SW -> GND

Credits:
    Chen et al. - "Freescale ARM Cortex-M Embedded Programming Using C Language"