#include "sys.h" 
#include "fontupd.h"
#include "lcd.h"
#include "text.h"
#include "M45PE161.H"
		   
/************************************************************************																										
** 函数功能: 读取SPI FLASH 中指定的汉字		
** 入口参数: code 字符指针开始
**			 从字库中查找出字模
**			 code 字符串的开始地址,ascii码
**			 mat  数据存放地址 size*2 bytes大小
** 出口参数: 无		
** 备    注: 无
************************************************************************/
void Get_HzMat(unsigned char *code,unsigned char *mat,u8 size)
{
	unsigned char qh,ql;
	unsigned char i;					  
	unsigned long foffset; 
	qh=*code;
	ql=*(++code);
	if(qh<0x81||ql<0x40||ql==0xff||qh==0xff)//非 常用汉字
	{   		    
	    for(i=0;i<(size*2);i++)*mat++=0x00;//填充满格
	    return; //结束访问
	}          
	if(ql<0x7f)ql-=0x40;//注意!
	else ql-=0x41;
	qh-=0x81;   
	foffset=((unsigned long)190*qh+ql)*(size*2);//得到字库中的字节偏移量  		  

	if(size==16)SPI_Flash_Read(mat,foffset+FONT16ADDR,32);
	else SPI_Flash_Read(mat,foffset+FONT12ADDR,24);     
} 
/************************************************************************																										
** 函数功能: 显示一个指定大小的汉字		
** 入口参数: x,y :汉字的坐标
**			 font:汉字GBK码
**			 size:字体大小
**			 mode:0,正常显示,1,叠加显示
** 出口参数: 无		
** 备    注: 无
************************************************************************/ 
void Show_Font(u8 x,u8 y,u8 *font,u8 size,u8 mode)
{
	u8 temp,t,t1;
	u8 y0=y;
	u8 dzk[32];
	u16 tempcolor;

	Get_HzMat(font,dzk,size);//得到相应大小的点阵数据
	if(mode==0)//正常显示
	{	 
	    for(t=0;t<size*2;t++)
	    {   												   
		    temp=dzk[t];//得到12数据                          
	        for(t1=0;t1<8;t1++)
			{
				if(temp&0x80)LCD_DrawPoint(x,y);
	 			else 
				{
					tempcolor=POINT_COLOR;
					POINT_COLOR=BACK_COLOR;
					LCD_DrawPoint(x,y);
					POINT_COLOR=tempcolor;//还原
				}
				temp<<=1;
				y++;
				if((y-y0)==size)
				{
					y=y0;
					x++;
					break;
				}
			}  	 
    	} 
	}else//叠加显示
	{
	    for(t=0;t<size*2;t++)
	    {   												   
		    temp=dzk[t];//得到12数据                          
	        for(t1=0;t1<8;t1++)
			{
				if(temp&0x80)LCD_DrawPoint(x,y);   
				temp<<=1;
				y++;
				if((y-y0)==size)
				{
					y=y0;
					x++;
					break;
				}
			}  	 
    	} 
	}    
}
/************************************************************************																										
** 函数功能: 在指定位置开始显示一个字符串		
** 入口参数: x,y :汉字的坐标 	    
**	 		 支持自动换行
**	 		 (x,y):起始坐标
**	  		 size :字体大小
** 			str  :字符串
** 出口参数: 无		
** 备    注: 无
************************************************************************/ 
   			   
void Show_Str(u8 x,u8 y,u8*str,u8 size,u8 mode)
{												  	  
    u8 bHz=0;     //字符或者中文  	    				    				  	  
    while(*str!=0)//数据未结束
    { 
        if(!bHz)
        {
	        if(*str>0x80)bHz=1;//中文 
	        else              //字符
	        {      
                if(x>(LCD_W-size/2))//换行
				{				   
					y+=size;
					x=0;	   
				}							    
		        if(y>(LCD_H-size))break;//越界返回      
		        if(*str==13)//换行符号
		        {         
		            y+=size;
					x=0;
		            str++; 
		        }  
		        else LCD_ShowChar(x,y,*str,size,mode);//有效部分写入 
				str++; 
		        x+=size/2; //字符,为全字的一半 
	        }
        }else//中文 
        {     
            bHz=0;//有汉字库    
            if(x>(LCD_W-size))//换行
			{	    
				y+=size;
				x=0;		  
			}
	        if(y>(LCD_H-size))break;//越界返回  						     
	        Show_Font(x,y,str,size,mode); //显示这个汉字,空心显示 
	        str+=2; 
	        x+=size;//下一个汉字偏移	    
        }						 
    }   
}
/************************************************************************																										
** 函数功能: 得到字符串的长度		
** 入口参数: str  :字符串 	    		
** 出口参数: 无		
** 备    注: 无
************************************************************************/  			 		  
u16 my_strlen(u8*str)
{
	u16 len=0;
	while(*str!='\0')
	{
		str++;
		len++;
	}
	return len;
}
/************************************************************************																										
** 函数功能: 在str1后面加入str2		
** 入口参数: str1 ，str2  :字符串 	    		
** 出口参数: 无		
** 备    注: 无
************************************************************************/  
void my_stradd(u8*str1,u8*str2)
{				  
	while(*str1!='\0')str1++;
	while(*str2!='\0')
	{
		*str1=*str2;
		str2++;
		str1++;
	}
	*str1='\0';//加入结束符  		 
}
/************************************************************************																										
** 函数功能: 在指定宽度的中间显示字符串	如果字符长度超过了len,则用Show_Str显示	
** 入口参数: len:指定要显示的宽度	    		
** 出口参数: 无		
** 备    注: 针对12*12字体!!!
************************************************************************/   
void Show_Str_Mid(u8 x,u16 y,u8*str,u8 size,u8 len)
{
	u16 strlenth=0;
   	strlenth=my_strlen(str);
	strlenth*=size/2;
	if(strlenth>len)Show_Str(x,y,str,size,1);
	else
	{
		strlenth=(len-strlenth)/2;
	    Show_Str(strlenth+x,y,str,size,1);
	}
}   





