
#include "fonts.h"
#include "ssd1306.h"
#include "main.h"
 ///key_pad
 GPIO_InitTypeDef GPIO_InitStructPrivate = {0};
 uint32_t previousMillis = 0;
 uint32_t currentMillis = 0;

///

I2C_HandleTypeDef hi2c1;
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);

int main(void)
{
	//call methods
	/////
	 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 1);
	 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);
	 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 1);
	 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 1);
 ///////////////
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_I2C1_Init();
	SSD1306_Init();
	SSD1306_GotoXY (0,0);
	SSD1306_Puts ("KEY-PAD", &Font_16x26, 1);
	SSD1306_UpdateScreen();
	HAL_Delay (1000);
	SSD1306_Clear();


	while (1)
	{



	}

}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}


static void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }


}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA4 PA5 PA6 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

  HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

}

/////////////////////
////////////////////


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  currentMillis = HAL_GetTick();
  if (currentMillis - previousMillis > 10) {
    /*Configure GPIO pins : PA0 PA1 PA2 PA3 to GPIO_INPUT*/
    GPIO_InitStructPrivate.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStructPrivate.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructPrivate.Pull = GPIO_NOPULL;
    GPIO_InitStructPrivate.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructPrivate);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 0);
    if(GPIO_Pin == GPIO_PIN_0 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)) //PIN-4,PIN-0 =key 1
    {
    	        SSD1306_Clear();
    	        SSD1306_GotoXY (0,0);
    		    SSD1306_Puts ("1", &Font_16x26, 1);
    		    SSD1306_UpdateScreen();



    }
    else if(GPIO_Pin == GPIO_PIN_1 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1))//PIN-4,PIN-1 =key 4
    {
    	SSD1306_Clear();
    	SSD1306_GotoXY (0,0);
    	SSD1306_Puts ("4", &Font_16x26, 1);
    	SSD1306_UpdateScreen();

    }
    else if(GPIO_Pin == GPIO_PIN_2 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2)) //PIN-4,PIN-2 key 7
    {
    	SSD1306_Clear();
    	SSD1306_GotoXY (0,0);
    	SSD1306_Puts ("7", &Font_16x26, 1);
    	SSD1306_UpdateScreen();

    }
    else if(GPIO_Pin == GPIO_PIN_3 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3)) //pin-4,pin-3 key *
    {

    	     SSD1306_Clear();
    	     SSD1306_GotoXY (0,0);
    	     SSD1306_Puts ("*", &Font_16x26, 1);
    	     SSD1306_UpdateScreen();

    }

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 0);
    if(GPIO_Pin == GPIO_PIN_0 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0))//pin-5,pin-0 key 2
    {
    	SSD1306_Clear();
    	SSD1306_GotoXY (0,0);
    	SSD1306_Puts ("2", &Font_16x26, 1);
    	SSD1306_UpdateScreen();
    }
    else if(GPIO_Pin == GPIO_PIN_1 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1))
    {

    	  SSD1306_Clear();
    	  SSD1306_GotoXY (0,0);
    	  SSD1306_Puts ("5", &Font_16x26, 1);
    	  SSD1306_UpdateScreen();
    }
    else if(GPIO_Pin == GPIO_PIN_2 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2))
    {

    	SSD1306_Clear();
    	SSD1306_GotoXY (0,0);
        SSD1306_Puts ("8", &Font_16x26, 1);
        SSD1306_UpdateScreen();
    }
    else if(GPIO_Pin == GPIO_PIN_3 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3))
    {
    	SSD1306_Clear();
    	SSD1306_GotoXY (0,0);
        SSD1306_Puts ("0", &Font_16x26, 1);
        SSD1306_UpdateScreen();
    }

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 0);
    if(GPIO_Pin == GPIO_PIN_0 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)) //pin-6 pin-0 key 3
    {
    	       SSD1306_Clear();
    	       SSD1306_GotoXY (0,0);
    	       SSD1306_Puts ("3", &Font_16x26, 1);
    	       SSD1306_UpdateScreen();
    }
    else if(GPIO_Pin == GPIO_PIN_1 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1))
    {
    	SSD1306_Clear();
    	SSD1306_GotoXY (0,0);
    	SSD1306_Puts ("6", &Font_16x26, 1);
    	SSD1306_UpdateScreen();
    }
    else if(GPIO_Pin == GPIO_PIN_2 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2))
    {
    	SSD1306_Clear();
    	SSD1306_GotoXY (0,0);
    	SSD1306_Puts ("9", &Font_16x26, 1);
    	SSD1306_UpdateScreen();
    }
    else if(GPIO_Pin == GPIO_PIN_3 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3))
    {
    	SSD1306_Clear();
    	SSD1306_GotoXY (0,0);
    	SSD1306_Puts ("#", &Font_16x26, 1);
    	SSD1306_UpdateScreen();
    }

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 1);
    if(GPIO_Pin == GPIO_PIN_0 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)) //pin-7 pin-0 key F1
    {
    	SSD1306_Clear();
    	SSD1306_GotoXY (0,0);
    	SSD1306_Puts ("F1", &Font_16x26, 1);
    	SSD1306_UpdateScreen();
    }
    else if(GPIO_Pin == GPIO_PIN_1 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1))
    {
    	SSD1306_Clear();
    	SSD1306_GotoXY (0,0);
    	SSD1306_Puts ("F2", &Font_16x26, 1);
    	SSD1306_UpdateScreen();
    }
    else if(GPIO_Pin == GPIO_PIN_2 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2))
    {
    	SSD1306_Clear();
    	SSD1306_GotoXY (0,0);
    	 SSD1306_Puts ("F3", &Font_16x26, 1);
    	 SSD1306_UpdateScreen();
    }
    else if(GPIO_Pin == GPIO_PIN_3 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3))
    {
    	SSD1306_Clear();
    	SSD1306_GotoXY (0,0);
    	SSD1306_Puts ("F4", &Font_16x26, 1);
    	SSD1306_UpdateScreen();
    }

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 1);
    /*Configure GPIO pins : PA0 PA1 PA2 PA3 back to EXTI*/
    GPIO_InitStructPrivate.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStructPrivate.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructPrivate);
    previousMillis = currentMillis;
  }
}

////////////////////////////
////////////////////////////
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
