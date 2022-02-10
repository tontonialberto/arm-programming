## Demo
![Demo GIF](./assets/oled-joystick-demo.gif)

## Description
* This program shows how to read analog values from a 2-axis joystick, and display them on an SSD1306 OLED display.
* Joystick Vy is read using ADC0 interrupts, and the value is converted to [-100, +100] interval.
* I've implemented a small library for SSD1306. It should be portable between different microcontrollers. You can find it under [this](./libs/SSD1306/) directory (any feedback is appreciated).

## Requirements
* Freescale FRDM-KL25Z development board with CMSIS-DAP debug/programming app
* SSD1306 128x32 OLED display
* 2-axis Joystick

## Connection Scheme
| Device Pin  | FRDM-KL25Z Pin |
|-------------|----------------|
| OLED SCL    | PTB0           |
| OLED SDA    | PTB1           |
| Joystick Vy | PTB3           |