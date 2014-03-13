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
FIL file;
u8 buffer_data[512];

FATFS Fatfs;
DIR dir;

#define	_USELCD	1 //use lcd
#define	_USEUART	0 //use urat

void Delay (uint32_t Time)
{
    uint32_t i;
    
    i = 0;
    while (Time--) {
        for (i = 0; i < 5000; i++);
    }
}
/*
*********************************************************************************************************
* Description: 	The function is setting font colors¡¢brush colors and the area of clear
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
	char text[]="\nhello\n test test test";
	char line[82];
	FRESULT fr;
//	u8* filename;
	//FileInfoStruct *CurFile;
//	FileInfoStruct FileTemp;
//		u32 fcluster=0;	 
//	long p1;
	FRESULT res;
//	u16 i=2;
//	FileInfoStruct *FileInfo1;
//	FileInfoStruct F_Info1[3];
//	BYTE *PIC_Name;
//	FileInfoStruct FileTemp;
//	u32 temp_cluster;
	SystemInit();
	SPIx_Init();

	ssp0_init(); //SPI init
	//FLASH_ID = SPI_Flash_ReadID();
	#if _USEUART == 1
		//UART2_Init(); // Init uart0
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
		//Font_Init();
		POINT_COLOR=Blue;      
		LCD_Clear(WHITE);
		LCD_ShowString(60,30,"www.Khanhoi.vn");
		POINT_COLOR=RED;    
		LCD_ShowString(30,50,"NXP1764 cdcard testing");				    	 
		LCD_ShowString(30,70,"Author: Pham The Long");				    	 
		LCD_ShowString(30,90,"Company: Khanhhoi");				    	 
		LCD_ShowString(30,110,"Copyright 2014");	
	#endif


				 
		if(disk_initialize(0)==STA_NOINIT){
			#if _USEUART == 1
				UART2_SendString("Card can not init\r\n");	
			#endif
			#if _USELCD == 1
				LCD_ShowString(60,150,"Card can not init");	
			#endif			
		}else{
			#if _USELCD == 1
			LCD_ShowString(60,150,"Card is ready");
			LCD_ShowString(30,180,"Card type: ");
			#endif
			#if _USEUART == 1
				UART2_SendString("Card is ready\r\n");
				UART2_SendString("Card type: \r\n");
			#endif	
			switch (SD_Type)
			{
					case SD_TYPE_MMC:
						#if _USELCD == 1
							LCD_ShowString(30,210,"MMC\n");			
						#endif
						#if _USEUART == 1
							UART2_SendString("MMC\r\n");
						#endif							
							break;
					case SD_TYPE_V1:
						#if _USELCD == 1
							LCD_ShowString(30,210,"Version 1.x Standard Capacity SD card.\n");
						#endif
						#if _USEUART == 1
							UART2_SendString("Version 1.x Standard Capacity SD card.\r\n");	
						#endif	
							break;
					case SD_TYPE_V2:
						#if _USELCD == 1
							LCD_ShowString(30,210,"Version 2.0 or later Standard Capacity SD card.\n");	
						#endif
						#if _USEUART == 1
							UART2_SendString("Version 2.0 or later Standard Capacity SD card.\r\n");	
						#endif	
							break;
					case SD_TYPE_V2HC:
						#if _USELCD == 1
							LCD_ShowString(30,210,"Version 2.0 or later High/eXtended Capacity SD card.\n");
						#endif
						#if _USEUART == 1
							UART2_SendString("Version 2.0 or later High/eXtended Capacity SD card.\r\n");	
						#endif	
							break;
					default:
							break;            
			}
		};
		#if _USELCD == 1
		LCD_Clear(WHITE);
		#endif	
		f_mount(&Fatfs, "0:", 0);
		fr = f_open(&file,"0:/MUSIC/Home.txt",FA_READ);
		if (fr) return (int)fr;
		/* Read all lines and display it */
    while (f_gets(line, sizeof line, &file))
        LCD_ShowString(30,160,line);

    /* Close the file */
    f_close(&file);
		
		/*Create folder*/
		f_mkdir("12032014");
		
		/*Open file to write infor if file does not exist create new file*/
		fr = f_open(&file,"0:/12032014/132260.txt",FA_CREATE_ALWAYS | FA_WRITE);
		if (fr) return (int)fr;
		//line="abc xyz";
		if(f_puts(text,&file)!=-1){
		while(1){}		
		}else{

		}
		//res = f_write(&file, text, strlen(text), &bw);

}



