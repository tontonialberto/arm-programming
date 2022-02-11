## Description
* Drive the red and green built-in LEDs using two external switches, using falling-edge, both-edges interrupt modes.

* Software debouncing is implemented for one of the two switches, whereas the other one doesn't need bouncing prevention.
	
## Usage
* Press the switch on PTA1 to toggle red led.

* Press the switch on PTA2 to make green led blink two times.

* The two events can happen simultaneously.
	
## Requirements
* Freescale FRDM-KL25Z development board with CMSIS-DAP debug/programming app

* 2 Pushbutton Switches

## Connection Scheme
| Device Pin | FRDM-KL25Z Pin |
| :--------: | :------------: |
|  Switch 1  |      PTA1      |
|  Switch 2  | PTA2
               |

## Credits
* Chen et al. - "Freescale ARM Cortex-M Embedded Programming Using C Language"