/****************************************Copyright (c)**************************************************
**                               			Khanhhoi Co.,LTD.
**
**--------------File Info-------------------------------------------------------------------------------
** File Name: 			Spi.h
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

#ifndef __SPI_H 
#define __SPI_H
#include "stdint.h"

#define SPI_SPEED_2   0
#define SPI_SPEED_4   1
#define SPI_SPEED_8   2
#define SPI_SPEED_16  3
#define SPI_SPEED_256 4
#define   SD_CS(x)      ((x) ? (LPC_GPIO1->FIOSET = (1<<21)) : (LPC_GPIO1->FIOCLR = (1<<21)))
void  SPI0_Init(void);
void SPI0_SetSpeed(uint8_t SpeedSet);
void ssp0_init (void);
uint8_t ssp0_send (uint8_t outb);
void spi0_on();
void spi0_off();


#endif
