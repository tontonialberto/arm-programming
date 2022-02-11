## Demo
![Demo GIF](./assets/i2c-console.gif)

## Description
Diagnostic firmware for I2C slave testing.
	
## Usage
* Connect your I2C devices to I2C0 pins of the FRDM board.

* Open a serial terminal to communicate with UART0, at 115200 baud.

* You can send the following commands, please note that the terminator char is **'.'** (ie. the dot):
	* **discover.** sends an I2C address write request to the I2C bus. Returns all the addresses that answered with an acknowledge
	* **slave \<hex address\>.** tests if the slave is connected
	* **write \<hex byte 0\> [\<hex byte 1\> ... \<hex byte n\>].** performs a burst write. **Warning:** you have to set the slave address before using this command (see **slave** command)

## Requirements
* Freescale FRDM-KL25Z development board with CMSIS-DAP debug/programming app

* One or more I2C slave devices

* Since my laptop didn't manage to recognize the OpenSDA COM port, I've connected a FTDI232 chip to the FRDM board, and then used PuTTY to visualize the characters sent by the mcu

## Connection Scheme
| Device Pin | FRDM-KL25Z Pin |
| :--------: | :------------: |
| Slave SCL  |      PTE1      |
| Slave SDA  |      PTE0      |
| FTDI232 TX |      PTA1      |
| FTDI232 RX |      PTA2      |

## Credits
* Chen et al. - "Freescale ARM Cortex-M Embedded Programming Using C Language"