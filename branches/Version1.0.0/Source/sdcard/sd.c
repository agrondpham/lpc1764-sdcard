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
#include "sys.h"
#include "spi.h"
#include "sd.h"
#include "lpc17xx.h"
#include "fatfs/diskio.h"
u8  SD_Type=0;				//SD card type

/* RTC timer strcuture */
typedef struct {
    u8     sec;     /* Second value - [0,59] */
    u8     min;     /* Minute value - [0,59] */
    u8     hour;    /* Hour value - [0,23] */
    u8     mday;    /* Day of the month value - [1,31] */
	u8     wday;    /* Day of week value - [0,6] */
	u16    yday;    /* Day of year value - [1,365] */
    u8     mon;     /* Month value - [1,12] */
    u16    year;    /* Year value - [0,4095] */    
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

u8 SD_GetResponse(u8 Response)
{
	u16 Count=0xFFF;//等待次数	   						  
	while ((ssp0_send(0XFF)!=Response)&&Count)Count--;//等待得到准确的回应  	  
	if (Count==0)return MSD_RESPONSE_FAILURE;//得到回应失败   
	else return MSD_RESPONSE_NO_ERROR;//正确回应
}

u8 SD_WaitDataReady(void)
{
    u8 r1=MSD_DATA_OTHER_ERROR;
    u32 retry;
    retry=0;
    do
    {
        r1=ssp0_send(0xFF)&0X1F;//读到回应
        if(retry==0xfffe)return 1; 
		retry++;
		switch (r1)
		{					   
			case MSD_DATA_OK://数据接收正确了	 
				r1=MSD_DATA_OK;
				break;  
			case MSD_DATA_CRC_ERROR:  //CRC校验错误
				return MSD_DATA_CRC_ERROR;  
			case MSD_DATA_WRITE_ERROR://数据写入错误
				return MSD_DATA_WRITE_ERROR;  
			default://未知错误    
				r1=MSD_DATA_OTHER_ERROR;
				break;	 
		}   
    }while(r1==MSD_DATA_OTHER_ERROR); //数据错误时一直等待
	retry=0;
	while(ssp0_send(0XFF)==0)//读到数据为0,则数据还未写完成
	{
		retry++;
		//delay_us(10);//SD卡写等待需要较长的时间
		if(retry>=0XFFFFFFFE)return 0XFF;//等待失败了
	};	    
    return 0;//成功了
}

/************************************************************************																										
** Description: Send command to sdcard
** Input: u8 cmd   Command code 
**			 u32 arg 
**	    	 u8 crc   Check sum
** Ouput: return value
************************************************************************/													  
u8 SD_SendCommand(u8 cmd, u32 arg, u8 crc)
{
    u8 r1;	
	u8 Retry=0;	         
	SD_CS(1);
    ssp0_send(0xff);//高速写命令延时
	ssp0_send(0xff);     
 	ssp0_send(0xff);  	 
    //片选端置低，选中SD卡
    SD_CS(0); 
    //发送
    ssp0_send(cmd | 0x40);//分别写入命令
    ssp0_send(arg >> 24);
    ssp0_send(arg >> 16);
    ssp0_send(arg >> 8);
    ssp0_send(arg);
    ssp0_send(crc); 
    //等待响应，或超时退出
    while((r1=ssp0_send(0xFF))==0xFF)
    {
        Retry++;	    
        if(Retry>200)break; 
    }   
    //关闭片选
    SD_CS(1);
    //在总线上额外增加8个时钟，让SD卡完成剩下的工作
    ssp0_send(0xFF);
    //返回状态值
    return r1;
}
u8 SD_SendCommand_NoDeassert(u8 cmd, u32 arg, u8 crc)
{
	u8 Retry=0;	         
	u8 r1;			   
    ssp0_send(0xff);//高速写命令延时
	ssp0_send(0xff);  	 	 
    SD_CS(0);//片选端置低，选中SD卡	   
    //发送
    ssp0_send(cmd | 0x40); //分别写入命令
    ssp0_send(arg >> 24);
    ssp0_send(arg >> 16);
    ssp0_send(arg >> 8);
    ssp0_send(arg);
    ssp0_send(crc);   
    //等待响应，或超时退出
    while((r1=ssp0_send(0xFF))==0xFF)
    {
        Retry++;	    
        if(Retry>200)break; 
    }  	  
    //返回响应值
    return r1;
}
u8 SD_Idle_Sta(void)
{
	u16 i;
	u8 retry;	   	  
    for(i=0;i<0xf00;i++);//纯延时，等待SD卡上电完成	 
    //先产生>74个脉冲，让SD卡自己初始化完成
    for(i=0;i<10;i++)ssp0_send(0xFF); 
    //-----------------SD卡复位到idle开始-----------------
    //循环连续发送CMD0，直到SD卡返回0x01,进入IDLE状态
    //超时则直接退出
    retry = 0;
    do
    {	   
        //发送CMD0，让SD卡进入IDLE状态
        i = SD_SendCommand(CMD0, 0, 0x95);
        retry++;
    }while((i!=0x01)&&(retry<200));
    //跳出循环后，检查原因：初始化成功？or 重试超时？
    if(retry==200)return 1; //失败
	return 0;//成功	 						  
}
/************************************************************************																										
** Description: Init SD card. make sd card to spi mode
** Output: 0:NO_ERR
**			 1:TIME_OUT
**			 99:NO_CARD
************************************************************************/    																 
u8 SD_Init(void)
{								 
    u8 r1;     //return value is card type by bit
    u16 retry;  
    u8 buff[6];
	ssp0_init();//Init SSP0
 	SPI0_SetSpeed(SPI_SPEED_256);//Set spi speec 256 
	SD_CS(1);	
  if(SD_Idle_Sta()) return 1;//Card fail can not read	  
 	SD_CS(0);	
	r1 = SD_SendCommand_NoDeassert(8, 0x1aa,0x87);	     
    //如果卡片版本信息是v1.0版本的，即r1=0x05，则进行以下初始化
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
            if(retry==400)return 1;   //MMC卡初始化超时		    
            //Set sdtype is MMC
						SD_Type = SD_TYPE_MMC;
        }
        //----------MMC卡额外初始化操作结束------------	    
        //设置SPI为高速模式
        SPI0_SetSpeed(SPI_SPEED_4);   
		ssp0_send(0xFF);	 
        //禁止CRC校验	   
		r1 = SD_SendCommand(CMD59, 0, 0x95);
        if(r1 != 0x00)return r1;  //命令错误，返回r1   	   
        //设置Sector Size
        r1 = SD_SendCommand(CMD16, 512, 0x95);
        if(r1 != 0x00)return r1;//命令错误，返回r1		 
        //-----------------SD卡、MMC卡初始化结束-----------------

    }//SD卡为V1.0版本的初始化结束	 
    //下面是V2.0卡的初始化
    //其中需要读取OCR数据，判断是SD2.0还是SD2.0HC卡
    else if(r1 == 0x01)
    {
        //V2.0的卡，CMD8命令后会传回4字节的数据，要跳过再结束本命令
        buff[0] = ssp0_send(0xFF);  //should be 0x00
        buff[1] = ssp0_send(0xFF);  //should be 0x00
        buff[2] = ssp0_send(0xFF);  //should be 0x01
        buff[3] = ssp0_send(0xFF);  //should be 0xAA	    
        SD_CS(1);	  
        ssp0_send(0xFF);//the next 8 clocks			 
        //判断该卡是否支持2.7V-3.6V的电压范围
        //if(buff[2]==0x01 && buff[3]==0xAA) //不判断，让其支持的卡更多
        {	  
            retry = 0;
            //发卡初始化指令CMD55+ACMD41
    		do
    		{
    			r1 = SD_SendCommand(CMD55, 0, 0);
    			if(r1!=0x01)return r1;	   
    			r1 = SD_SendCommand(ACMD41, 0x40000000, 0);
                if(retry>200)return r1;  //超时则返回r1状态  
            }while(r1!=0);		  
            //初始化指令发送完成，接下来获取OCR信息		   
            //-----------鉴别SD2.0卡版本开始-----------
            r1 = SD_SendCommand_NoDeassert(CMD58, 0, 0);
            if(r1!=0x00)
			{
				SD_CS(1);//释放SD片选信号
				return r1;  //如果命令没有返回正确应答，直接退出，返回应答	 
			}//读OCR指令发出后，紧接着是4字节的OCR信息
            buff[0] = ssp0_send(0xFF);
            buff[1] = ssp0_send(0xFF); 
            buff[2] = ssp0_send(0xFF);
            buff[3] = ssp0_send(0xFF);		 
            //OCR接收完成，片选置高
            SD_CS(1);
            ssp0_send(0xFF);	   
            //检查接收到的OCR中的bit30位（CCS），确定其为SD2.0还是SDHC
            //如果CCS=1：SDHC   CCS=0：SD2.0
            if(buff[0]&0x40)SD_Type = SD_TYPE_V2HC;    //检查CCS	 
            else SD_Type = SD_TYPE_V2;	    
            //-----------鉴别SD2.0卡版本结束----------- 
            //设置SPI为高速模式
            SPI0_SetSpeed(SPI_SPEED_4);  
        }	    
    }
    return r1;
}	 					
/************************************************************************																										
** Description: Receipt data from sd card
** Input: u8 *data()
**			 u16 len(len of data
**			 u8 release(Data is return
** Outpur: 0:NO_ERR
**			 other: 
************************************************************************/ 	 														  
u8 SD_ReceiveData(u8 *data, u16 len, u8 release)
{
    // 启动一次传输
    SD_CS(0);				  	  
	if(SD_GetResponse(0xFE))//等待SD卡发回数据起始令牌0xFE
	{	  
		SD_CS(1);
		return 1;
	}
    while(len--)//开始接收数据
    {
        *data=ssp0_send(0xFF);
        data++;
    }
    //下面是2个伪CRC（dummy CRC）
    ssp0_send(0xFF);
    ssp0_send(0xFF);
    if(release==RELEASE)//按需释放总线，将CS置高
    {
        SD_CS(1);//传输结束
        ssp0_send(0xFF);
    }											  					    
    return 0;
}
/************************************************************************																										
** Description: Read data from sdcard with single block
** Input: u32 sector : Current sector where is used to read data
**			 u8 *buffer : templ variable to store data
** output: 
************************************************************************/      															  
u8 SD_ReadSingleBlock(u32 sector, u8 *buffer)
{
	u8 r1;	    
    //设置为高速模式
    SPI0_SetSpeed(SPI_SPEED_4);  		   
    //如果不是SDHC，给定的是sector地址，将其转换成byte地址
    if(SD_Type!=SD_TYPE_V2HC)
    {
        sector = sector<<9;
    } 
	r1 = SD_SendCommand(CMD17, sector, 0);//读命令	 		    
	if(r1 != 0x00)return r1; 		   							  
	r1 = SD_ReceiveData(buffer, 512, RELEASE);		 
	if(r1 != 0)return r1;   //读数据出错！
    else return 0; 
}


