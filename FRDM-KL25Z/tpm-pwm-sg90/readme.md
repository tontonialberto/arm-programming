## Description
* This project shows basic usage of the PWM module on TPM2, Channel 0.

* To drive servomotors easily, I've implemented a Servo data structure and a utility function, both inspired by the [Arduino Servo library](https://www.arduino.cc/reference/en/libraries/servo/).

* In the program, the servo loops between 4 different positions.
	
## Note
You may have to adjust min and max duty cycle values according to your specific motor (datasheets may be unreliable!)

## Requirements
* Freescale FRDM-KL25Z development board with CMSIS-DAP debug/programming app.

* SG90 servo, or Futaba S3003, or similar

## Connection Scheme
|  Device Pin  | FRDM-KL25Z |
| :----------: | :--------: |
| Servo Signal |   PTE22    |

**Note**: servo must be externally powered. Hence you will also need common ground between servo and FRDM board.

## Credits
* Chen et al. - "Freescale ARM Cortex-M Embedded Programming Using C Language"