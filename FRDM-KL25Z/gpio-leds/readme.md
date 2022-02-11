## Demo
![Demo GIF](./assets/gpio-leds.gif)

## Description
* This project shows basic usage of the GPIO, 
using the built-in RGB led to make two distinct color sequences:
    * The first one just involves switching between red, green, and blue;
    * The second one shows all possible combinations (2^3 = 8) of the three led colors (they can be either ON or OFF).

* Please note that the delayMs() function is not accurate. 

* Compiled using MDK-Keil v5.36 and Keil uVision 5.

* Flashed using OpenSDA and CMSIS-DAP firmware app.

## Requirements
* Freescale FRDM-KL25Z development board with CMSIS-DAP debug/programming app.

* No external hardware is required.

## Credits
* Chen et al. - "Freescale ARM Cortex-M Embedded Programming Using C Language"