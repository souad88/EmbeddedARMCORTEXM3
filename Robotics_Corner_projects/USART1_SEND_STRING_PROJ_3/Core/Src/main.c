
#include "main.h"


#define  FOS 8000000
#define  BAUD 9600


void LED_INIT()
	{
		RCC->APB2ENR|=(1<<4); //ENABLE PORT C
		GPIOC->CRH=0X00000300;

	}
void LED_ON()
	{
		GPIOC->ODR=(1<<13);
	}
void LED_OF()
	{
		GPIOC->ODR&=~(1<<13);
	}
int main(void)
{

	    void LED_INIT();
	    void LED_ON();
	    void LED_OF();
	    void store_char_(char);
	    void send_string(char*);
	    LED_INIT();
	    RCC->APB2ENR |=(1<<14);
	    RCC->APB2ENR |=(1<<2);
	    RCC->APB2ENR |=(1<<0);

	    GPIOA->CRH=(0X0B<<4);
	    GPIOA->CRH=(4<<8);

	    USART1->CR1 |=(1<<13);
	    USART1->CR1 |=(1<<2);
	    USART1->CR1 |=(1<<3);

	    double USART_BAUD_RATE=FOS/BAUD;
	    USART1->BRR = USART_BAUD_RATE;
	    unsigned char buffer_d=USART1->DR&0x00FF;

	    while (1)
			{
	    			while(!(USART1->SR&1<<5));
	    			if(buffer_d=='w')
	    				{

	    					LED_ON();
	    					send_string("LED IS ON");
	    				}
	    			else
	    				{
	    					LED_OF();
	    				}

	    		}




	}

void store_char_(char data)
{
	USART1->DR=data&0xFF;
	while(!(USART1->SR & 1<<7));

}
void send_string(char *data)
{
	int i=0;
	while(data[i]!='\0') //+i if data not equal null
	{
      store_char_(data[i]);
	//USART1->DR=data&0xFF;
	//while(!(USART1->SR & 1<<7));
	  i++;
	}

}
