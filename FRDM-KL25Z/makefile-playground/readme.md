## Description
This is an experiment to show how you can leverage a Makefile
to build, lint, and deploy (and customize) an embedded project generated with Keil UVision.

It is meant to be used with a NXP FRDM-KL25Z evaluation board.

## Features
* **Build** firmware, using ARM Clang 
* **Format** source code, using clang-format
* **Flash** firmware onto the board, using Keil UVision
* **Debug** using Keil UVision
	
## Usage
* Edit the Makefile so as to reflect your local configuration
* Open a terminal to interact with the Makefile
	* **make build**: compile and link the firmware
	* **make format**: format every .h, .c file inside the project
	* **make flash**: flash firmware onto the board
	* **make clean**: clear all object files generated by the build
	* **make debug**: open the project using Keil UVision in debug mode

## Requirements
* GNU Make<sup>*</sup>
* Keil UVision 4
* ARM Clang compiler and linker<sup>**</sup> (ie. armclang, armlink)
* clang-format

<hr>
<sup>*</sup> Developed using GNU Make 3.81.

<sup>**</sup> Developed using ARM Compiler 6.16.