#define BLOCK_SIZE 512 
//////////////////////////////////////////////////////////////////////////
/************************************************************************																										
** Description: Write data to sdcard with single block  
** Input: u32 sector :Current sector to write data to sdcard
**			 u8 *data : is data from buffer
** Output: 
************************************************************************/															  
u8 SD_WriteSingleBlock(u32 sector, const u8 *data)
{
    u8 r1;
    u16 i;
    u16 retry;

    //设置为高速模式
    //SPI0_SetSpeed(SPI_SPEED_HIGH);	   
    //如果不是SDHC，给定的是sector地址，将其转换成byte地址
    if(SD_Type!=SD_TYPE_V2HC)
    {
        sector = sector<<9;
    }   
    r1 = SD_SendCommand(CMD24, sector, 0x00);
    if(r1 != 0x00)
    {
        return r1;  //应答不正确，直接返回
    }
    
    //开始准备数据传输
    SD_CS(0);
    //先放3个空数据，等待SD卡准备好
    ssp0_send(0xff);
    ssp0_send(0xff);
    ssp0_send(0xff);
    //放起始令牌0xFE
    ssp0_send(0xFE);

    //放一个sector的数据
    for(i=0;i<512;i++)
    {
        ssp0_send(*data++);
    }
    //发2个Byte的dummy CRC
    ssp0_send(0xff);
    ssp0_send(0xff);
    
    //等待SD卡应答
    r1 = ssp0_send(0xff);
    if((r1&0x1F)!=0x05)
    {
        SD_CS(1);
        return r1;
    }
    
    //等待操作完成
    retry = 0;
    while(!ssp0_send(0xff))
    {
        retry++;
        if(retry>0xfffe)        //如果长时间写入没有完成，报错退出
        {
            SD_CS(1);
            return 1;           //写入超时返回1
        }
    }	    
    //写入完成，片选置1
    SD_CS(1);
    ssp0_send(0xff);

    return 0;
}
/************************************************************************																										
** Decription: Read multi Block directly from sdcard 
** Input: :u32 sector Current sector where to read data
**			 u8 *buffer buffer (ram variable) is used to templ store data
**			 u8 count count of block
** Output: 
************************************************************************/															  
u8 SD_ReadMultiBlock(u32 sector, u8 *buffer, u8 count)
{
    u8 r1;	 			 
    //SPI0_SetSpeed(SPI_SPEED_HIGH);//设置为高速模式  
 	//如果不是SDHC，将sector地址转成byte地址
    if(SD_Type!=SD_TYPE_V2HC)sector = sector<<9;  
    //SD_WaitDataReady();
    //发读多块命令
	r1 = SD_SendCommand(CMD18, sector, 0);//读命令
	if(r1 != 0x00)return r1;	 
    do//开始接收数据
    {
        if(SD_ReceiveData(buffer, 512, NO_RELEASE) != 0x00)break; 
        buffer += 512;
    } while(--count);		 
    //全部传输完毕，发送停止命令
    SD_SendCommand(CMD12, 0, 0);
    //释放总线
    SD_CS(1);
    ssp0_send(0xFF);    
    if(count != 0)return count;   //如果没有传完，返回剩余个数	 
    else return 0;	 
}		
/************************************************************************																										
** Description: Write multi block to sdcard 
** Input: u32 sector: current sector to strore data in sdcard
**			 u8 *buffer data to write to sdcard 
**			 u8 count Count of block
** Output: 
************************************************************************/															   
u8 SD_WriteMultiBlock(u32 sector, const u8 *data, u8 count)
{
    u8 r1;
    u16 i;	 		 
    //SPI0_SetSpeed(SPI_SPEED_HIGH);//设置为高速模式	 
    if(SD_Type != SD_TYPE_V2HC)sector = sector<<9;//如果不是SDHC，给定的是sector地址，将其转换成byte地址  
    if(SD_Type != SD_TYPE_MMC) r1 = SD_SendCommand(ACMD23, count, 0x00);//如果目标卡不是MMC卡，启用ACMD23指令使能预擦除   
    r1 = SD_SendCommand(CMD25, sector, 0x00);//发多块写入指令
    if(r1 != 0x00)return r1;  //应答不正确，直接返回	 
    SD_CS(0);//开始准备数据传输   
    ssp0_send(0xff);//先放3个空数据，等待SD卡准备好
    ssp0_send(0xff);   
    //--------下面是N个sector写入的循环部分
    do
    {
        //放起始令牌0xFC 表明是多块写入
        ssp0_send(0xFC);	  
        //放一个sector的数据
        for(i=0;i<512;i++)
        {
            ssp0_send(*data++);
        }
        //发2个Byte的dummy CRC
        ssp0_send(0xff);
        ssp0_send(0xff);
        
        //等待SD卡应答
        r1 = ssp0_send(0xff);
        if((r1&0x1F)!=0x05)
        {
            SD_CS(1);    //如果应答为报错，则带错误代码直接退出
            return r1;
        }		   
        //等待SD卡写入完成
        if(SD_WaitDataReady()==1)
        {
            SD_CS(1);    //等待SD卡写入完成超时，直接退出报错
            return 1;
        }	   
    }while(--count);//本sector数据传输完成  
    //发结束传输令牌0xFD
    r1 = ssp0_send(0xFD);
    if(r1==0x00)
    {
        count =  0xfe;
    }		   
    if(SD_WaitDataReady()) //等待准备好
	{
		SD_CS(1);
		return 1;  
	}
    //写入完成，片选置1
    SD_CS(1);
    ssp0_send(0xff);  
    return count;   //返回count值，如果写完则count=0，否则count=1
}
