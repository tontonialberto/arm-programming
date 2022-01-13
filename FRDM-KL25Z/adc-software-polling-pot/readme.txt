Author:
    Alberto Tontoni

Description:
	This program shows how to use the ADC0 peripheral in software trigger mode.
	It reads the raw 16-bit value from ADC and converts it into voltage range [0, 3.3] (assuming potentiometer is connected)
	and celsius degrees (assuming KY-013 is connected).

Usage:
	Open a debug session and watch the volatile variables changing their value
	according to the status of the connected analog device.

Requirements:
    -Freescale FRDM-KL25Z development board with CMSIS-DAP debug/programming app.
	-10K Potentiometer
	-KY-013 analog temperature sensor (optional if you have the potentiometer)
	-The schematic is as follows:
		Potentiometer / KY-013 S pin -> PTE22

Credits:
    Chen et al. - "Freescale ARM Cortex-M Embedded Programming Using C Language"
	KY-013 datasheet: https://datasheetspdf.com/pdf-file/1402026/Joy-IT/KY-013/1