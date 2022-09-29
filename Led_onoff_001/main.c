#include "stm32f10x.h"
int main ()
{
	RCC->APB2ENR |=(1<<4 | 1<<5);
	GPIOC->CRL = 0x44444443; 
	GPIOD->CRL =0x44444443;
	while(1)
	{
		GPIOC->BSRR =1<<0;
		
		GPIOD->BSRR =1<<1;
	
	}
}
 
