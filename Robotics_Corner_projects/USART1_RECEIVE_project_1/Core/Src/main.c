//program :when USART1 in MCU RECIEVEs 'w' character open LED ON pc13
#include "main.h"

void SystemClock_Config(void);
//enable port and initialization led ON PORT-C-
void LED_INIT()
{
		RCC->APB2ENR=(1<<4);
		GPIOC->CRH=(3<<20);//CNF_OUTPUT:00 MOD:11->0011=3

}
///
void LED_ON()
{
		GPIOC->ODR=(1<<13);
}
void LED_OF()
{
		GPIOC->ODR &=~(1<<13);
}
#define FOSC 8000000
#define BAUD_RATE 9600
int main(void)
{
  void LED_INIT();
  void LED_ON();
  void LED_OF();
  LED_INIT();
  //ENABLE PORT-A FOR [PA9,PA10] THAT  USART(IN [MCU]) WILL TRANSIMT TX AND RECIEVE FROM BLUTOOTH OR USB
  RCC->APB2ENR|=(1<<2);
  //ENABLE USART_1->->AFIOEN ALTERNATE FUNCTION ENABLE ON BIT 0,
  RCC->APB2ENR|=(1<<0);
  //ENABLE CLOCK ON USART_1->USART1EN ON BIT 14
  RCC->APB2ENR|=(1<<14);

  //-----------------------------------------------
  //GENERAL PURPOSE INPUT OUPUT GPIOX-[CRL&CRH]
  //PA9->TX //OUTPUT ALTERNATE FUNCTION
  GPIOA->CRH=(0X0B<<4); //CNF 10 MOD 11->1011=B IN HEX
  //PA10->RX//INPUT
  GPIOA->CRH=(4<<8); //CNF 01 MOD 00->0100=4 IN HEX
  //MANEGING THE USART_1 SETTINGS(BRR,ENABLE TRANSMIT,ENABLE RECIEVE)
  //1-ENABLE UE BIT13
  USART1->CR1 |=(1<<13);
  //2-ENABLE TRANSMITION
  USART1->CR1 |=(1<<3);
  //3-ENABLE RECEIVE
  USART1->CR1 |=(1<<2);
//BAUD RATE REGISTER
  double USART_DIV=FOSC/BAUD_RATE;
  USART1->BRR=USART_DIV;
  //CHECK(STATUS OF RECIEVE)STATUS_REGISTER IF BIT=0 THEN RECIEVE DATA AND STORE IT IF BIT=1 THEN STILL IN THIS
  //DUMMY LOOP UNTILL IT IS EMPTY BIT=0 THEN READ THE BUFFER
  while (1)
    {
	  	  unsigned buffer_data=USART1->DR&0x00FF;//?????INITIALIZTION FOR THE STORE
	  	  while(!(USART1->SR&1<<5));
	  	  if(buffer_data=='w')
	  	  {

	  		  LED_ON();

           }
	  	  else
	  	  {
	  		  LED_OF();
	  	  }
    }

  HAL_Init();


  SystemClock_Config();



}


