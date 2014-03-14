/****************************************Copyright (c)**************************************************
**                               ShenZhen ZLG-MCU Development Co.,LTD.
**                                      graduate school
**                                 http://www.openmcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File Name: 			AT45DB161Drv.h
** Last modified Date: 	2007-04-25
** Last Version: 		1.0
** Descriptions: 		M45PE161µÄ²Ù×÷º¯Êý
**
**------------------------------------------------------------------------------------------------------
** Created 	 By: 		OpenMCU
** Created date: 		2010-12-23
** Version: 1.0
** Descriptions: 		First version
**
**------------------------------------------------------------------------------------------------------
** Modified by: 	
** Modified date: 	
** Version:
** Descriptions:  	
**
********************************************************************************************************/

#ifndef __F016BDRV_H 
#define __F016BDRV_H
#include "stdint.h"
#include "sys.h"


#define M45P_WriteEnable		0x06 
#define M45P_WriteDisable		0x04 
#define M45P_ReadStatusReg		0x05 
//#define M45P_WriteStatusReg		0x01 
#define M45P_ReadData			0x03 
#define M45P_FastReadData		0x0B 
//#define M45P_FastReadDual		0x3B 
#define M45P_PageProgram		0x02 
#define M45P_PageErase			0xDB 
#define M45P_SectorErase		0xD8   //0x20 
//#define M45P_ChipErase			0xC7 
#define M45P_PowerDown			0xB9 
#define M45P_ReleasePowerDown	0xAB 
//#define M45P_ManufactDeviceID	0x90 
#define M45P_JedecDeviceID		0x9F 

void  SPIx_Init(void);
void SPIx_SetSpeed(u8 SpeedSet);
void ssp0_init (void);
u8 ssp0_send (u8 outb);



#endif
