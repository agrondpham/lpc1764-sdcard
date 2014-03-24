/****************************************Copyright (c)**************************************************
**                               			Khanhhoi Co.,LTD.
**
**--------------File Info-------------------------------------------------------------------------------
** File Name: 			Sd.c
** Last modified Date: 	14-03-2014
** Last Version: 		1.1
** Descriptions: 		sdcard low level library in board L32LPC
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
#include "spi.h"
#include "sd.h"
#include "lpc17xx.h"
#include "fatfs/diskio.h"
uint8_t  SD_Type=0;				//SD card type

/* RTC timer strcuture */
typedef struct {
	uint8_t     sec;     /* Second value - [0,59] */
	uint8_t     min;     /* Minute value - [0,59] */
	uint8_t     hour;    /* Hour value - [0,23] */
	uint8_t     mday;    /* Day of the month value - [1,31] */
	uint8_t     wday;    /* Day of week value - [0,6] */
	uint16_t    yday;    /* Day of year value - [1,365] */
	uint8_t     mon;     /* Month value - [1,12] */
	uint16_t    year;    /* Year value - [0,4095] */
} RTCTime;

static volatile DSTATUS Stat = STA_NOINIT;	/* Physical drive status */
/*-----------------------------------------------------------------------*/
/* Initialize disk drive                                                 */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE drv		/* Physical drive number (0) */
)
{
	if (drv) return STA_NOINIT;			/* Supports only drive 0 */
	SD_Init();
	if (SD_Type== 0x00) {	/* Failed */		
		Stat = STA_NOINIT;
	} else {			/* OK */
		Stat &= ~STA_NOINIT;	/* Clear STA_NOINIT flag */
	}
	
	return Stat;
}
DRESULT disk_ioctl (
BYTE drv,		/* Physical drive number (0) */
BYTE ctrl,		/* Control code */
void *buff		/* Buffer to send/receive control data */
)
{
DRESULT res;
	//BYTE n, *ptr = buff;

	if (drv) return RES_PARERR;
	if (Stat & STA_NOINIT) return RES_NOTRDY;

	res = RES_ERROR;

	switch (ctrl) {
	case CTRL_SYNC :		/* Make sure that no pending write process */
        //SD_Select();
//		if (SD_WaitForReady() == SD_TRUE)
			res = RES_OK;
		break;

	case GET_SECTOR_COUNT :	/* Get number of sectors on the disk (DWORD) */
//		*(DWORD*)buff = CardConfig.sectorcnt;
		res = RES_OK;
		break;

	case GET_SECTOR_SIZE :	/* Get R/W sector size (WORD) */
		*(WORD*)buff = 512;//CardConfig.sectorsize;
		res = RES_OK;
		break;

	case GET_BLOCK_SIZE :	/* Get erase block size in unit of sector (DWORD) */
//		*(DWORD*)buff = CardConfig.blocksize;
		res = RES_OK;
		break;

	case MMC_GET_TYPE :		/* Get card type flags (1 byte) */
//		*ptr = CardType;
		res = RES_OK;
		break;

	case MMC_GET_CSD :		/* Receive CSD as a data block (16 bytes) */
//		for (n=0;n<16;n++)
//			*(ptr+n) = CardConfig.csd[n]; 
		res = RES_OK;
		break;

	case MMC_GET_CID :		/* Receive CID as a data block (16 bytes) */
//		for (n=0;n<16;n++)
//			*(ptr+n) = CardConfig.cid[n];
		res = RES_OK;
		break;

	case MMC_GET_OCR :		/* Receive OCR as an R3 resp (4 bytes) */
//		for (n=0;n<4;n++)
//			*(ptr+n) = CardConfig.ocr[n];
		res = RES_OK;
		break;

	case MMC_GET_SDSTAT :	/* Receive SD status as a data block (64 bytes) */
//		for (n=0;n<64;n++)
//            *(ptr+n) = CardConfig.status[n]; 
        res = RES_OK;   
		break;

	default:
		res = RES_PARERR;
	}

    //SD_DeSelect();

	return res;
}
/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/
DRESULT disk_read (
BYTE drv,			/* Physical drive number (0) */
BYTE *buff,			/* Pointer to the data buffer to store read data */
DWORD sector,		/* Start sector number (LBA) */
UINT count			/* Sector count (1..255) */
)
{
	if(count>1){
	}
		if(SD_ReadSingleBlock(sector,buff)==0)return RES_OK;
		return RES_ERROR;
	
		
}

