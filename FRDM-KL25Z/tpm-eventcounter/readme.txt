Author:
    Alberto Tontoni

Description:
    Count how many times an external switch gets released by using TPM2
	event counter peripheral.
	
Usage:
	Put a breakpoint right after the code that populates the count variable. 
	Then, press and release the switch to visualize how many times the event has happened.

Note:
	Due to the switch bouncing issue, the event counting may be not accurate.

Requirements:
    -Freescale FRDM-KL25Z development board with CMSIS-DAP debug/programming app.
    -Pushbutton Switch
	-The schematic is as follows:
		SW -> PTE29
		SW -> GND

Credits:
    Chen et al. - "Freescale ARM Cortex-M Embedded Programming Using C Language"