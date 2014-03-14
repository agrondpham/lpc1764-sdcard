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
#include "sys.h"


void  SPI0_Init(void);
void SPI0_SetSpeed(u8 SpeedSet);
void ssp0_init (void);
u8 ssp0_send (u8 outb);



#endif