/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/
DSTATUS disk_status (
BYTE drv		/* Physical drive number (0) */
)
{
if (drv) return STA_NOINIT;		/* Supports only single drive */

return Stat;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
BYTE drv,			/* Physical drive number (0) */
const BYTE *buff,	/* Pointer to the data to be written */
DWORD sector,		/* Start sector number (LBA) */
UINT count			/* Sector count (1..255) */
)
{
if (drv || !count) return RES_PARERR;
if (Stat & STA_NOINIT) return RES_NOTRDY;

	if ( SD_WriteSingleBlock(sector, buff) == 0)
		return RES_OK;
	else
		return 	RES_ERROR;

}
/*---------------------------------------------------------*/
/* User Provided Timer Function for FatFs module           */
/*---------------------------------------------------------*/
/* This is a real time clock service to be called from     */
/* FatFs module. Any valid time must be returned even if   */
/* the system does not support a real time clock.          */
/* This is not required in read-only configuration.        */
/* Get RTC timer value */
void rtc_gettime( RTCTime *rtc ) 
{
	rtc->sec    = LPC_RTC->SEC;
	rtc->min    = LPC_RTC->MIN;
	rtc->hour   = LPC_RTC->HOUR;
	rtc->mday   = LPC_RTC->DOM;
	rtc->wday   = LPC_RTC->DOW;
	rtc->yday   = LPC_RTC->DOY;
	rtc->mon    = LPC_RTC->MONTH;
	rtc->year   = LPC_RTC->YEAR;   
}
DWORD get_fattime ()
{
	RTCTime rtc;

	/* Get local time */
	//LPC17xx_RTC_GetTime( rtc )
	rtc_gettime(&rtc);

	/* Pack date and time into a DWORD variable */
	return	  ((DWORD)(rtc.year - 1980) << 25)
			| ((DWORD)rtc.mon << 21)
			| ((DWORD)rtc.mday << 16)
			| ((DWORD)rtc.hour << 11)
			| ((DWORD)rtc.min << 5)
			| ((DWORD)rtc.sec >> 1);

}


///*-----------------------------------------------------------------------*/
///* Device timer function  (Platform dependent)                           */
///*-----------------------------------------------------------------------*/
///* This function must be called from timer interrupt routine in period
///  of 10 ms to generate card control timing.
//*/
//void disk_timerproc (void)
//{
//	    WORD n;

//	n = Timer1;						/* 100Hz decrement timer stopped at 0 */
//	if (n) Timer1 = --n;
//	n = Timer2;
//	if (n) Timer2 = --n;   				
//}

uint8_t SD_GetResponse(uint8_t Response)
{
	uint16_t Count=0xFFF;//�ȴ����
	while ((ssp0_send(0XFF)!=Response)&&Count)Count--;//�ȴ�õ�׼ȷ�Ļ�Ӧ  	  
	if (Count==0)return MSD_RESPONSE_FAILURE;//�õ���Ӧʧ��   
	else return MSD_RESPONSE_NO_ERROR;//��ȷ��Ӧ
}

uint8_t SD_WaitDataReady(void)
{
    uint8_t r1=MSD_DATA_OTHER_ERROR;
    uint32_t retry;
    retry=0;
    do
    {
        r1=ssp0_send(0xFF)&0X1F;//������Ӧ
        if(retry==0xfffe)return 1; 
		retry++;
		switch (r1)
		{					   
			case MSD_DATA_OK://��ݽ�����ȷ��	 
				r1=MSD_DATA_OK;
				break;  
			case MSD_DATA_CRC_ERROR:  //CRCУ�����
				return MSD_DATA_CRC_ERROR;  
			case MSD_DATA_WRITE_ERROR://���д�����
				return MSD_DATA_WRITE_ERROR;  
			default://δ֪����    
				r1=MSD_DATA_OTHER_ERROR;
				break;	 
		}   
    }while(r1==MSD_DATA_OTHER_ERROR); //��ݴ���ʱһֱ�ȴ�
	retry=0;
	while(ssp0_send(0XFF)==0)//�������Ϊ0,����ݻ�δд���
	{
		retry++;
		//delay_us(10);//SD��д�ȴ���Ҫ�ϳ���ʱ��
		if(retry>=0XFFFFFFFE)return 0XFF;//�ȴ�ʧ����
	};	    
    return 0;//�ɹ���
}

