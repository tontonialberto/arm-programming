Author:
    Alberto Tontoni

Description:
	This program shows how to communicate through the I2C1 channel.
	For this purpose, an MPU6050 is interfaced with the microcontroller.
	To make the code as readable and easy as possible,
	I've implemented a minimalist I2C master driver.
	The accelerometer readings can be retrieved using a serial terminal.
	
Usage:
	Open a serial terminal using 115200 as baud rate.
	You will see the acceleration values expressed in g.
	
Requirements:
    -Freescale FRDM-KL25Z development board with CMSIS-DAP debug/programming app;
    -Since my laptop didn't manage to recognize the OpenSDA COM port, I've connected PTA1,2 
		to a FTDI232 chip, and then used PuTTY to visualize the characters sent by the mcu. 

Credits:
    Chen et al. - "Freescale ARM Cortex-M Embedded Programming Using C Language"
	MPU-6000 and MPU-6050 Register Map and Descriptions, Revision 4.2
	FRDM-KL25Z Pin usage and pinout chart, Revision 1.0