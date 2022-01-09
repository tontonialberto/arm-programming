Author:
    Alberto Tontoni

Description:
    Blinky project using built-in green LED and TPM1 timer.
	TPM source is set to OSCERCLK, which has 8MHz frequency.
	Therefore, the maximum obtainable delay (divider = 128) is approximately 1 second,
	with no need to reload the TPM.

Requirements:
    -Freescale FRDM-KL25Z development board with CMSIS-DAP debug/programming app.
    -No external hardware is required.

Credits:
    Chen et al. - "Freescale ARM Cortex-M Embedded Programming Using C Language"