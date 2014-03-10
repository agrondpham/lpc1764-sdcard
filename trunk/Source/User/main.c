/******************************************************************************/
/* Blinky.c: LED Flasher and graphic demo                                     */
/******************************************************************************/
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2005-2009 Keil Software. All rights reserved.                */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/                  
#include <LPC17xx.H>                    				//NXP LPC17xx definitions 
#include "LCD.h"
#include "string.h"
#include "sys.h"
#include "lcd.h"
#include "touch.h"
#include "exti.h"
#include "M45PE161.h"
#include "text.h"
#include "MMC_SD.h"
#include "FAT.h"
#include "fontupd.h"
#include "delay.h"
#include "ssp0.h"
#include "jpegbmp.h"
#include "../Fatfs/diskio.h"
#include "../Fatfs/ff.h"
#include "../Fatfs/integer.h"
#include "../uart/uart.h"
#define  M45PE16_FLASH_ID    0x4015
volatile unsigned long FLASH_ID = 0;
const unsigned char *sysfile[4]=
{			  
"0.BMP",
"4.BMP",
"320_216.BMP",
"1.BMP",	  
};

const unsigned char *folderData[2]=
{
"MUSIC",
"MUSIC2",	  
};
 		unsigned char jpg_buffer1[1024];


FATFS Fatfs;
DIR dir;

#define	_USELCD	0 //use lcd
#define	_USEUART	1 //use urat
/*
*********************************************************************************************************
* Description: 	The function is setting font colors、brush colors and the area of clear
* Arguments  : 	None
* Returns    : 	None
*********************************************************************************************************
*/
void Load_Drow_Dialog(void)
{
	LCD_Clear(WHITE);   
 	POINT_COLOR=Red;									//Setting font colors 
	LCD_ShowString(216,0,"RST");						//Displaying screen clear area
  	POINT_COLOR=RED;									//Setting brush color 
}
int xatoi (			/* 0:Failed, 1:Successful */
	char **str,		/* Pointer to pointer to the string */
	long *res		/* Pointer to the valiable to store the value */
)
{
	unsigned long val;
	unsigned char c, r, s = 0;


	*res = 0;

	while ((c = **str) == ' ') (*str)++;	/* Skip leading spaces */

	if (c == '-') {		/* negative? */
		s = 1;
		c = *(++(*str));
	}

	if (c == '0') {
		c = *(++(*str));
		switch (c) {
		case 'x':		/* hexdecimal */
			r = 16; c = *(++(*str));
			break;
		case 'b':		/* binary */
			r = 2; c = *(++(*str));
			break;
		default:
			if (c <= ' ') return 1;	/* single zero */
			if (c < '0' || c > '9') return 0;	/* invalid char */
			r = 8;		/* octal */
		}
	} else {
		if (c < '0' || c > '9') return 0;	/* EOL or invalid char */
		r = 10;			/* decimal */
	}

	val = 0;
	while (c > ' ') {
		if (c >= 'a') c -= 0x20;
		c -= '0';
		if (c >= 17) {
			c -= 7;
			if (c <= 9) return 0;	/* invalid char */
		}
		if (c >= r) return 0;		/* invalid char for current radix */
		val = val * r + c;
		c = *(++(*str));
	}
	if (s) val = 0 - val;			/* apply sign if needed */

	*res = val;
	return 1;
}

/*



*/

