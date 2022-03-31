#include <MKL25Z4.h>

#define PIN_LED_GREEN 19

#define A "hello"

int main() {
  SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
  PORTB->PCR[PIN_LED_GREEN] = PORT_PCR_MUX(1);
  GPIOB->PDDR |= (1 << PIN_LED_GREEN);

  SysTick->CTRL &= ~SysTick_CTRL_CLKSOURCE_Msk;
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

  while (1) {
    // 1s delay
    SysTick->LOAD = (SystemCoreClock / 16) - 1;

    while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk))
      ;

    GPIOB->PTOR = 1 << PIN_LED_GREEN;
  }
}
