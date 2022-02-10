## Description
This program shows how to receive data to UART0 and display the message on HD44780 LCD.

## Requirements
* Freescale FRDM-KL25Z development board with CMSIS-DAP debug/programming app

* Since my laptop didn't manage to recognize the OpenSDA COM port, I've connected PTA1,2 to a FTDI232 chip, and then used PuTTY to visualize the characters sent by the mcu

* HD44780 LCD display

* 10K Potentiometer

* 220 ohm resistor

## Connection Scheme 

| HD44780 Pin | FRDM-KL25Z Pin / Potentiometer |
|:-----------:|:------------------------------:|
|     VSS     |               GND              |
|     VDD     |               5V               |
|      V0     |      Potentiometer Control     |
|      RS     |              PTE20             |
|     RNW     |              PTE21             |
|      E      |              PTE22             |
|      D4     |              PTB0              |
|      D5     |              PTB1              |
|      D6     |              PTB2              |
|      D7     |              PTB3              |
|      A      |      5V (through 220ohm R)     |
|      K      |               GND              |


| FTDI232 chip | FRDM-KL25Z Pin |
| :----------: | :------------: |
| TX           |      PTA1      |
| RX           |      PTA2      |

## Credits
* Chen et al. - "Freescale ARM Cortex-M Embedded Programming Using C Language"

* [HD44780 Instruction Set](https://mil.ufl.edu/3744/docs/lcdmanual/commands.html)