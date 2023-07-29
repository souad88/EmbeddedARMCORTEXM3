
//kEYPAD 4*4 FUNCTIONS
#include "main.h"

  GPIO_InitTypeDef GPIO_InitStructPrivate = {0};
  uint32_t previousMillis = 0;
  uint32_t currentMillis = 0;
  uint8_t keyPressed = 0;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);

int main(void)
{


		//LEDS
			RCC->APB2ENR |=(1<<4);
			GPIOC->CRH    =(3<<20);
			GPIOC->CRH   |=(3<<24);
			GPIOC->CRH   |=(3<<28);

			//20-42-28
	////////////////////////

	HAL_Init();

  SystemClock_Config();


  MX_GPIO_Init();
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 1);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 1);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 1);
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

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

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
/////////////////////////
////////////////////////
///////////////////////
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
    if(GPIO_Pin == GPIO_PIN_0 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)) //PIN-0,PIN-4
    {
    			GPIOC->ODR =(1<<13);
    	//keyPressed = 1
    }
    else if(GPIO_Pin == GPIO_PIN_1 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1))//PIN-1,PIN-4
    {

    			GPIOC->ODR =(1<<14);
    	//keyPressed = 4
    }
    else if(GPIO_Pin == GPIO_PIN_2 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2)) //PIN-2,PIN-4
    {
    			GPIOC->ODR =(1<<15);
    	//keyPressed = 7
    }
    else if(GPIO_Pin == GPIO_PIN_3 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3))
    {
    			//keyPressed = 65; //ASCII value of A
    }

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 0);
    if(GPIO_Pin == GPIO_PIN_0 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0))
    {
    			//keyPressed = 35; //ASCII value of #
    }
    else if(GPIO_Pin == GPIO_PIN_1 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1))
    {
    			//keyPressed = 57; //ASCII value of 9
    }
    else if(GPIO_Pin == GPIO_PIN_2 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2))
    {
    			//keyPressed = 54; //ASCII value of 6
    }
    else if(GPIO_Pin == GPIO_PIN_3 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3))
    {
    			//keyPressed = 51; //ASCII value of 3
    }

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 0);
    if(GPIO_Pin == GPIO_PIN_0 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0))
    {
    			//keyPressed = 48; //ASCII value of 0
    }
    else if(GPIO_Pin == GPIO_PIN_1 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1))
    {
    			//keyPressed = 56; //ASCII value of 8
    }
    else if(GPIO_Pin == GPIO_PIN_2 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2))
    {
    			//keyPressed = 53; //ASCII value of 5
    }
    else if(GPIO_Pin == GPIO_PIN_3 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3))
    {
    			//keyPressed = 50; //ASCII value of 2
    }

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 1);
    if(GPIO_Pin == GPIO_PIN_0 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0))
    {
    			//keyPressed = 42; //ASCII value of *
    }
    else if(GPIO_Pin == GPIO_PIN_1 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1))
    {
    			//keyPressed = 55; //ASCII value of 7
    }
    else if(GPIO_Pin == GPIO_PIN_2 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2))
    {
    			//keyPressed = 52; //ASCII value of 4
    }
    else if(GPIO_Pin == GPIO_PIN_3 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3))
    {
    			//keyPressed = 49; //ASCII value of 1
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

//////////////////////
/////////////////////
////////////////////

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
