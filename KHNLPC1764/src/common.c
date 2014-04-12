#include "common.h"
#include "GPIO/GPIO.h"
#include <stdlib.h>
//local
unsigned int counter_send_gps;
//global
char data_gps[gpsLen];
unsigned int timer_gps;
unsigned int timer_send_gps;
unsigned int timer_read_sms;
volatile uint32_t msTicks;
struct MODEM flag_modem;
struct GPRS flag_gprs;

char rx_buffer1[RX_BUFFER_SIZE1];

//for key of car
void key_init() {
	GPIOSetDir(KEY_IN, INPUT);
	GPIOSetPull(KEY_IN, PULLUP);
}

void delay_ms(uint32_t dlyTicks) {
	uint32_t curTicks;

	curTicks = msTicks;
	while ((msTicks - curTicks) < dlyTicks)
		;
}
/*
 *
 *
 * How to user
 *
 *
 * int main(void) {
 int x;
 char* dataCollection;
 char line[256]="Khanhhoi;0913742108;213432534435;324123412341;0946309067;0913742108;213432534435;324123412341;324123412341";
 char* printData;
 UART2_Init(9600);
 UART2_PrintString("xxxx");
 //dataCollection = strtok(line, ";");

 get_data_from_flash(line);

 UART2_PrintString(flash_data.company);
 UART2_PrintString(flash_data.address);
 }
 *
 *
 *
 *
 */
//void get_data_from_flash(char data[256]) {
//	char * dataCollection;
//	dataCollection = strtok(data, ";");
//	int x;
//	for (x = 0; x < 9; x++) {
//		switch (x) {
//		//company
//		case 0:
//			flash_data.company = dataCollection;
//			break;
//			//address
//		case 1:
//			flash_data.address = dataCollection;
//			break;
//		case 2:
//			flash_data.vin_No = dataCollection;
//			break;
//		case 3:
//			flash_data.id_device = dataCollection;
//			break;
//		case 4:
//			flash_data.ownerName = dataCollection;
//			break;
//		case 5:
//			flash_data.so_gplx = dataCollection;
//			break;
//		case 6:
//			flash_data.license = dataCollection;
//			break;
//		case 7:
//			flash_data.license_iss_date = dataCollection;
//			break;
//
//		case 8:
//			flash_data.license_exp_date = dataCollection;
//			break;
//		}
//		dataCollection = strtok(NULL, ";");
//	}
//
//}
//char *strptime(const char *s, const char *format, struct tm *tm);

//Convert time by timezone
//HOW TO USE
//int main(void) {
//	char buf[20];
//	khn_time_type datetime;
//	UART2_Init(9600);
//	datetime = convert_data_time("120598", "214531", 7);
//	sprintf(buf, "Day:%i \r", datetime.day);
//	UART2_PrintString(buf);
//	sprintf(buf, "Day:%i \r", datetime.month);
//	UART2_PrintString(buf);
//	sprintf(buf, "Day:%i \r", datetime.year);
//	UART2_PrintString(buf);
//	sprintf(buf, "Day:%i \r", datetime.hour);
//	UART2_PrintString(buf);
//	sprintf(buf, "Day:%i \r", datetime.min);
//	UART2_PrintString(buf);
//	sprintf(buf, "Day:%i \r", datetime.sec);
//	UART2_PrintString(buf);
//
//}
khn_time_type convert_data_time(char *date, char *times, int timezone) {
	time_t utc;
	struct tm* tm_local;
	khn_time_type return_time;
	char current_date[8];
	char current_time[8];

	struct tm tm;

	strcpy(current_date, date);
	strcpy(current_time, times);
	char tmp[3];
	//set data to UTC datetime
	sprintf(tmp, "%c%c", current_date[0], current_date[1]);
	tm.tm_mday = atoi(tmp);
	sprintf(tmp, "%c%c", current_date[2], current_date[3]);
	tm.tm_mon = atoi(tmp) - 1;
	sprintf(tmp, "%c%c", current_date[4], current_date[5]);
	tm.tm_year = atoi(tmp) + 100;
	sprintf(tmp, "%c%c", current_time[0], current_time[1]);
	tm.tm_hour = atoi(tmp);
	sprintf(tmp, "%c%c", current_time[2], current_time[3]);
	tm.tm_min = atoi(tmp);
	sprintf(tmp, "%c%c", current_time[4], current_time[5]);
	tm.tm_sec = atoi(tmp);

	utc = mktime(&tm);
	utc += (timezone * 60 * 60);
	tm_local = localtime(&utc);
	return_time.day = tm_local->tm_mday;
	return_time.month = tm_local->tm_mon + 1;
	return_time.year = (tm_local->tm_year + 1900);
	return_time.hour = tm_local->tm_hour;
	return_time.min = tm_local->tm_min;
	return_time.sec = tm_local->tm_sec;
	return return_time;
}
