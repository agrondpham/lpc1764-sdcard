#include <LPC17xx.H>                    				//NXP LPC17xx definitions 
#include "LCD.h"
#include "string.h"
#include "sys.h"
#include "lcd.h"
#include "touch.h"
#include "exti.h"
#include "../sdcard/SPI.h"
#include "../sdcard/SD.h"
#include "delay.h"
#include "../sdcard/Fatfs/diskio.h"
#include "../sdcard/Fatfs/ff.h"
#include "../uart/uart.h"

FIL file,file2;
u8 buffer_data[512];

FATFS Fatfs;
DIR dir;

#define	_USELCD	0 //use lcd
#define	_USEUART	1 //use urat

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
#if _USELCD == 1
void Load_Drow_Dialog(void)
{
	LCD_Clear(WHITE);   
 	POINT_COLOR=Red;									//Setting font colors 
	LCD_ShowString(216,0,"RST");						//Displaying screen clear area
  	POINT_COLOR=RED;									//Setting brush color 
}
#endif
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
	FRESULT res;
	SystemInit();
	SPI0_Init();

	//ssp0_init(); //SPI init
	#if _USEUART == 1
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
			#if _USELCD == 1
				LCD_ShowString(30,160,line);
			#endif
			#if _USEUART == 1
				UART2_SendString(line);	
			#endif	
        

    /* Close the file */
    f_close(&file);
		
		/*Create folder*/
		f_mkdir("12032014");
		
		/*Open file to write infor if file does not exist create new file*/
		fr = f_open(&file2,"0:/12032014/132260.txt",FA_CREATE_ALWAYS | FA_WRITE);
		if (fr) return (int)fr;
		//line="abc xyz";
		f_puts(text,&file2);
		
				f_putc('c',&file2);
		f_close(&file2);
				while(1){}		

}