/************************************************************************																										
** Description: Send command to sdcard
** Input: uint8_t cmd   Command code
**			 uint32_t arg
**	    	 uint8_t crc   Check sum
** Ouput: return value
************************************************************************/													  
uint8_t SD_SendCommand(uint8_t cmd, uint32_t arg, uint8_t crc)
{
    uint8_t r1;
	uint8_t Retry=0;
	SD_CS(1);
    ssp0_send(0xff);//����д������ʱ
	ssp0_send(0xff);     
 	ssp0_send(0xff);  	 
    //Ƭѡ���õͣ�ѡ��SD��
    SD_CS(0); 
    //����
    ssp0_send(cmd | 0x40);//�ֱ�д������
    ssp0_send(arg >> 24);
    ssp0_send(arg >> 16);
    ssp0_send(arg >> 8);
    ssp0_send(arg);
    ssp0_send(crc); 
    //�ȴ���Ӧ����ʱ�˳�
    while((r1=ssp0_send(0xFF))==0xFF)
    {
        Retry++;	    
        if(Retry>200)break; 
    }   
    //�ر�Ƭѡ
    SD_CS(1);
    //�������϶�������8��ʱ�ӣ���SD�����ʣ�µĹ���
    ssp0_send(0xFF);
    //����״ֵ̬
    return r1;
}
uint8_t SD_SendCommand_NoDeassert(uint8_t cmd, uint32_t arg, uint8_t crc)
{
	uint8_t Retry=0;
	uint8_t r1;
    ssp0_send(0xff);//����д������ʱ
	ssp0_send(0xff);  	 	 
    SD_CS(0);//Ƭѡ���õͣ�ѡ��SD��	   
    //����
    ssp0_send(cmd | 0x40); //�ֱ�д������
    ssp0_send(arg >> 24);
    ssp0_send(arg >> 16);
    ssp0_send(arg >> 8);
    ssp0_send(arg);
    ssp0_send(crc);   
    //�ȴ���Ӧ����ʱ�˳�
    while((r1=ssp0_send(0xFF))==0xFF)
    {
        Retry++;	    
        if(Retry>200)break; 
    }  	  
    //������Ӧֵ
    return r1;
}
uint8_t SD_Idle_Sta(void)
{
	uint16_t i;
	uint8_t retry;
    for(i=0;i<0xf00;i++);//����ʱ���ȴ�SD���ϵ����	 
    //�Ȳ���>74�����壬��SD���Լ���ʼ�����
    for(i=0;i<10;i++)ssp0_send(0xFF); 
    //-----------------SD����λ��idle��ʼ-----------------
    //ѭ��������CMD0��ֱ��SD������0x01,����IDLE״̬
    //��ʱ��ֱ���˳�
    retry = 0;
    do
    {	   
        //����CMD0����SD������IDLE״̬
        i = SD_SendCommand(CMD0, 0, 0x95);
        retry++;
    }while((i!=0x01)&&(retry<200));
    //���ѭ���󣬼��ԭ�򣺳�ʼ���ɹ���or ���Գ�ʱ��
    if(retry==200)return 1; //ʧ��
	return 0;//�ɹ�	 						  
}
/************************************************************************																										
** Description: Init SD card. make sd card to spi mode
** Output: 0:NO_ERR
**			 1:TIME_OUT
**			 99:NO_CARD
************************************************************************/    																 
uint8_t SD_Init(void)
{								 
    uint8_t r1;     //return value is card type by bit
    uint16_t retry;
    uint8_t buff[6];
	ssp0_init();//Init SSP0
 	SPI0_SetSpeed(SPI_SPEED_256);//Set spi speec 256 
	SD_CS(1);	
  if(SD_Idle_Sta()) return 1;//Card fail can not read	  
 	SD_CS(0);	
	r1 = SD_SendCommand_NoDeassert(8, 0x1aa,0x87);	     
    //���Ƭ�汾��Ϣ��v1.0�汾�ģ���r1=0x05����������³�ʼ��
    if(r1 == 0x05)
    {
        //Set sdtype to type v1
        SD_Type = SD_TYPE_V1;	   

        SD_CS(1);
        ssp0_send(0xFF);	  
        retry = 0;
        do
        {
					//Send command CMD55
            r1 = SD_SendCommand(CMD55, 0, 0);
            if(r1 == 0XFF)return r1;//return error 
          //Send commnad ACMD41  
					r1 = SD_SendCommand(ACMD41, 0, 0);
            retry++;
        }while((r1!=0x00) && (retry<400));
				if(retry==400)
        {
            retry = 0;
            do
            {
                r1 = SD_SendCommand(1,0,0);
                retry++;
            }while((r1!=0x00)&& (retry<400));
            if(retry==400)return 1;   //MMC����ʼ����ʱ		    
            //Set sdtype is MMC
						SD_Type = SD_TYPE_MMC;
        }
        //----------MMC�������ʼ����������------------	    
        //����SPIΪ����ģʽ
        SPI0_SetSpeed(SPI_SPEED_4);   
		ssp0_send(0xFF);	 
        //��ֹCRCУ��	   
		r1 = SD_SendCommand(CMD59, 0, 0x95);
        if(r1 != 0x00)return r1;  //������󣬷���r1   	   
        //����Sector Size
        r1 = SD_SendCommand(CMD16, 512, 0x95);
        if(r1 != 0x00)return r1;//������󣬷���r1		 
        //-----------------SD����MMC����ʼ������-----------------

    }//SD��ΪV1.0�汾�ĳ�ʼ������	 
    //������V2.0���ĳ�ʼ��
    //������Ҫ��ȡOCR��ݣ��ж���SD2.0����SD2.0HC��
    else if(r1 == 0x01)
    {
        //V2.0�Ŀ���CMD8�����ᴫ��4�ֽڵ���ݣ�Ҫ����ٽ�������
        buff[0] = ssp0_send(0xFF);  //should be 0x00
        buff[1] = ssp0_send(0xFF);  //should be 0x00
        buff[2] = ssp0_send(0xFF);  //should be 0x01
        buff[3] = ssp0_send(0xFF);  //should be 0xAA	    
        SD_CS(1);	  
        ssp0_send(0xFF);//the next 8 clocks			 
        //�жϸÿ��Ƿ�֧��2.7V-3.6V�ĵ�ѹ��Χ
        //if(buff[2]==0x01 && buff[3]==0xAA) //���жϣ�����֧�ֵĿ����
        {	  
            retry = 0;
            //������ʼ��ָ��CMD55+ACMD41
    		do
    		{
    			r1 = SD_SendCommand(CMD55, 0, 0);
    			if(r1!=0x01)return r1;	   
    			r1 = SD_SendCommand(ACMD41, 0x40000000, 0);
                if(retry>200)return r1;  //��ʱ�򷵻�r1״̬  
            }while(r1!=0);		  
            //��ʼ��ָ�����ɣ���������ȡOCR��Ϣ		   
            //-----------���SD2.0���汾��ʼ-----------
            r1 = SD_SendCommand_NoDeassert(CMD58, 0, 0);
            if(r1!=0x00)
			{
				SD_CS(1);//�ͷ�SDƬѡ�ź�
				return r1;  //�������û�з�����ȷӦ��ֱ���˳�������Ӧ��	 
			}//��OCRָ����󣬽�������4�ֽڵ�OCR��Ϣ
            buff[0] = ssp0_send(0xFF);
            buff[1] = ssp0_send(0xFF); 
            buff[2] = ssp0_send(0xFF);
            buff[3] = ssp0_send(0xFF);		 
            //OCR������ɣ�Ƭѡ�ø�
            SD_CS(1);
            ssp0_send(0xFF);	   
            //�����յ���OCR�е�bit30λ��CCS����ȷ����ΪSD2.0����SDHC
            //���CCS=1��SDHC   CCS=0��SD2.0
            if(buff[0]&0x40)SD_Type = SD_TYPE_V2HC;    //���CCS	 
            else SD_Type = SD_TYPE_V2;	    
            //-----------���SD2.0���汾����----------- 
            //����SPIΪ����ģʽ
            SPI0_SetSpeed(SPI_SPEED_4);  
        }	    
    }
    return r1;
}	 					
/************************************************************************																										
** Description: Receipt data from sd card
** Input: uint8_t *data()
**			 uint16_t len(len of data
**			 uint8_t release(Data is return
** Outpur: 0:NO_ERR
**			 other: 
************************************************************************/ 	 														  
uint8_t SD_ReceiveData(uint8_t *data, uint16_t len, uint8_t release)
{
    // ����һ�δ���
    SD_CS(0);				  	  
	if(SD_GetResponse(0xFE))//�ȴ�SD�����������ʼ����0xFE
	{	  
		SD_CS(1);
		return 1;
	}
    while(len--)//��ʼ�������
    {
        *data=ssp0_send(0xFF);
        data++;
    }
    //������2��αCRC��dummy CRC��
    ssp0_send(0xFF);
    ssp0_send(0xFF);
    if(release==RELEASE)//�����ͷ����ߣ���CS�ø�
    {
        SD_CS(1);//�������
        ssp0_send(0xFF);
    }											  					    
    return 0;
}
/************************************************************************																										
** Description: Read data from sdcard with single block
** Input: uint32_t sector : Current sector where is used to read data
**			 uint8_t *buffer : templ variable to store data
** output: 
************************************************************************/      															  
uint8_t SD_ReadSingleBlock(uint32_t sector, uint8_t *buffer)
{
	uint8_t r1;
    //����Ϊ����ģʽ
    SPI0_SetSpeed(SPI_SPEED_4);  		   
    //�����SDHC�������sector��ַ������ת����byte��ַ
    if(SD_Type!=SD_TYPE_V2HC)
    {
        sector = sector<<9;
    } 
	r1 = SD_SendCommand(CMD17, sector, 0);//������	 		    
	if(r1 != 0x00)return r1; 		   							  
	r1 = SD_ReceiveData(buffer, 512, RELEASE);		 
	if(r1 != 0)return r1;   //����ݳ��?
    else return 0; 
}


