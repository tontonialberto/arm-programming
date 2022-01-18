Author:
    Alberto Tontoni

Description:
	Basic operation of the DFRobot DFR0555 Gravity LCD module.
	Init procedure, cursor setting, and display of characters are implemented
	as a minimal C header-only library.

Requirements:
    	-Freescale FRDM-KL25Z development board with CMSIS-DAP debug/programming app;
    	-Since my laptop didn't manage to recognize the OpenSDA COM port, I've connected PTA1,2 
		to a FTDI232 chip, and then used PuTTY to visualize the characters sent by the mcu.
    	-Schematic between the LCD module and Freedom board:
		SCL -> PTE1
		SDA -> PTE0

Credits:
    	Chen et al. - "Freescale ARM Cortex-M Embedded Programming Using C Language"
	AiP31068-AX-BJ-153EN - Product specification
	DFRobot_RGBLCD1602 Arduino library