## Description
* Basic operation of the DFRobot DFR0555 Gravity LCD module.

* Init procedure, cursor setting, and display of characters are implemented as a minimalist C header-only library.

## Requirements
* Freescale FRDM-KL25Z development board with CMSIS-DAP debug/programming app

* DFRobot DFR0555 Gravity LCD Display 16x2

## Connection Scheme
| Device Pin  | FRDM-KL25Z Pin |
| :---------: | :------------: |
| DFR0555 SCL |      PTE1      |
| DFR0555 SDA | PTE0
               |

## Credits
* Chen et al. - "Freescale ARM Cortex-M Embedded Programming Using C Language"

* AiP31068-AX-BJ-153EN - Product specification

* DFRobot_RGBLCD1602 Arduino library