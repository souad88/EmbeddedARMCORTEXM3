//project: send a char from mobile application to a Bluetooth card wireless to open leds
//leds or closed it
//the Bluetooth card is connected to the ARM32F103_ via USART_1
//data be sent and received :[ TX: PA9],[ RX: PA10]
// 'o' :open led pb11
// 'c' :close led pb11
// 'a' :open all leds pc13,pc14,pc15
// other_chars : close all leds
//display the event on OLED
#include "main.h"
#include "fonts.h"
#include "ssd1306.h"

//METHODS DEF:
//
I2C_HandleTypeDef hi2c1;
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
//
//1-leds
void LEDS_INIT();
void LED_PB11_ON();
void LED_PB11_OFF();
void LEDS_ON();
void LEDS_OFF();
//2-USART1
void send_string_from_usart(char *); //send a string to this func
void send_char_DR(char); //send char to data register of usart1
#define FOSC  8000000
#define BAUD_RATE 9600

int main(void)
{   //INITIALIZATION
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_I2C1_Init();
    LEDS_INIT();
    SSD1306_Init();

    double USART_DIV=FOSC/BAUD_RATE;
         //RCC&GPIO
    RCC->APB2ENR |=(1<<2);   //ENABLE PORT A
    RCC->APB2ENR |=(1<<0);	  //AIFO
    RCC->APB2ENR |=(1<<14);  //USART1
    GPIOA->CRH   |=(0X0B<<4); //TX PA9 O/P
    GPIOA->CRH   |=(4<<8);    //RX PA10 I/P
         //USART1_REGS
    USART1->CR1 |=(1<<13); //ENABLE UE BIT 13
    USART1->CR1 |=(1<<3); //ENABLE TRANSMITTER
    USART1->CR1 |=(1<<2); //ENABLE RECEIVER
    USART1->BRR=USART_DIV;//BAUD RATE
         //MAIN CODE

    while (1)
      {

        unsigned char buffer=USART1->DR&0x00FF;
        while(!(USART1->SR&1<<5));
        if (buffer=='o')
        {

     	 	 send_string_from_usart("LED ON PB11 IS OPENED");
    	   	 LED_PB11_ON();
    	     //DISPLAY PB11-OPENED
    	     SSD1306_Clear();
    	   	 SSD1306_GotoXY (0,0);
    	     SSD1306_Puts ("LED ON PB11", &Font_11x18, 1);
    	     SSD1306_GotoXY (0,30);
       	     SSD1306_Puts ("IS OPENED", &Font_11x18,1);
    	     SSD1306_UpdateScreen();

    	  }

    	 else if(buffer=='c')
    	  {

    	    	 		 //DISPLAY PB11-CLOSED
    	    	 		SSD1306_Clear();
    	    	 	    SSD1306_GotoXY (0,0);
    	    	        SSD1306_Puts ("LED ON PB11", &Font_11x18, 1);
    	    	 	    SSD1306_GotoXY (0,30);
    	    		    SSD1306_Puts ("IS CLOSED", &Font_11x18,1);
    	    	 	    SSD1306_UpdateScreen();
    	    	 	    //usart
    	    	 	    send_string_from_usart("LED ON PB11 IS CLOSED");
    	    	 	    LED_PB11_OFF();


      	 	 }

    	  else if(buffer== 'a')
    	   	 {
    		      //DISPLAY LEDS ARE OPENED
    		         SSD1306_Clear();
    		         SSD1306_GotoXY (0,0);
    		         SSD1306_Puts ("ALL LEDS", &Font_11x18, 1);
    		         SSD1306_GotoXY (0,30);
    		         SSD1306_Puts ("ARE OPENED", &Font_11x18,1);
    		         SSD1306_UpdateScreen();
    		         //usart
         	 		 send_string_from_usart("ALL LEDS OPENED");
        	 		 LEDS_ON();
    	 	 }

    	  else
      	 	 {
    		     //DISPLAY LEDS ARE CLOSED
    		      SSD1306_Clear();
    		      SSD1306_GotoXY (0,0);
    		      SSD1306_Puts ("ALL LEDS", &Font_11x18, 1);
    		      SSD1306_GotoXY (0,30);
    		      SSD1306_Puts ("ARE CLOSED", &Font_11x18,1);
    		      SSD1306_UpdateScreen();
                 //usart
    	   		 send_string_from_usart("ALL LEDS CLOSED");
    	    	 LEDS_OFF();

       	 	 }

      }

}



void LEDS_INIT()
{
	 RCC->APB2ENR  |=(1<<3); //PORT_B
     RCC->APB2ENR  |=(1<<4); //PORT_C
     GPIOB->CRH     =(3<<12);// CRH ON PBB11

	 GPIOC->CRH     =(3<<20);//CRH IN PC13
	 GPIOC->CRH    |=(3<<24);//CRH IN PC14 TRUE
	 GPIOC->CRH    |=(3<<28);//CRH IN PC15
}
void LED_PB11_ON()
{
	GPIOB->ODR =(1<<11);
}
void LED_PB11_OFF()
{
	GPIOB->ODR &=~(1<<11);


}
void LEDS_ON()
{
				  GPIOC->ODR =(1<<13);
				  GPIOC->ODR |=(1<<14);
				  GPIOC->ODR |=(1<<15);
}
void LEDS_OFF()
{

		GPIOC->ODR &=~(1<<13);
	    GPIOC->ODR &=~(1<<14);
		GPIOC->ODR &=~(1<<15);


}


void send_string_from_usart(char *p)
{
	int j=0;
	while(p[j]!='\0')
	{
	  send_char_DR(p[j]);
	  j++;
	}

}


void send_char_DR(char p)
{
	USART1->DR=p&0xFF;
	while (!(USART1->SR&1<<7));
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

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
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
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

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
