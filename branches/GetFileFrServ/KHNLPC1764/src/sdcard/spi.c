/****************************************Copyright (c)**************************************************
**                               			Khanhhoi Co.,LTD.
**
**--------------File Info-------------------------------------------------------------------------------
** File Name: 			Spi.c
** Last modified Date: 	14-03-2014
** Last Version: 		1.1
** Descriptions: 		SPI for sdcard in board L32LPC
**
**------------------------------------------------------------------------------------------------------
** Created 	 By: 		Agrond
** Created date: 		14-03-2014
** Version: 				1.0
** Descriptions: 		First version
**
**------------------------------------------------------------------------------------------------------
** Modified by: 		
** Modified date: 	
** Version:					
** Descriptions:  	
**
********************************************************************************************************/
#include "LPC17XX.H"
#include "SPI.H"

/* Private typedef -----------------------------------------------------------*/
#define SPI_FLASH_PageSize    0x210

/* Private define ------------------------------------------------------------*/
#define READ       0xD2  /* Read from Memory instruction */
#define WRITE      0x82  /* Write to Memory instruction */

#define RDID       0x9F  /* Read identification */
#define RDSR       0xD7  /* Read Status Register instruction  */

#define SE         0x7C  /* Sector Erase instruction */
#define PE         0x81  /* Page Erase instruction */

#define RDY_Flag   0x80  /* Ready/busy(1/0) status flag */

#define Dummy_Byte 0xA5

void delay_us(int count)
{
  while(count--);
}

/*******************************************************************************************															
** Init SPI interface				
** Input: void																
** Output: void
** 															
*******************************************************************************************/
void  SPI0_Init(void)
{  
	/* */
//	LPC_PINCON->PINSEL0 	=	0;
   	LPC_GPIO0->FIODIR2 	|=  	1<<0;
	/* MAP pinsel	*/
   	LPC_PINCON->PINSEL0	|= 	0xc0000000;				// P0.15 SCK
   	//LPC_PINCON->PINSEL1	 = (0x03 << 2) | (0x03 << 4);	// P0.17|P0.18

   	LPC_SPI->SPCCR =	0x40;		       		// 	72MHz/64*4

 	LPC_SPI->SPCR  = 	(0 << 2) |				// SPI
 				(0 << 3) |				// CPHA = 0,
 				(0 << 4) |				// CPOL = 0, SCK
 				(1 << 5) |				// MSTR = 1, SPI
 				(0 << 6) |				// LSBF = 0, SPI
 				(0 << 7);				// SPIE = 0, SPI
}
/************************************************************************																										
** Description: Set speed of SPI	
** input: str1:speed 	    		
** output: void		
** Note: SPI_SPEED_2   2   (SPI 36M@sys 72M)
**			 SPI_SPEED_4   4   (SPI 18M@sys 72M)
**			 SPI_SPEED_8   8   (SPI 9M@sys 72M)
**			 SPI_SPEED_16  16  (SPI 4.5M@sys 72M)
**			 SPI_SPEED_256 256 (SPI 281.25K@sys 72M)
************************************************************************/ 

void SPI0_SetSpeed(uint8_t SpeedSet)
{
	   	LPC_SPI->SPCCR =	0xFE;			//Fsck=Fcpu/128*2
	switch(SpeedSet)
	{
		case SPI_SPEED_2://8
			LPC_SSP0->CPSR = 2;//Fsck=Fpclk/2=36Mhz
			break;
		case SPI_SPEED_4://8
			LPC_SSP0->CPSR = 4;//Fsck=Fpclk/4=18Mhz
			break;
		case SPI_SPEED_8://
			LPC_SSP0->CPSR = 8;//Fsck=Fpclk/8=9Mhz
			break;
		case SPI_SPEED_16://
			LPC_SSP0->CPSR = 16;//Fsck=Fpclk/16=4.5Mhz
			break;
		case SPI_SPEED_256://256
			LPC_SSP0->CPSR = 250;//Fsck=Fpclk/256=281.25Khz
			break;
	}		 	  
}
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
  //LPC_GPIO3->FIOPIN   &= ~(1<<26);            /* set P3.26 low(enable power) */

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
uint8_t ssp0_send (uint8_t outb)
{
  /* Write and Read a byte on SPI interface. */

  LPC_SSP0->DR = outb;
  while (LPC_SSP0->SR & 0x10);                 /* Wait for transfer to finish */
  return (LPC_SSP0->DR);                      /* Return received value       */
}
void spi0_on(){
	LPC_GPIO3->FIOSET = (1<<26);
	//LPC_GPIO3->FIOPIN   &= ~(1<<26);
}
void spi0_off(){
	LPC_GPIO3->FIOCLR = (1<<26);
}
