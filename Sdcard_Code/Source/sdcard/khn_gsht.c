/****************************************Copyright (c)**************************************************
**                               			Khanhhoi Co.,LTD.
**
**--------------File Info-------------------------------------------------------------------------------
** File Name: 			Sd.c
** Last modified Date: 	21-03-2014
** Last Version: 		1.0
** Descriptions: 		sdcard function using for KHN
**
**------------------------------------------------------------------------------------------------------
** Created 	 By: 		Agrond
** Created date: 		21-03-2014
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
//Include for sdcard; Remember change compiler part to ../source/sdcard;../source/sdcard/Fatfs;
#include "khn_gsht.h"
//Validation definition

//u8 buffer_data[512];

FATFS Fatfs;
DIR dir;
char fileName[300];
FIL file;

int ReadData(int code,int date,int time){
	FRESULT fr;
//Get DateTime
//Open folder of that date
	f_mount(&Fatfs, "0:", 0);
	sprintf(fileName,"0:/%d/%d.txt",date,time);
	fr = f_open(&file,fileName,FA_READ);
	if(fr) return 1;
	else return 0;
	//return file;
	//f_close(&file);
}
int WriteData(int code,int date,int time){
	FRESULT fr;
//Get DateTime
//Create folder of that date
	f_mount(&Fatfs, "0:", 0);
	sprintf(fileName,"%d",date);
	fr=f_mkdir(fileName);
	//check if folder can not create(exist or can create)
	if(fr != FR_OK && fr != FR_EXIST)
		return 1;
	//Create file to hold data
	sprintf(fileName,"0:/%d/%d.txt",date,time);
	/*Open file to write infor if file does not exist create new file*/
	fr = f_open(&file,fileName,FA_WRITE |FA_OPEN_ALWAYS);
	f_lseek(&file, f_size(&file));
	if(fr) return 2;
	return 0;
}

void Close(){
	f_close(&file);
}