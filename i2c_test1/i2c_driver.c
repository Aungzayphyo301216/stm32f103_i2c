#include "i2c_driver.h"


//SET UP AND ACTIVATE I2C
//Initialization i2c
void init_i2c(char i2c,unsigned short speed_mode)
{
	RCC->APB2ENR |=1;  //Enable AFIO
	
	if (i2c==1)
	{
		RCC->APB1ENR |= 0x00200000;// enable i2c1 open clock for i2c bit no 21
		
		//initialize GPIO Pins
		GPIO_PinConfigure(GPIOB,6,GPIO_AF_OPENDRAIN,GPIO_MODE_OUT50MHZ); // Pin portB 6 for SCL 
		GPIO_PinConfigure(GPIOB,7,GPIO_AF_OPENDRAIN,GPIO_MODE_OUT50MHZ);  //Pin portB 7 for SDA
		
		//Peripheral under reset state
		I2C1->CR1 |= 0x8000; // for SWRST register
		I2C1->CR1 &= ~(0x8000);// Force reset again  SWRST Register
		
		I2C1->CR2 = 0x8; //put i2c frequency as 8Mhz at FREQ 
		//speed of the i2c CCR clock control register
		
		
		I2C1->CCR =speed_mode; 
		I2C1->TRISE=0x9; // (0x8+1)
		
		I2C1->CR1 |=1; // enable for PE register
 	}
	else if (i2c==2)
	{
		RCC->APB1ENR |= 0x00400000; //enable i2c2 open clock for i2c2 bit 22
		
		//initialize GPIO Pins
		GPIO_PinConfigure(GPIOB,10,GPIO_AF_OPENDRAIN,GPIO_MODE_OUT50MHZ); // Pin portB 10 for SCL 
		GPIO_PinConfigure(GPIOB,11,GPIO_AF_OPENDRAIN,GPIO_MODE_OUT50MHZ);  //Pin portB 11 for SDA
		
		//Peripheral under reset state
		I2C2->CR1 |= 0x8000; 
		I2C2->CR1 &= ~(0x8000);// Force reset again 
		
		I2C2->CR2 = 0x8; //put i2c frequency as 8Mhz at FREQ 
		//speed of the i2c CCR clock control register
		
		
		I2C2->CCR =speed_mode;
		I2C2->TRISE=0x9; // (0x8+1)
		
		I2C2->CR1 |=1; // enable for PE register
	}
 }

 //start condition
 void i2c_start(char i2c)
 {
		if (i2c==1)
		{
			I2C1->CR1 |= 0x0100; //enable for Start bit from CR1 register
			while (!(I2C1->SR1 &1)) 
			{};										//(i2c1->sr1 ==1) start genearted waitting state
		}
		else if (i2c==2)
		{
			I2C2->CR1 |= 0x0100; //enable for Start bit from CR1 register
			while (!(I2C2->SR1 &1))
			{};										//(i2c2->sr1 ==1)  start generated waitting state
		}
 }
 //Sending the address and Read /Write
 void i2c_address (char i2c, char address, char Read_Write)
 {
		volatile int tempo; // for data register ADDR
		if (i2c==1)
		{
			I2C1->DR =(address |= Read_Write);     // Sending address we also need READ or Write
			//while ((I2C1->SR1 &2)==0){}; // when ADDR =0 ,waiting state
				//cleared by reading SR1 register followed by reading SR2
			while (I2C1->SR1 &2)
			{
					tempo =I2C1->SR1;
					tempo =I2C1->SR2;
				if ((I2C1->SR1 &2)==0)
				{
					break;
				}
			}		
		}
		if (i2c==2)
		{
			I2C2->DR =(address |= Read_Write);     // Sending address we also need READ or Write
			//while ((I2C2->SR1 &2)==0){}; // when ADDR =0 ,waiting state
				//cleared by reading SR1 register followed by reading SR2
			while (I2C2->SR1 &2)
			{ 
					tempo =I2C2->SR1;
					tempo =I2C2->SR2;
				if ((I2C2->SR1 &2)==0)
				{
					break;
				}
			}		
		}
 }
 //Sending data
 
 void i2c_send_data (char i2c, char send_data)
 {
	if (i2c==1)
	{
		while ( (I2C1->SR1 &0x0080)==0){};
			I2C1->DR =send_data;
		while ( (I2C1->SR1 &0x0080)==0){};
	}
	if (i2c==2)
	{
		while ( (I2C2->SR1 &0x80)==1){};//edit 
			I2C2->DR =send_data;
		while ( (I2C2->SR1 &0x80)==1){};//
	}
 
 }
 //Stop conditions
 
 void i2c_stop(char i2c)
	 {
		 volatile int tempo;
			if (i2c==1)
			{
				tempo =I2C1->SR1;
				tempo =I2C1->SR2;	
				I2C1->CR1 |= 0x200;
			}
			else if (i2c==2)
			{
				tempo =I2C2->SR1;
				tempo =I2C2->SR2;	
				I2C2->CR1 |= 0x200;				
			}
 }
 
 // i2c_write Function
 void i2c_write (char i2c,char address,char send_data[])
 {
	 int i=0;
	 i2c_start(i2c);
	 i2c_address (i2c,address,0); 
	 while (send_data[i])
	 {
		 i2c_send_data (i2c,send_data[i]);  //send a lot of data 
		 i++;
	 }
	 i2c_stop(i2c);
 }
 
 
 //i2c receive data 
 void i2c_receive_data (char i2c,char receive_data)
 {
		if (i2c==1)
		{
			while ( (I2C1->SR1 &0x40)==0){};
			I2C1->DR =receive_data;
			while ( (I2C1->SR1 &0x40)==0){};
		}
		else if (i2c==2)
		{
			while ( (I2C2->SR1 &0x40)==1){};//edit 1==0
			I2C2->DR =receive_data;
			while ( (I2C2->SR1 &0x40)==1){};//edit 1==0
		}
	
 }
 
 //i2c_read function
 void i2c_read (char i2c,char address,char receive_data[])
 {
	 int j=0;
	 i2c_start(i2c);
	 i2c_address (i2c,address,1); 
	 while (receive_data[j])
	 {
		 i2c_receive_data (i2c,receive_data[j]);  //send a lot of data 
		 j++;
	 }
	 i2c_stop(i2c);
	 
 }