#ifndef __FONTUPD_H__
#define __FONTUPD_H__	 
#include "lpc17xx.h"
#include "fat.h"	 
//���ĺ���֧�ֳ��� V1.1					   
//2010/5/23	

#define EN_UPDATE_FONT //ʹ���������,ͨ���ر�����ʵ�ֽ�ֹ�ֿ����

 
extern u32 FONT16ADDR ;
extern u32 FONT12ADDR ;
extern u32 UNI2GBKADDR;
extern u32 PIC_Cluster;
u8 PIC_Display(void);//�����ֿ�   
u8 Font_Init(void);//��ʼ���ֿ�
#endif











