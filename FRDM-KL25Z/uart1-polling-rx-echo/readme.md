## Description
* This program shows how to receive bytes to UART1 and echo back the bytes to the user.

* The baud rate has been set to 9600 by setting the value of SBR bits according to the bus clock frequency,	which is fixed to 20.971MHz by defining the CLOCK_SETUP macro within the system_MKL25Z4.h header.
	
## Usage
* Connect the FTDI232 chip to your PC and open up a serial terminal, baud rate = 9600.
* Once you type some characters, you will see them appearing on the terminal.

## Requirements
* Freescale FRDM-KL25Z development board with CMSIS-DAP debug/programming app.

* Since my laptop didn't manage to recognize the OpenSDA COM port, I've connected PTC3,4 to a FTDI232 chip, and then used PuTTY to visualize the characters sent by the mcu. 

## Connection Scheme
| FTDI232 chip | FRDM-KL25Z Pin |
| :----------: | :------------: |
|      TX      |      PTC3      |
|      RX      |      PTC4      |

## Credits
* Chen et al. - "Freescale ARM Cortex-M Embedded Programming Using C Language"

* [HD44780 Instruction Set](https://mil.ufl.edu/3744/docs/lcdmanual/commands.html)