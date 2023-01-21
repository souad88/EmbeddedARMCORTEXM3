
#include "main.h"

void LED_INIT()
{	//LED ON P-C13
	RCC->APB2ENR|=(1<<4);
	GPIOC->CRH=0X00000300;//0011;
}

void LED_ON()
{
	GPIOC->ODR=(1<<13);
}
void LED_OF()
{
	GPIOC->ODR&=~(1<<13);
}
void SystemClock_Config(void);

# define FOSC 8000000
# define BAUD 9600
int main(void)
{
	void LED_INIT();
	void LED_ON();
	void LED_OF();
    LED_INIT();
    //ENABLE USART-PORT-PINS (PA9-PA10-ALTERNATIVE FUNCTION BIT-USART_1_PIN_CLOCK)
    RCC->APB2ENR |=(1<<2);//ENABLE PORT A FOR PA-9,PA-10
    RCC->APB2ENR |=(1<<0);//ENABLE ALTERNATE FUNCTION
    RCC->APB2ENR |=(1<14);//ENABLE USART_1 CLOCK
    GPIOA->CRH |=(0X0B<<4) ;//PA-9 TX OUTPUT 1011 =B IN HEX
    GPIOA->CRH |=(4<<8);//PA-10 RX INPUT 0100=4 IN HEX
    //CONTROL USART REGISTERS (CR1-CR2-CR3-SR-DR)
    //CR1
    USART1->CR1 |=(1<<13);//USART_BIT_ENABLE
    USART1->CR1 |=(1<<3);//ENABLE TRANSMIT
    USART1->CR1 |=(1<<2);//ENABLE RECEIVE
    //BAUD RATE
    double usart_BAUD_RATE=FOSC/BAUD;
    USART1->BRR=usart_BAUD_RATE;
    ////
    //define storage
    unsigned char buffer_data=USART1->DR&0X00FF;

    while (1)
    {

    		while(!(USART1->SR&1<<5)); //WHILE RECIEVER_BUFFER IS EMPTY READ DATA AND STORE IT
    		if(buffer_data=='w')
    			{
    				LED_ON();
    				USART1->DR=0XFF&'M'; //SEND CHAR 'M'
    				while(!(USART1->SR&1<<7)) ; //STILL IN THIS DUMMY LOOP WHILE TRANSMITION IS NOT COMPLETE
    			}
    		else
    			{

    				LED_OF();
    			}

    }


}






