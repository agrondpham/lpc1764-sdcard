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

#ifndef __KHN_GSHT_H 
#define __KHN_GSHT_H
#include "spi.h"
#include "SD.h"
#include "Fatfs/diskio.h"
#include "Fatfs/ff.h"
extern FIL file;
void KHN_SDCARD_INIT();
int ReadData(char*,char*,char*);
int WriteData(int ,int ,int );
void Close(void);
#endif
