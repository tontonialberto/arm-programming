Author:
    Alberto Tontoni

Description:
    This project shows basic usage of the PWM module on TPM2, Channel 0.
	To drive servomotors easily, I've implemented a Servo data structure
	and a utility function, both inspired by the Arduino Servo library.
	In the program, the servo loops between 4 different positions.
	
Note:
	You may have to adjust min and max duty cycle values according to your specific motor
	(datasheets may be unreliable!)

Requirements:
    -Freescale FRDM-KL25Z development board with CMSIS-DAP debug/programming app.
    -SG90 servo, or Futaba S3003, or similar
	-Schematic is as follows:
		Servomotor Signal -> PTE22
		Servo VCC -> External 5V power supply
		Common ground between microcontroller and Servo

Credits:
    Chen et al. - "Freescale ARM Cortex-M Embedded Programming Using C Language"
	Arduino Servo library - https://www.arduino.cc/reference/en/libraries/servo/