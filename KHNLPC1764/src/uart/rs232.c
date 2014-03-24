/*
 * rs232.c
 *
 *  Created on: 19-03-2014
 *      Author: MrTeo
 */
#include <string.h>
#include "rs232.h"
#include "uart.h"
#include <stdio.h>
#include <stdarg.h>
char temp[200];

void Read232(char* str) { //"READ01031214#"
	if (strncmp("READ", str, 4) == 0 && strlen(str) == 13) {
		Buff232.type = str[4] * 10 + str[5] - 528;
		Buff232.day = str[6] * 10 + str[7] - 528;
		Buff232.month = str[8] * 10 + str[9] - 528;
		Buff232.year = str[10] * 10 + str[11] - 528;
		//DebugPrint("type %d, day %d, month %d, year %d\r\n", Buff232.type,
		//		Buff232.day, Buff232.month, Buff232.year);
	}
}

void Write232(char cmd, char *str) {
	sprintf(temp, "$GSHT,%d,%d,<%s>,", cmd, strlen(str), str);
	DebugPrint("%s%d# \r\n", temp,CheckSum(temp));
}

int CheckSum(char* str) {
	int sum = 0;
	while (*str) {
		sum += *str;
		str++;
	}
	return sum&255;
}

void DebugPrint(const char *format, ...) {
	char tempUSB[200];
	va_list arg;
	va_start(arg, format);
	vsprintf(tempUSB, format, arg);
	va_end(arg);
	UART2_PrintString(tempUSB);
}

