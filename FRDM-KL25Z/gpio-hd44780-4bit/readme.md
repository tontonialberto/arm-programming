## Description
This program shows how to send commands and display characters on a HD44780 16x2 LCD display, using 4-bit mode to save some microcontroller pins.

## Requirements
* Freescale FRDM-KL25Z development board with CMSIS-DAP debug/programming app.

* HD44780 LCD display

* 10K Potentiometer

* 220 ohmm resistor

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

## Credits
* Chen et al. - "Freescale ARM Cortex-M Embedded Programming Using C Language"

* [HD44780 Instruction Set](https://mil.ufl.edu/3744/docs/lcdmanual/commands.html)