#define BLOCK_SIZE 512 
//////////////////////////////////////////////////////////////////////////
/************************************************************************																										
** Description: Write data to sdcard with single block  
** Input: uint32_t sector :Current sector to write data to sdcard
**			 uint8_t *data : is data from buffer
** Output: 
************************************************************************/															  
uint8_t SD_WriteSingleBlock(uint32_t sector, const uint8_t *data)
{
    uint8_t r1;
    uint16_t i;
    uint16_t retry;

    //����Ϊ����ģʽ
    //SPI0_SetSpeed(SPI_SPEED_HIGH);	   
    //�����SDHC�������sector��ַ������ת����byte��ַ
    if(SD_Type!=SD_TYPE_V2HC)
    {
        sector = sector<<9;
    }   
    r1 = SD_SendCommand(CMD24, sector, 0x00);
    if(r1 != 0x00)
    {
        return r1;  //Ӧ����ȷ��ֱ�ӷ���
    }
    
    //��ʼ׼����ݴ���
    SD_CS(0);
    //�ȷ�3������ݣ��ȴ�SD��׼����
    ssp0_send(0xff);
    ssp0_send(0xff);
    ssp0_send(0xff);
    //����ʼ����0xFE
    ssp0_send(0xFE);

    //��һ��sector�����
    for(i=0;i<512;i++)
    {
        ssp0_send(*data++);
    }
    //��2��Byte��dummy CRC
    ssp0_send(0xff);
    ssp0_send(0xff);
    
    //�ȴ�SD��Ӧ��
    r1 = ssp0_send(0xff);
    if((r1&0x1F)!=0x05)
    {
        SD_CS(1);
        return r1;
    }
    
    //�ȴ�������
    retry = 0;
    while(!ssp0_send(0xff))
    {
        retry++;
        if(retry>0xfffe)        //���ʱ��д��û����ɣ������˳�
        {
            SD_CS(1);
            return 1;           //д�볬ʱ����1
        }
    }	    
    //д����ɣ�Ƭѡ��1
    SD_CS(1);
    ssp0_send(0xff);

    return 0;
}
/************************************************************************																										
** Decription: Read multi Block directly from sdcard 
** Input: :uint32_t sector Current sector where to read data
**			 uint8_t *buffer buffer (ram variable) is used to templ store data
**			 uint8_t count count of block
** Output: 
************************************************************************/															  
uint8_t SD_ReadMultiBlock(uint32_t sector, uint8_t *buffer, uint8_t count)
{
    uint8_t r1;
    //SPI0_SetSpeed(SPI_SPEED_HIGH);//����Ϊ����ģʽ  
 	//�����SDHC����sector��ַת��byte��ַ
    if(SD_Type!=SD_TYPE_V2HC)sector = sector<<9;  
    //SD_WaitDataReady();
    //�����������
	r1 = SD_SendCommand(CMD18, sector, 0);//������
	if(r1 != 0x00)return r1;	 
    do//��ʼ�������
    {
        if(SD_ReceiveData(buffer, 512, NO_RELEASE) != 0x00)break; 
        buffer += 512;
    } while(--count);		 
    //ȫ��������ϣ�����ֹͣ����
    SD_SendCommand(CMD12, 0, 0);
    //�ͷ�����
    SD_CS(1);
    ssp0_send(0xFF);    
    if(count != 0)return count;   //���û�д��꣬����ʣ�����	 
    else return 0;	 
}		
/************************************************************************																										
** Description: Write multi block to sdcard 
** Input: uint32_t sector: current sector to strore data in sdcard
**			 uint8_t *buffer data to write to sdcard
**			 uint8_t count Count of block
** Output: 
************************************************************************/															   
uint8_t SD_WriteMultiBlock(uint32_t sector, const uint8_t *data, uint8_t count)
{
    uint8_t r1;
    uint16_t i;
    //SPI0_SetSpeed(SPI_SPEED_HIGH);//����Ϊ����ģʽ	 
    if(SD_Type != SD_TYPE_V2HC)sector = sector<<9;//�����SDHC�������sector��ַ������ת����byte��ַ  
    if(SD_Type != SD_TYPE_MMC) r1 = SD_SendCommand(ACMD23, count, 0x00);//���Ŀ�꿨����MMC��������ACMD23ָ��ʹ��Ԥ����   
    r1 = SD_SendCommand(CMD25, sector, 0x00);//�����д��ָ��
    if(r1 != 0x00)return r1;  //Ӧ����ȷ��ֱ�ӷ���	 
    SD_CS(0);//��ʼ׼����ݴ���   
    ssp0_send(0xff);//�ȷ�3������ݣ��ȴ�SD��׼����
    ssp0_send(0xff);   
    //--------������N��sectorд���ѭ������
    do
    {
        //����ʼ����0xFC �����Ƕ��д��
        ssp0_send(0xFC);	  
        //��һ��sector�����
        for(i=0;i<512;i++)
        {
            ssp0_send(*data++);
        }
        //��2��Byte��dummy CRC
        ssp0_send(0xff);
        ssp0_send(0xff);
        
        //�ȴ�SD��Ӧ��
        r1 = ssp0_send(0xff);
        if((r1&0x1F)!=0x05)
        {
            SD_CS(1);    //���Ӧ��Ϊ���?���������ֱ���˳�
            return r1;
        }		   
        //�ȴ�SD��д�����
        if(SD_WaitDataReady()==1)
        {
            SD_CS(1);    //�ȴ�SD��д����ɳ�ʱ��ֱ���˳�����
            return 1;
        }	   
    }while(--count);//��sector��ݴ������  
    //������������0xFD
    r1 = ssp0_send(0xFD);
    if(r1==0x00)
    {
        count =  0xfe;
    }		   
    if(SD_WaitDataReady()) //�ȴ�׼����
	{
		SD_CS(1);
		return 1;  
	}
    //д����ɣ�Ƭѡ��1
    SD_CS(1);
    ssp0_send(0xff);  
    return count;   //����countֵ�����д����count=0������count=1
}
