#include "fontupd.h"
#include "sys.h"
#include "fat.h"
#include "M45PE161.h"
#include "lcd.h"
#include "delay.h"
#include "TEXT.H"
#include "jpegbmp.h"

u32 PIC_Cluster = 0;
//系统文件夹		  
const unsigned char *folder[2]=
{
"MUSIC",
"FONT",	  
};

//系统文件定义

//字节0~3,  记录UNI2GBKADDR;字节4~7  ,记录UNI2GBKADDR的大小
//字节8~11, 记录FONT16ADDR ;字节12~15,记录FONT16ADDR的大小
//字节16~19,记录FONT12ADDR ;字节20~23,记录FONT12ADDR的大小
//字体存放,从100K处开始
//100K,存放UNICODE2GBK的转换码	

u32 FONT16ADDR ;//16字体存放的地址
u32 FONT12ADDR ;//12字体存放的地址
u32 UNI2GBKADDR;//UNICODE TO GBK 表存放地址	  	 

#ifdef EN_UPDATE_FONT
u8 temp[512];  //零时空间
FileInfoStruct F_Info1[5];
u8 PIC_Display(void)
{
	u32 fcluster=0;	   		 
	FileInfoStruct FileTemp;//零时文件夹 

//	u16 index=1;  //当前选择的文件编号		  				    	 
	//得到根目录的簇号
	if(FAT32_Enable)fcluster=FirstDirClust;
	else fcluster=0;			   
	FileTemp=F_Search(fcluster,(unsigned char *)folder[0],T_FILE);//查找MUSIC文件夹	  
	if(FileTemp.F_StartCluster==0)return 1;						  //系统文件夹丢失			  
	{
		PIC_Cluster = FileTemp.F_StartCluster;      //字体文件夹簇号	
		Show_Str(60,150,"找到MUSIC文件夹",16,0);
		delay_ms(500);
			
		    
	}		    	
	return 0;//成功
}
#endif

//用这个函数得到字体地址
//在系统使用汉字支持之前,必须调用该函数
//包括FONT16ADDR,FONT12ADDR,UNI2GBKADDR;  
u8 Font_Init(void)
{
	u32 tempsys[2];//临时记录文件起始位置和文件大小
	u8 fontok=0;
	SPI_Flash_Read(&fontok,24,1);//读出字库标志位,看是否存在字库
	if(fontok!=0XAA)return 1;
	SPI_Flash_Read((u8*)tempsys,0,8);//读出8个字节   
	UNI2GBKADDR=tempsys[0];					  
	//printf("tempsysgbk[0]:%d\n",tempsys[0]);
	//printf("tempsysgbk[1]:%d\n",tempsys[1]);

	SPI_Flash_Read((u8*)tempsys,8,8);//读出8个字节   
	//printf("tempsysf16[0]:%d\n",tempsys[0]);
	//printf("tempsysf16[1]:%d\n",tempsys[1]);
	FONT16ADDR=tempsys[0];
	
	SPI_Flash_Read((u8*)tempsys,16,8);//读出8个字节   
	//printf("tempsysf12[0]:%d\n",tempsys[0]);
	//printf("tempsysf12[1]:%d\n",tempsys[1]);
	FONT12ADDR=tempsys[0];
	return 0;	 
}






