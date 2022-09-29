
#include "i2c_driver.h"

//char send_data[2]={0x03,0x02};
char receive_data[2]={0x01,0x02};
void delayMs(unsigned long t)
{
	for (t=0;t<50000;t++);
}
int main (void)
	
{
	init_i2c(2,I2C_FM);
	
	while(1)
	{
		//i2c_write (2,0x78,send_data);
		i2c_read (2, 0x76, receive_data);
		delayMs(10);
	}
}
