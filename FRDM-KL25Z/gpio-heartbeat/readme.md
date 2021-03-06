## Description
* Basically, an endless while loop where at each step a GPIO pin gets toggled.

* The pin signal can be used to measure how much time it takes to execute a single iteration.

* This can be useful to write naive delay functions, or to compare different compilers and/or different optimization levels.

## Requirements
* Freescale FRDM-KL25Z development board with CMSIS-DAP debug/programming app.

* Logic Level Analyzer, if you want to analyze the signal generated by the GPIO pin.
In this case, you will also need a software that allows you to interact with the analyzer (eg. I used [Sigrok PulseView](https://sigrok.org/wiki/PulseView)).

## Credits
* Chen et al. - "Freescale ARM Cortex-M Embedded Programming Using C Language"