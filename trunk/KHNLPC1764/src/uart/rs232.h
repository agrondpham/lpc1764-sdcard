/*
 * rs232.h
 *
 *  Created on: 19-03-2014
 *      Author: MrTeo
 */

#ifndef RS232_H_
#define RS232_H_

struct Print232 {
	int day, month, year;
	int type;
} Buff232;

enum DataType            /* Defines an enumeration type    */
{
	TatCa=0,
	DuLieu =1,
	TGLaiXe=2,
	DungDo =3,
	DongMoCua =4,
	HanhTrinh =5,
	VanToc=6
} typeData;

int CheckSum( char* str);
void Read232( char* str);
void Write232( char cmd, char *str);
void DebugPrint(const char *format, ...);
#endif /* RS232_H_ */
