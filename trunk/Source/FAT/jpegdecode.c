#include "jpegbmp.h"    
#include "lcd.h"

   	
FileInfoStruct *CurFile;

typedef struct
{			 
	u32 ImgWidth; 
	u32 ImgHeight;

	u32 Div_Fac;  
	
	u32 S_Height; 
	u32 S_Width;
	
	u32	S_XOFF;	
	u32 S_YOFF;

	u32 staticx;  
	u32 staticy;
}PIC_POS;
		  
PIC_POS PICINFO;
unsigned char jpg_buffer[1024];//Buffer for image
/////////////////////////////////////////////////////    

u8* GetFileName(FileInfoStruct *FileName)
{	
	u8* filename;
	PICINFO.S_Width=240;
	CurFile=FileName;
	filename=CurFile->F_Name;
	return filename;

}
 