#include "fontupd.h"
#include "sys.h"
#include "fat.h"
#include "M45PE161.h"
#include "lcd.h"
#include "delay.h"
#include "TEXT.H"
#include "jpegbmp.h"

u32 PIC_Cluster = 0;
//ϵͳ�ļ���		  
const unsigned char *folder[2]=
{
"MUSIC",
"FONT",	  
};

//ϵͳ�ļ�����

//�ֽ�0~3,  ��¼UNI2GBKADDR;�ֽ�4~7  ,��¼UNI2GBKADDR�Ĵ�С
//�ֽ�8~11, ��¼FONT16ADDR ;�ֽ�12~15,��¼FONT16ADDR�Ĵ�С
//�ֽ�16~19,��¼FONT12ADDR ;�ֽ�20~23,��¼FONT12ADDR�Ĵ�С
//������,��100K����ʼ
//100K,���UNICODE2GBK��ת����	

u32 FONT16ADDR ;//16�����ŵĵ�ַ
u32 FONT12ADDR ;//12�����ŵĵ�ַ
u32 UNI2GBKADDR;//UNICODE TO GBK ���ŵ�ַ	  	 

#ifdef EN_UPDATE_FONT
u8 temp[512];  //��ʱ�ռ�
FileInfoStruct F_Info1[5];
u8 PIC_Display(void)
{
	u32 fcluster=0;	   		 
	FileInfoStruct FileTemp;//��ʱ�ļ��� 

//	u16 index=1;  //��ǰѡ����ļ����		  				    	 
	//�õ���Ŀ¼�Ĵغ�
	if(FAT32_Enable)fcluster=FirstDirClust;
	else fcluster=0;			   
	FileTemp=F_Search(fcluster,(unsigned char *)folder[0],T_FILE);//����MUSIC�ļ���	  
	if(FileTemp.F_StartCluster==0)return 1;						  //ϵͳ�ļ��ж�ʧ			  
	{
		PIC_Cluster = FileTemp.F_StartCluster;      //�����ļ��дغ�	
		Show_Str(60,150,"�ҵ�MUSIC�ļ���",16,0);
		delay_ms(500);
			
		    
	}		    	
	return 0;//�ɹ�
}
#endif

//����������õ������ַ
//��ϵͳʹ�ú���֧��֮ǰ,������øú���
//����FONT16ADDR,FONT12ADDR,UNI2GBKADDR;  
u8 Font_Init(void)
{
	u32 tempsys[2];//��ʱ��¼�ļ���ʼλ�ú��ļ���С
	u8 fontok=0;
	SPI_Flash_Read(&fontok,24,1);//�����ֿ��־λ,���Ƿ�����ֿ�
	if(fontok!=0XAA)return 1;
	SPI_Flash_Read((u8*)tempsys,0,8);//����8���ֽ�   
	UNI2GBKADDR=tempsys[0];					  
	//printf("tempsysgbk[0]:%d\n",tempsys[0]);
	//printf("tempsysgbk[1]:%d\n",tempsys[1]);

	SPI_Flash_Read((u8*)tempsys,8,8);//����8���ֽ�   
	//printf("tempsysf16[0]:%d\n",tempsys[0]);
	//printf("tempsysf16[1]:%d\n",tempsys[1]);
	FONT16ADDR=tempsys[0];
	
	SPI_Flash_Read((u8*)tempsys,16,8);//����8���ֽ�   
	//printf("tempsysf12[0]:%d\n",tempsys[0]);
	//printf("tempsysf12[1]:%d\n",tempsys[1]);
	FONT12ADDR=tempsys[0];
	return 0;	 
}






