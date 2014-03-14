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

#ifndef _SD_H_
#define _SD_H_
#include "sys.h"
#include "stdint.h"	
#include "Fatfs/diskio.h"

//SD data status 
#define NO_RELEASE      0
#define RELEASE         1		 
// SD card type
#define SD_TYPE_MMC     0
#define SD_TYPE_V1      1
#define SD_TYPE_V2      2
#define SD_TYPE_V2HC    4	   
// SD card command  	   
#define CMD0    0       //����λ
#define CMD1    1
#define CMD9    9       //����9 ����CSD����
#define CMD10   10      //����10����CID����
#define CMD12   12      //����12��ֹͣ���ݴ���
#define CMD16   16      //����16������SectorSize Ӧ����0x00
#define CMD17   17      //����17����sector
#define CMD18   18      //����18����Multi sector
#define ACMD23  23      //����23�����ö�sectorд��ǰԤ�Ȳ���N��block
#define CMD24   24      //����24��дsector
#define CMD25   25      //����25��дMulti sector
#define ACMD41  41      //����41��Ӧ����0x00
#define CMD55   55      //����55��Ӧ����0x01
#define CMD58   58      //����58����OCR��Ϣ
#define CMD59   59      //����59��ʹ��/��ֹCRC��Ӧ����0x00
//����д���Ӧ������
#define MSD_DATA_OK                0x05
#define MSD_DATA_CRC_ERROR         0x0B
#define MSD_DATA_WRITE_ERROR       0x0D
#define MSD_DATA_OTHER_ERROR       0xFF
//SD����Ӧ�����
#define MSD_RESPONSE_NO_ERROR      0x00
#define MSD_IN_IDLE_STATE          0x01
#define MSD_ERASE_RESET            0x02
#define MSD_ILLEGAL_COMMAND        0x04
#define MSD_COM_CRC_ERROR          0x08
#define MSD_ERASE_SEQUENCE_ERROR   0x10
#define MSD_ADDRESS_ERROR          0x20
#define MSD_PARAMETER_ERROR        0x40
#define MSD_RESPONSE_FAILURE       0xFF
 							   						 	 
				    	  

extern u8  SD_Type;//SD��������
//���������� 
u8 SD_WaitReady(void);                          //�ȴ�SD������
u8 SD_SendCommand(u8 cmd, u32 arg, u8 crc);     //SD������һ������
u8 SD_SendCommand_NoDeassert(u8 cmd, u32 arg, u8 crc);
u8 SD_Init(void);                               //SD����ʼ��
u8 SD_Idle_Sta(void);                           //����SD��������ģʽ

u8 SD_ReceiveData(u8 *data, u16 len, u8 release);//SD��������

u8 SD_ReadSingleBlock(u32 sector, u8 *buffer);  //Read single block
u8 SD_WriteSingleBlock(u32 sector, const u8 *buffer); 		//Write single block
u8 SD_ReadMultiBlock(u32 sector, u8 *buffer, u8 count); 	//read multi Block
u8 SD_WriteMultiBlock(u32 sector, const u8 *data, u8 count);//write multi block
#endif




