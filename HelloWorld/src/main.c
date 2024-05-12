#include <stdint.h>
#include <stm32f4xx.h>

/* delay counter */
#define DELAY_MAX       300000

int main(void)
{
  /* turn on clock on GPIOA */
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

  /* set PA5 to output mode */
  GPIOA->MODER &= ~GPIO_MODER_MODE5_1;
  GPIOA->MODER |=  GPIO_MODER_MODE5_0;

  while(1) {
      /* set HIGH value on pin PA5 */
      GPIOA->BSRR |= GPIO_BSRR_BS5;
      for(uint32_t i=DELAY_MAX; i--;);

      /* set LOW value on pin PA5 */
      GPIOA->BSRR |= GPIO_BSRR_BR5;
      for(uint32_t i=DELAY_MAX; i--;);
    }
}