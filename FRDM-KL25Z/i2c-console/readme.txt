Author:
    Alberto Tontoni

Description:
	Diagnostic firmware for I2C slave testing.
	
Usage:
	Connect your I2C devices to I2C0 pins of the FRDM board.
	Open a serial terminal to communicate with UART0, at 115200 baud.
	You can send the following commands, please note that the terminator char is '.':
	- discover. sends an I2C address write request, and shows the addresses corresponding to received ACKs
	- slave <hex address>. tests if the slave is connected
	- write <hex byte 0> [<hex byte 1> ... <hex byte n>]. performs a burst write. Warning: you have to set the slave address before using this command (see "slave" command)

Requirements:
    -Freescale FRDM-KL25Z development board with CMSIS-DAP debug/programming app;
    -Since my laptop didn't manage to recognize the OpenSDA COM port, I've connected PTA1,2 
		to a FTDI232 chip, and then used PuTTY to visualize the characters sent by the mcu.
    -Slaves SCL connected to PTE1, SDA to PTE0

Credits:
    Chen et al. - "Freescale ARM Cortex-M Embedded Programming Using C Language"