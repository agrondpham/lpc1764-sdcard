#include  "delay.h"
/*
*********************************************************************************************************
* Description: 	Delay function
* Arguments  : 	The time of ms
* Returns    : 	None
*********************************************************************************************************
*/
void delay_ms(unsigned int delay1msdata)
{
	unsigned int i,j,k;
	for(i=0;i<delay1msdata;i++)
		for(j=0;j<1000;j++)
			for(k=0;k<100;k++)
		;
}



