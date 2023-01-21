
//PROJ_OBJECTIVE: LED LIGHT WHEN SESOR DETECT A MOTION,LED IS OFF WHEN THERE IS NO MOTION
//				  SUPPOSSE: IF MOTION_VALUE =1 ,THEN LED LIGHTH, IF MOTION_VALUE=0 THEN LED OFF
//MOTION_SENSOR_PART1
//HC_SR505 MOTION SENSOR DETECTOR
//DATA PIN OF SENSOR CONNECTED TO ADC1
//DATA-PIN->PA1

#include "main.h"
void SystemClock_Config(void);
void ADC1_INIT();
int ADC1_READ();
//led_init
//led_light
int long ADC1_VAL;
int main(void)
{

     HAL_Init();


     SystemClock_Config();


  while (1)
  {

     ADC1_VAL=ADC1_READ();
     //LED_LIGHT(ADC1_VAL);

  }

}

void ADC1_INIT()
{
	RCC->APB2ENR |=(1<<2); //GPIOA
	RCC->APB2ENR |=(1<<0); //AFIO
	RCC->APB2ENR |=(1<<9); //ENABLE ADC1
	//-----------
	GPIOA->CRL =0X44444404; //PA1 FOR ANALOG INPUT
	//
	RCC->CFGR &=~(3<<14);
	RCC->CFGR=(4<<14);
	//--
	//select channel
	ADC1->SQR1|=0X00000180;
	ADC1->SQR2|=0;
	ADC1->SQR3|=0X00000001;
	//--------
	ADC1->SMPR1|=(6<<0);//SAMPLING TIME SELECTION ADC1 ->A1

	//--
	ADC1->CR2 |=(7<<17); //ENABLE EXT EVENT
	ADC1->CR2 |=(1<<20); //ENABLE EXT TRIGGER
	ADC1->CR2 &=~(1<<11); //RIGHT ALLIGNMENT
	ADC1->CR2 |=(1<<3); //RESET CALIBRATION
	while(ADC1->CR2&(1<<3)); //wait until calibration is initialized
	ADC1->CR2 |=(1<<2); //ENABLE CALIBRATION
	while(ADC1->CR2&(1<<2)); //wait until calibration is completed
	ADC1->CR2 |=(1<<1); //CONTINOUS CONVERSION
	ADC1->CR2 |=(1<<0); //ENABLE ADC

}
int ADC1_READ()
{
	ADC1->CR2 |=(1<<22); //START CONVERSION
	while(!(ADC1->SR&(1<<1))); //WAIT UNTILL FLAG OF EOC=1
	ADC1_VAL=ADC1->DR; //READ CONVERSION OF DATA
	ADC1->SR &=~(1<<1); //RESET FLAGE
	return ADC1_VAL; //value converted
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