/*
*********************************************************************************************************
* Description: 	Main function
* Arguments  : 	None
* Returns    : 	None
*********************************************************************************************************
*/
int main (void)                       
{
	u8* filename;
	//FileInfoStruct *CurFile;
	FileInfoStruct FileTemp;
		u32 fcluster=0;	 
	long p1;
	FRESULT res;
	u16 i=2;
	FileInfoStruct *FileInfo1;
	FileInfoStruct F_Info1[3];
//	BYTE *PIC_Name;
//	FileInfoStruct FileTemp;
	u32 temp_cluster;
	SystemInit();
	SPIx_Init();

	ssp0_init(); //SPI init
	FLASH_ID = SPI_Flash_ReadID();/* M45PE161的值为204515H(十六进制)   或 2113557D （十进制）  */
	#if _USEUART == 1
		//UART0_Init(); // Init uart0
		UART2_Init(); //init uart2
		UART2_SendString("www.Khanhoi.vn\r\n");
		UART2_SendString("NXP1764 cdcard testing\r\n");				    	 
		UART2_SendString("Author: Pham The Long\r\n");				    	 
		UART2_SendString("Company: Khanhhoi\r\n");				    	 
		UART2_SendString("Copyright 2014\r\n");	
	#endif
	#if _USELCD == 1
		LCD_Init();
		Load_Drow_Dialog();
		Font_Init();
		POINT_COLOR=Blue;      
		LCD_Clear(WHITE);
		LCD_ShowString(60,30,"www.Khanhoi.vn");
		POINT_COLOR=RED;    
		Show_Str(30,50,"NXP1764 cdcard testing",16,0);				    	 
		Show_Str(30,70,"Author: Pham The Long",16,0);				    	 
		Show_Str(30,90,"Company: Khanhhoi",16,0);				    	 
		Show_Str(30,110,"Copyright 2014",16,0);	
	#endif


				 
		if(disk_initialize(0)==STA_NOINIT){
			#if _USEUART == 1
				UART2_SendString("Card can not init\r\n");	
			#endif
			#if _USELCD == 1
				Show_Str(60,150,"Card can not init",16,0);	
			#endif			
		}else{
			#if _USELCD == 1
			Show_Str(60,150,"Card is ready",16,0);
			Show_Str(30,180,"Card type: ",16,0);
			#endif
			#if _USEUART == 1
				UART2_SendString("Card is ready\r\n");
				UART2_SendString("Card type: \r\n");
			#endif	
			switch (SD_Type)
			{
					case SD_TYPE_MMC:
						#if _USELCD == 1
							Show_Str(30,210,"MMC\n",16,0);			
						#endif
						#if _USEUART == 1
							UART2_SendString("MMC\r\n");
						#endif							
							break;
					case SD_TYPE_V1:
						#if _USELCD == 1
							Show_Str(30,210,"Version 1.x Standard Capacity SD card.\n",16,0);
						#endif
						#if _USEUART == 1
							UART2_SendString("Version 1.x Standard Capacity SD card.\r\n");	
						#endif	
							break;
					case SD_TYPE_V2:
						#if _USELCD == 1
							Show_Str(30,210,"Version 2.0 or later Standard Capacity SD card.\n",16,0);	
						#endif
						#if _USEUART == 1
							UART2_SendString("Version 2.0 or later Standard Capacity SD card.\r\n");	
						#endif	
							break;
					case SD_TYPE_V2HC:
						#if _USELCD == 1
							Show_Str(30,210,"Version 2.0 or later High/eXtended Capacity SD card.\n",16,0);
						#endif
						#if _USEUART == 1
							UART2_SendString("Version 2.0 or later High/eXtended Capacity SD card.\r\n");	
						#endif	
							break;
					default:
							break;            
			}
		};
//		p1=0;
//		f_mount(&Fatfs,"",(BYTE)p1);
//		res = f_opendir(&dir, "");                       /* Open the directory */
//    if (res == FR_OK){
//			Show_Str(60,180,"Can read direct",16,0);
//		}

		//字体更新 
		//SD_Init();
		#if _USELCD == 1
		LCD_Clear(WHITE);
		#endif	
	 	while(FAT_Init())//FAT 错误
		{
			#if _USELCD == 1
			LCD_ShowString(60,150,"FAT SYS ERROR");  
			#endif
			#if _USEUART == 1
				UART2_SendString("FAT SYS ERROR\r\n");
			#endif		
			i= SD_Init();
			if(i)//SD卡初始化 
			{		
				#if _USELCD == 1				
					LCD_ShowString(60,170,"SD_CARD ERROR");
				#endif
				#if _USEUART == 1
					UART2_SendString("SD_CARD ERROR\r\n");
				#endif		
			}	  
			delay_ms(500);
			//LCD_Fill(60,90,240,126,WHITE);//清除显示			  
			//delay_ms(500);  
		}
			#if _USELCD == 1	
				LCD_ShowString(60,130,"FAT SYS OK!"); 
			#endif
				#if _USEUART == 1
					UART2_SendString("FAT SYS OK!\r\n");
				#endif	

		if(FAT32_Enable)fcluster=FirstDirClust;
		else fcluster=0;
		FileTemp=F_Search(fcluster,(unsigned char *)folderData[0],T_FILE);
		if(FileTemp.F_StartCluster==0)					  //Check folder		  
		{
						#if _USELCD == 1
						Show_Str(30,160,"Can not find *.BMP in this folder.\n",16,0);
						#endif
						#if _USEUART == 1
							UART2_SendString("Can not find *.BMP in this folder.\r\n");
						#endif		
		}else{
			PIC_Cluster=FileTemp.F_StartCluster;
						#if _USELCD == 1
						Show_Str(30,160,"reading this folder.\n",16,0);
						#endif
						#if _USEUART == 1
							UART2_SendString("reading this folder.\r\n");
						#endif		

		}

		
	 	/*while(PIC_Display()!=0)//字体更新出错
		{						  
			LCD_ShowString(60,170,"SYSTEM FILE LOST");		  
			delay_ms(500);    
			LCD_ShowString(60,170,"Please Check....");
			delay_ms(500);    
		};*/
		#if _USELCD == 1 
		LCD_Clear(WHITE);		
		#endif

		temp_cluster = PIC_Cluster;
		FileInfo1=&F_Info1[0];//开辟暂存空间.

		//while(1)
		//{
			//for(i=1;i<10;i++)
			//{
					Get_File_Info(temp_cluster,FileInfo1,T_BMP,&i);	
					//LCD_Clear(White);	
					//CurFile=FileInfo1;
					filename=GetFileName(FileInfo1);//显示图片
					//POINT_COLOR = Red;
					//Show_Str(0,0,FileInfo1->F_Name,16,1);	 	
//					F_Open(FileInfo1);
//					F_Read(FileInfo1,jpg_buffer1);	   
//					F_Read(FileInfo1,jpg_buffer1+512);
						#if _USELCD == 1
									Show_Str(30,190,filename,16,0);	
						#endif
						#if _USEUART == 1
							UART2_SendString(filename);
						#endif		
				//delay_ms(300);		
			//}
		 //}
}


