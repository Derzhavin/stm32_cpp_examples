extern "C" 
{
  #include <stdint.h>
  #include <stdio.h>
  #include "stm32f4xx_hal.h"
  #include "stm32f4xx_hal_uart.h"
}

#define USARTx                           USART1
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART1_CLK_ENABLE();
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE() 

#define USARTx_FORCE_RESET()             __HAL_RCC_USART1_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART1_RELEASE_RESET()

#define USARTx_TX_PIN                    GPIO_PIN_9
#define USARTx_TX_GPIO_PORT              GPIOA  
#define USARTx_TX_AF                     GPIO_AF7_USART1
#define USARTx_RX_PIN                    GPIO_PIN_10
#define USARTx_RX_GPIO_PORT              GPIOA 
#define USARTx_RX_AF                     GPIO_AF7_USART1

UART_HandleTypeDef UartHandle;

extern "C"
{
  static void Error_Handler(void)
  {
    while(1)
    {
    }
  }


  void NMI_Handler(void)
  {
  }

  void HardFault_Handler(void)
  {
    while (1)
    {
    }
  }

  void MemManage_Handler(void)
  {
    while (1)
    {
    }
  }

  void BusFault_Handler(void)
  {
    while (1)
    {
    }
  }

  void UsageFault_Handler(void)
  {
    while (1)
    {
    }
  }

  void SVC_Handler(void)
  {

  }

  void DebugMon_Handler(void)
  {

  }

  void PendSV_Handler(void)
  {

  }


  void SysTick_Handler(void)
  {
    HAL_IncTick();
  }

  void HAL_UART_MspInit(UART_HandleTypeDef *huart)
  {  
    GPIO_InitTypeDef  GPIO_InitStruct;
    
    USARTx_TX_GPIO_CLK_ENABLE();
    USARTx_RX_GPIO_CLK_ENABLE();
    
    USARTx_CLK_ENABLE(); 
    
    GPIO_InitStruct.Pin       = USARTx_TX_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = USARTx_TX_AF;
    
    HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);
      
    GPIO_InitStruct.Pin = USARTx_RX_PIN;
    GPIO_InitStruct.Alternate = USARTx_RX_AF;
      
    HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);
  }

  void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
  {
    USARTx_FORCE_RESET();
    USARTx_RELEASE_RESET();

    HAL_GPIO_DeInit(USARTx_TX_GPIO_PORT, USARTx_TX_PIN);
    HAL_GPIO_DeInit(USARTx_RX_GPIO_PORT, USARTx_RX_PIN);
  }
}

int main(int, char* argv[]){
  HAL_Init();

  UartHandle.Instance          = USART1;
  
  UartHandle.Init.BaudRate     = 9600;
  UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits     = UART_STOPBITS_1;
  UartHandle.Init.Parity       = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode         = UART_MODE_TX;
  UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

  if(HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    Error_Handler(); 
  }

  uint8_t str[] = "Aloha!\r\n";
  
  while (1)
  {
    HAL_UART_Transmit(&UartHandle, str, 8, 30);
    HAL_Delay(500);
  }

	return 0;
}
