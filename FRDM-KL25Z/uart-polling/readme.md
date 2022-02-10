## Description
* This program shows how to configure UART0 peripheral to send characters to a PC.

* Polling the control register is performed to detect when to transmit the next byte.

## Usage
* Open a serial terminal, at 115200 baud.

* You will see the message sent by the microcontroller.
	
## Requirements
* Freescale FRDM-KL25Z development board with CMSIS-DAP debug/programming app

* Since my laptop didn't manage to recognize the OpenSDA COM port, I've connected PTA1,2 to a FTDI232 chip, and then used PuTTY to visualize the characters sent by the mcu

## Credits
* Chen et al. - "Freescale ARM Cortex-M Embedded Programming Using C Language"