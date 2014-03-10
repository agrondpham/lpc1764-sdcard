#ifndef __FONTUPD_H__
#define __FONTUPD_H__	 
#include "lpc17xx.h"
#include "fat.h"	 
//中文汉字支持程序 V1.1					   
//2010/5/23	

#define EN_UPDATE_FONT //使能字体更新,通过关闭这里实现禁止字库更新

 
extern u32 FONT16ADDR ;
extern u32 FONT12ADDR ;
extern u32 UNI2GBKADDR;
extern u32 PIC_Cluster;
u8 PIC_Display(void);//更新字库   
u8 Font_Init(void);//初始化字库
#endif











