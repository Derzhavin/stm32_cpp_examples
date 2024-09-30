#include <cstdint>

extern "C"
{
  #include "stm32f4xx_hal.h"
  #include "stm32f4xx_hal_gpio.h"
  #include "stm32f4xx_hal_i2c.h"
}

#define I2C_ADDRESS 0x31
#define I2C_SLAVE_ADDRESS 0x30


#define I2Cx                             I2C1
#define I2Cx_CLK_ENABLE()                __HAL_RCC_I2C1_CLK_ENABLE()
#define I2Cx_SDA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()
#define I2Cx_SCL_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE() 

#define I2Cx_FORCE_RESET()               __HAL_RCC_I2C1_FORCE_RESET()
#define I2Cx_RELEASE_RESET()             __HAL_RCC_I2C1_RELEASE_RESET()

/* Definition for I2Cx Pins */
#define I2Cx_SCL_PIN                    GPIO_PIN_6
#define I2Cx_SCL_GPIO_PORT              GPIOB
#define I2Cx_SCL_AF                     GPIO_AF4_I2C1
#define I2Cx_SDA_PIN                    GPIO_PIN_9
#define I2Cx_SDA_GPIO_PORT              GPIOB
#define I2Cx_SDA_AF                     GPIO_AF4_I2C1

/* Definition for I2Cx's NVIC */
#define I2Cx_EV_IRQn                    I2C1_EV_IRQn
#define I2Cx_EV_IRQHandler              I2C1_EV_IRQHandler
#define I2Cx_ER_IRQn                    I2C1_ER_IRQn
#define I2Cx_ER_IRQHandler              I2C1_ER_IRQHandler

#define RXBUFFERSIZE 128
#define TXBUFFERSIZE 128

I2C_HandleTypeDef I2cHandle;

uint8_t aTxBuffer[] = {0xA, 0xB, 0xC, 0xD, 0xE};

uint8_t aRxBuffer[RXBUFFERSIZE];

extern "C"
{
  static void SystemClock_Config(void);
  static void Error_Handler(void);

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

  void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
  {  
    GPIO_InitTypeDef  GPIO_InitStruct;
    
    /*##-1- Enable GPIO Clocks #################################################*/
    /* Enable GPIO TX/RX clock */
    I2Cx_SCL_GPIO_CLK_ENABLE();
    I2Cx_SDA_GPIO_CLK_ENABLE();
    
    /*##-2- Configure peripheral GPIO ##########################################*/  
    /* I2C TX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = I2Cx_SCL_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = I2Cx_SCL_AF;
    
    HAL_GPIO_Init(I2Cx_SCL_GPIO_PORT, &GPIO_InitStruct);
      
    /* I2C RX GPIO pin configuration  */
    GPIO_InitStruct.Pin = I2Cx_SDA_PIN;
    GPIO_InitStruct.Alternate = I2Cx_SDA_AF;
      
    HAL_GPIO_Init(I2Cx_SDA_GPIO_PORT, &GPIO_InitStruct);

    /*##-3- Enable I2C peripheral Clock ########################################*/ 
    /* Enable I2C1 clock */
    I2Cx_CLK_ENABLE();
        
    /*##-4- Configure the NVIC for I2C #########################################*/   
    /* NVIC for I2C1 */
    HAL_NVIC_SetPriority(I2Cx_ER_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(I2Cx_ER_IRQn);
    HAL_NVIC_SetPriority(I2Cx_EV_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(I2Cx_EV_IRQn);
  }

  void SysTick_Handler(void)
  {
    HAL_IncTick();
  }

  void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c)
  {
    /*##-1- Reset peripherals ##################################################*/
    I2Cx_FORCE_RESET();
    I2Cx_RELEASE_RESET();

    /*##-2- Disable peripherals and GPIO Clocks ################################*/
    /* Configure I2C Tx as alternate function  */
    HAL_GPIO_DeInit(I2Cx_SCL_GPIO_PORT, I2Cx_SCL_PIN);
    /* Configure I2C Rx as alternate function  */
    HAL_GPIO_DeInit(I2Cx_SDA_GPIO_PORT, I2Cx_SDA_PIN);
    
    /*##-3- Disable the NVIC for I2C ###########################################*/
    HAL_NVIC_DisableIRQ(I2Cx_ER_IRQn);
    HAL_NVIC_DisableIRQ(I2Cx_EV_IRQn);
  }
}

int main(void)
{    
  HAL_Init();

  SystemClock_Config();

  I2cHandle.Instance             = I2C1;
  // I2cHandle.Init.Timing          = I2C_TIMING;
  I2cHandle.Init.OwnAddress1     = I2C_ADDRESS;
  I2cHandle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
  I2cHandle.Init.ClockSpeed      = 100000;
  I2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  I2cHandle.Init.DutyCycle       = I2C_DUTYCYCLE_16_9;
  I2cHandle.Init.OwnAddress2     = 0xFF;
  // I2cHandle.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  I2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  I2cHandle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;  

  
  if(HAL_I2C_Init(&I2cHandle) != HAL_OK)
  {
    Error_Handler();    
  }
  
while(1) {
    // HAL_StatusTypeDef status =  HAL_I2C_Master_Transmit_IT(&I2cHandle, 
    // I2C_SLAVE_ADDRESS,
    // &aTxBuffer[0], 5);
    // while (HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY)
    // {
    // }
    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(&I2cHandle, 
                        I2C_SLAVE_ADDRESS, 
                        &aTxBuffer[0],
                        5,
                        HAL_MAX_DELAY);

  }

}

extern "C"
{
  void I2Cx_EV_IRQHandler(void)
  {
    HAL_I2C_EV_IRQHandler(&I2cHandle);
  }

  void I2Cx_ER_IRQHandler(void)
  {
    HAL_I2C_ER_IRQHandler(&I2cHandle);
  }

  void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *i2c_handle)
  {

  }

  void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *i2c_handle)
  {
    
    // if (g_i2c_buf[0] == I2C_PWR_ON_CMD)
    // {
    //   g_i2c_cmd = PwrOnI2cCmd;
    // }
    // else if (g_i2c_buf[1] == I2C_PWR_OFF_CMD)
    // {
    //   g_i2c_cmd = PwrOffI2cCmd;
    // }
    // else if (g_i2c_buf[2] == I2C_SHUTDOWN_CMD)
    // {
    //   g_i2c_cmd = ShutdownI2cCmd;
    // }
  }

  void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *i2c_handle)
  {
    if (HAL_I2C_GetError(&I2cHandle) != HAL_I2C_ERROR_AF)
    {
      Error_Handler();
    }
  }


  static void Error_Handler(void)
  {
    while(1)
    {

    }
  }

  static void SystemClock_Config(void)
  {
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;

    __HAL_RCC_PWR_CLK_ENABLE();
    
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = 0x10;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 16;
    RCC_OscInitStruct.PLL.PLLN = 400;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
      Error_Handler();
    }
    
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;  
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
    {
      Error_Handler();
    }
  }

  #ifdef  USE_FULL_ASSERT

  /**
    * @brief  Reports the name of the source file and the source line number
    *         where the assert_param error has occurred.
    * @param  file: pointer to the source file name
    * @param  line: assert_param error line source number
    * @retval None
    */
  void assert_failed(uint8_t* file, uint32_t line)
  {
    /* User can add his own implementation to report the file name and line number,
      ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1)
    {
    }
  }
  #endif

  /**
    * @}
    */ 

  /**
    * @}
    */ 
}