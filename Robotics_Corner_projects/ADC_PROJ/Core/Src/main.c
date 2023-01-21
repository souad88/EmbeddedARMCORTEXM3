//questions : clock_function?????????????????????????/
//warning due to SPRINTF _function???????????????????????????????


//program: we have potentiometer ,we want to control the value of voltage that entered in
//        the MCU , show the changes of voltage_value in MCU on LED_lighting
#include "main.h"

void SystemClock_Config(void);
void LED_BLINKING(int long D); //function to change lightness of led
void ADC1_INIT(); //initialization of ADC1
int ADC1_READ(); //read analog signal input in ADC1 and convert to a digital signal
void ADC2_INIT(); //initialization of ADC2
int ADC2_READ(); //read analog signal input in ADC2 and convert it to a digital signal
int valx,valy;
char buffer[20];
int main(void)
{
	HAL_Init();
	SystemClock_Config();
	//ENABLE (PORT-C [LED ON PC13])  (PORT-A[PA1-PA2])  (ADC1[PA1])  (ADC2[PA2]) (AFIO)
	RCC->APB2ENR|=(1<<0)|(1<<2)|(1<<4)|(1<<9)|(1<<10);
	//PORTS_CONFIGURATION
    //LED
	GPIOC->CRH=0X00300000; //(3<<20)
    //ADC1
	ADC1_INIT();
	//ADC2
	ADC2_INIT();
	//OUTPUT ON THE PIN
    GPIOC->ODR=(1<<13);


  while (1)
  {
    valx=ADC1_READ(); //STORE ADC1 CONVERSONS IN VALX
    LED_BLINKING(valx); //PASSING ADC1 CONVERSIONS INTO LED_BLINKING FUNCTION
    valy=ADC2_READ();//STORE ADC2 CONVERSIONS IN VALY
   // sprintf(buffer,"%d,%d",valx,valy);
    LED_BLINKING(valy*3);
  }

}

//
void ADC1_INIT()
    {
	//config PA1,PA2 AS AN ANALOG INPUT MOD:00 CONF:00
	GPIOA->CRH |=0X44444004; //0x444444400

	//SET A CLOCK TO ADC1
	RCC->CFGR &=~(3<<14);//RESET IT
	RCC->CFGR=(2<<14); //PCLK2 DEVIDED BY 6
	//select channel
	ADC1->SQR1|=0X00000180;
	ADC1->SQR2|=0;
	ADC1->SQR3|=0X00000001;
	//SAMPLING TIME SELECTION ADC1 ->A1
	ADC1->SMPR1|=(6<<0);
	//SELECT SWSTART AS TRIGGER
	ADC1->CR2 |=(7<<17);
	//ENABLE EXTERNAL TRIGGER
	ADC1->CR2 |=(1<<20);
	//RIGHT ALIGNMENT
	ADC1->CR2 &=~(1<<11);
	//CONTINOUS CONVERSION
	ADC1->CR2 |=(1<<1);
	//ADC_ON
	ADC1->CR2 |=(1<<0);
	//CALIBRATION:-
	//Calibration significantly reduces accuracy errors due to internal capacitor bank variations
	//THE ADC CONVERSION CAN BE: simultaneous or sequential conversion modes:
	//SEQUENTIAL: ADC channels are converted one at a time:
	//A0 -> A1 -> A2 -> ... B0 -> B1 -> B2 -> ....

	//SIMULTANEOUS: ADC channels are converted in pairs:
	//A0,B0 -> A1,B1 -> A2,B2 -> ....
	//1-RESET CALIBRATION
	ADC1->CR2 |=(1<<3);
	//2-WAIT UNTILL RESET CALIBRATION FINISHED
	while(ADC1->CR2&(1<<3)); //still here if =1(start initializing ) ,if =0(initializing finished)
	ADC1->CR2 |=(1<<2); //START CALIBRATION IF =1
	while(ADC1->CR2&(1<<2)); //STILL HERE IF=1 CALIBRATION NOT FINISHED ,IF =0 CALIBRATION COMPLETED
}
void ADC2_INIT()
{
	//INPUT PA1 PA2->CONFIG &MODE-> CONF:00 ANALOG MODE , MODE :RESET 00
	GPIOA->CRL |=0X44444004; //(0<<0)|(0<<4)
	//SET CLOCK TO ADC2
	 //RESET CLOCK
	RCC->CFGR &=~(3<<14);
	//ENABLE ACLOCK FREQUENCY
	RCC->CFGR =(2<<14);
	//SELECT CHANNEL
	ADC2->SQR1|=0X00000180;
	ADC2->SQR2|=0;
	ADC2->SQR3|=0X00000001;
	//SAMPLING TIME SELECTION ADC2 ->A2
	ADC2->SMPR1 |=(6<<0);
	//SELECT SWSTART
	ADC2->CR2 |=(7<<17);
	//ENABLE EXTERNAL TRIGGER
	ADC2->CR2 |=(1<<20);
	//RIGHT ALLIGNMENT
	ADC2->CR2 &=~(1<<11);
	//CONTINOUS CONVERSION
	ADC2->CR2 |=(1<<1);
	//ADC ON
	ADC2->CR2 |=(1<<0);
	//RESET CALIBRATION
	ADC2->CR2 |=(1<<3);
	//WAIT UNTILL INITIALIZATION FINISHED
	while(ADC2->CR2&(1<<3));
	//START CALIBRATION
	ADC2->CR2 |=(1<<2);
	//WAIT UNTILL CALIBRATION FINISHED
	while(ADC2->CR2&(1<<2));

}
int ADC1_READ()
{
	 static int ADC1_VAL;
	//START SW CONVERSION
	 ADC1->CR2 |=(1<<22);
	//WAIT UNTILL EOC(END OF CONVERSION) FLAGE
	 while(!(ADC1->SR&(1<<1))); //polling
	ADC1_VAL=ADC1->DR;
	//RESET EOC FLAG
	ADC1->SR &=~(1<<1);
	//RETURN ADC_VALUE
	return ADC1_VAL;
}
int ADC2_READ()
{
	static int ADC2_VAL;
	//START SW CONVERSION
	ADC2->CR2 |=(1<<22);
	//WAIT UNTILL EOC FLAG
	while(!(ADC2->SR&(1<<1))); //still here if EOC=0
	ADC2_VAL=ADC2->DR;
	//RESET EOC FLAG
	ADC2->SR &=~(1<<1);
	//RETURN ADC2_VAL
	return ADC2_VAL;

}
void LED_BLINKING(int long D)
{	//dummy loop
	GPIOC->ODR &=~(1<<13);
	for(int i=0;D*i<30;i++);
	GPIOC->ODR |=(1<<13);
	for(int i=0; D*i<30; i++);
	GPIOC->ODR |=(1<<13);
}
//-------------------------------------
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

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
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
