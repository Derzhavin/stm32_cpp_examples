#include <stdint.h>
#include <stm32f4xx.h>

int main(void)
{
  /* turn on clock on GPIOA */
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

  /* set PA5 to output mode */
  GPIOA->MODER &= ~GPIO_MODER_MODE5_1;
  GPIOA->MODER |=  GPIO_MODER_MODE5_0;

  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

  GPIOC->MODER &= ~GPIO_MODER_MODER13;
  GPIOC->MODER &= ~GPIO_MODER_MODE13_1;
  GPIOC->MODER &= ~GPIO_MODER_MODE13_0;

  // GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD13_1;
  // GPIOC->PUPDR |= GPIO_PUPDR_PUPD13_0;

  GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD13_0;
  GPIOC->PUPDR |= GPIO_PUPDR_PUPD13_1;

  uint32_t button_val = 0;
  while(1) {
    button_val = GPIOC->IDR & GPIO_IDR_ID13;
    if (button_val){
      /* set HIGH value on pin PA5 */
      GPIOA->BSRR |= GPIO_BSRR_BS5;
    }
    else
    {
      /* set LOW value on pin PA5 */
      GPIOA->BSRR |= GPIO_BSRR_BR5;
    }
  }
}