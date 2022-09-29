#include "stm32f10x.h"
#include "GPIO_STM32F10x.h"


#define I2C_FM 0x2d  // clock decimal 45 this will be standard mode std mode is 100kb/s
#define I2C_SM 0xB4 //  clock decimal 180 this will be Fast Mode is 400kb/s


void init_i2c(char i2c,unsigned short speed_mode);

void i2c_write (char i2c,char address,char send_data[]);

 void i2c_read (char i2c,char address,char receive_data[]);
