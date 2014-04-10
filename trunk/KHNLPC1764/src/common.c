#include "common.h"
#include "GPIO/GPIO.h"
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
