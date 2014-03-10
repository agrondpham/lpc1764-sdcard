#include "lpc17xx.h"
#include "ssp0.h"
#include "sys.h"
/************************************************************************																										
** 函数功能: SPI0初始化		
** 入口参数: 无    		
** 出口参数: 无		
** 备    注: 速度为72M/250
************************************************************************/ 
void ssp0_init (void) 
{
  /* Initialize and enable the SSP Interface module. */

  LPC_SC->PCONP       |= (1 << 21);           /* Enable power to SSPI0 block */

  /* SSEL is GPIO, output set to high. */
  LPC_GPIO1->FIODIR   |=  (1<<21);            /* P1.21 is output             */
  LPC_GPIO1->FIOPIN   |=  (1<<21);            /* set P1.21 high (SSEL inact.)*/
  LPC_PINCON->PINSEL3 &= ~(0<<10);             /* P1.21 SSEL (used as GPIO)   */
 
  /* P3.26 is SD Card Power Supply Enable Pin */
  LPC_GPIO3->FIODIR   |=  (1<<26);            /* P3.26 is output             */
  LPC_GPIO3->FIOPIN   &= ~(1<<26);            /* set P3.26 low(enable power) */

  /* SCK, MISO, MOSI are SSP pins. */
  LPC_PINCON->PINSEL3 &= ~(3<<8);              /* P1.20 cleared               */
  LPC_PINCON->PINSEL3 |=  (3<<8);              /* P1.20 SCK0                  */
  LPC_PINCON->PINSEL3 &= ~((3<<14) | (3<<16));  /* P1.23, P1.24 cleared        */
  LPC_PINCON->PINSEL3 |=  ((3<<14) | (3<<16));  /* P1.23 MISO0, P1.24 MOSI0    */

  LPC_SC->PCLKSEL1 &= ~(3<<10);               /* PCLKSP0 = CCLK/4 ( 25MHz)   */
  LPC_SC->PCLKSEL1 |=  (1<<10);               /* PCLKSP0 = CCLK   (100MHz)   */

  LPC_SSP0->CPSR = 250;                       /* 72MHz / 250 = 400kBit      */
                                              /* maximum of 18MHz is possible*/    
  LPC_SSP0->CR0  = 0x0007;                    /* 8Bit, CPOL=0, CPHA=0        */
  LPC_SSP0->CR1  = 0x0002;                    /* SSP0 enable, master         */
}
/************************************************************************																										
** 函数功能: 设置SPI0的速度		
** 入口参数: str1 ，str2  :字符串 	    		
** 出口参数: 无		
** 备    注: SPI_SPEED_2   2分频   (SPI 36M@sys 72M)
**			 SPI_SPEED_4   4分频   (SPI 18M@sys 72M)
**			 SPI_SPEED_8   8分频   (SPI 9M@sys 72M)
**			 SPI_SPEED_16  16分频  (SPI 4.5M@sys 72M)
**			 SPI_SPEED_256 256分频 (SPI 281.25K@sys 72M)
************************************************************************/ 

void SPIx_SetSpeed(u8 SpeedSet)
{
	   	LPC_SPI->SPCCR =	0xFE;			//Fsck=Fcpu/128*2
	switch(SpeedSet)
	{
		case SPI_SPEED_2://8分频
			LPC_SSP0->CPSR = 2;//Fsck=Fpclk/2=36Mhz
			break;
		case SPI_SPEED_4://8分频
			LPC_SSP0->CPSR = 4;//Fsck=Fpclk/4=18Mhz
			break;
		case SPI_SPEED_8://八分频
			LPC_SSP0->CPSR = 8;//Fsck=Fpclk/8=9Mhz
			break;
		case SPI_SPEED_16://十六分频
			LPC_SSP0->CPSR = 16;//Fsck=Fpclk/16=4.5Mhz
			break;
		case SPI_SPEED_256://256分频
			LPC_SSP0->CPSR = 250;//Fsck=Fpclk/256=281.25Khz
			break;
	}		 	  
}
/************************************************************************																										
** 函数功能: 发送数据（SD卡）		
** 入口参数: outb 要发送的数据 	    		
** 出口参数: 无		
** 备    注: 无
************************************************************************/ 
u8 ssp0_send (u8 outb) 
{
  /* Write and Read a byte on SPI interface. */

  LPC_SSP0->DR = outb;
  while (LPC_SSP0->SR & 0x10);                 /* Wait for transfer to finish */
  return (LPC_SSP0->DR);                      /* Return received value       */
}










