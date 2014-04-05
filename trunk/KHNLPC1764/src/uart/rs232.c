/*
 * rs232.c
 *
 *  Created on: 19-03-2014
 *      Author: MrTeo
 */
#include <string.h>
#include <LPC17xx.h>
#include "rs232.h"
#include "uart.h"
#include <stdio.h>
#include <stdarg.h>
#include "../sdcard/khn_gsht.h"
#include "../sdcard/Fatfs/ff.h"
#include "../sdcard/Fatfs/diskio.h"
char temp[200];

void Read232(char* str) { //"READ01031214#"
	//UART2_PrintString(str);
	int pclk;
		unsigned long int Fdiv;
	if (strncmp("READ", str, 4) == 0) {
		//UART2_Init(115200);

		//strncpy(Buff232.type, str + 4, 2);
#if _DEBUG==1
		UART2_PrintString("uart 115200");
#endif
	}
//		Buff232.type = str[4] * 10 + str[5] - 528;
//		Buff232.day = str[6] * 10 + str[7] - 528;
//		Buff232.month = str[8] * 10 + str[9] - 528;
//		Buff232.year = str[10] * 10 + str[11] - 528;
//		strncpy(Buff232.type, str + 4, 2);
//		strncpy(Buff232.day, str + 6, 2);
//		strncpy(Buff232.month, str + 8, 2);
//		strncpy(Buff232.year, str + 10, 2);

		//
//		strncpy(Buff232.ddmmyy, str + 6, 6);

//		UART2_PrintString(Buff232.type);
//		UART2_PrintString(Buff232.ddmmyy);
		//DebugPrint("type %d, day %d, month %d, year %d\r\n", Buff232.type,
		//		Buff232.day, Buff232.month, Buff232.year);

		//call function to get all data from sdcard
//		UART2_PrintString("Put function get data in sdcard here \r\n");
//
//		ReadData(Buff232.type, Buff232.ddmmyy, "170000");
//
//		//slit string
//
//		//put this one outside of this function
//		char* dataCollection;
//		char* data;
//		char* nextData;
//		char* line[250];
//		while (f_gets(line, sizeof line, &file)) {
//			dataCollection = strtok(line, "&");
//			sprintf(data,"%s%s",nextData,dataCollection[0]);
//			UART2_PrintString(data);
//			nextData = dataCollection[1];
//		}
//	}else{
//		UART2_PrintString("not read \r\n");
//	}
}
void cutString(void) {
	//check buff have special character or not

}
void Write232(char cmd, char *str) {
	sprintf(temp, "$GSHT,%d,%d,<%s>,", cmd, strlen(str), str);
	DebugPrint("%s%d# \r\n", temp, CheckSum(temp));
}

int CheckSum(char* str) {
	int sum = 0;
	while (*str) {
		sum += *str;
		str++;
	}
	return sum & 255;
}

void DebugPrint(const char *format, ...) {
	char tempUSB[200];
	va_list arg;
	va_start(arg, format);
	vsprintf(tempUSB, format, arg);
	va_end(arg);
	UART2_PrintString(tempUSB);
}



