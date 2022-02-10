## Description
* This program shows how to use the ADC0 peripheral in software trigger mode, using interrupts.
* It reads the raw 16-bit value from ADC and stores it inside a variable.

## Usage
Open a debug session and watch the volatile variables changing their value according to the status of the connected analog device.

## Requirements
* Freescale FRDM-KL25Z development board with CMSIS-DAP debug/programming app.
* 2-axis joystick (by the way, any analog sensor will do)

## Connection Scheme
| Device Pin | FRDM-KL25Z Pin  |
|------------|-----------------|
| Joystick Vy| PTB3			   |