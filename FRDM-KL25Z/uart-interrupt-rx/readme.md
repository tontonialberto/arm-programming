## Description
This program shows how to control the built-in red LED using UART0 in interrupt RX mode.

## Usage
* Open a serial monitor at 115200 baud.

* Writing the character 's' (ie. set) turns on the red LED.

* Writing 'c' (ie. clear) turns it off.

## Requirements
* Freescale FRDM-KL25Z development board with CMSIS-DAP debug/programming app

* Since my laptop didn't manage to recognize the OpenSDA COM port, I've connected PTA1,2 to a FTDI232 chip, and then used PuTTY to visualize the characters sent by the mcu

## Credits
* Chen et al. - "Freescale ARM Cortex-M Embedded Programming Using C Language"