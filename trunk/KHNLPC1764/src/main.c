//*****************************************************************************
//   +--+       
//   | ++----+   
//   +-++    |  
//     |     |  
//   +-+--+  |   
//   | +--+--+  
//   +----+    Copyright (c) 2009-11 Code Red Technologies Ltd.
//
// UART example project for RDB1768 development board
//
// Software License Agreement
// 
// The software is owned by Code Red Technologies and/or its suppliers, and is 
// protected under applicable copyright laws.  All rights are reserved.  Any 
// use in violation of the foregoing restrictions may subject the user to criminal 
// sanctions under applicable laws, as well as to civil liability for the breach 
// of the terms and conditions of this license.
// 
// THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
// OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
// USE OF THIS SOFTWARE FOR COMMERCIAL DEVELOPMENT AND/OR EDUCATION IS SUBJECT
// TO A CURRENT END USER LICENSE AGREEMENT (COMMERCIAL OR EDUCATIONAL) WITH
// CODE RED TECHNOLOGIES LTD. 
//
//*****************************************************************************
///
//
//ATu <14/04/05,08:57:59;1052.0366;10641.1721;0;0;0>
//$$KHN$3.13,56,359772037898310,1052.0366,N,10641.1721,E,0,100,0,0,1,CHUA CAP NHAP,000,1,0,14/04/05,08:56:52$

#include "LPC17xx.h"
#include <cr_section_macros.h>
#include <NXP/crp.h>
//#include "uart.h"
//#include "timer.h"
//#include "rs232.h"
#include "sdcard/khn_gsht.h"
#include "sdcard/Fatfs/ff.h"
#include "sdcard/Fatfs/diskio.h"

#include <stdlib.h>
#include <stdio.h>
#include "IAP/iap.h"
#include "uart/uart.h"
#include "gpio/gpio.h"
#include "timer/timer.h"
#include "gsm/gsm.h"

#include "common.h"
#include "led/led.h"
#include "gps/gps.h"
#include "Tcp/tcp.h"

/*	Flash sectors to be used for data storage */
#define	DATA_START_SECTOR	0x00018000	/* Start Sector 17 */
#define	DATA_END_SECTOR		0x0001FFFF	/* End Sector 17 */

#define IIR_RLS		0x03
#define IIR_RDA		0x02
#define LSR_RDR		0x01

__CRP const unsigned int CRP_WORD = CRP_NO_CRP;
///////////////////////////////////////

///this is used in main function

#define gsm						 	0
#define gps 						1

#define connect_error   3
#define timeLen 				4
#define ok_status   		2
#define gsm_bufferLen		200
#define gps_bufferLen		200
#define no_send     		0
#define complete    		0
#define so_vinLen      	20
#define ok_status   		2
#define data_serverLen  200
#define cstt_error  		0
#define ciicr_error 		1

extern int co_gps;
uint8_t rx_gps_index;
volatile uint32_t send_data_server;
volatile uint32_t toggle = 0;
volatile uint32_t couter0 = 0;
uint8_t data[100];
uint16_t rx_gsm_index;
unsigned char resend;

//char phone_1[phoneLen];

unsigned int interruptTimer_0;
char timeCheck[timeLen];
void start_up_gsm();
char gsm_buffer[gsm_bufferLen];
char gps_buffer[gps_bufferLen];
char data_server[data_serverLen];

unsigned int pointer_server = 0;
unsigned char rx_buffer_overflow1;
unsigned char rx_buffer_overflow0;
char version[] = "3.13";
unsigned char resend;
unsigned int oil_value;
char so_vin[so_vinLen] = "";

char year_str[3];
char month_str[3];
char day_str[3];
char hour_str[3];
char min_str[3];
char sec_str[3];

char ten_laixe[ten_laixeLen] ;
char so_gplx[so_gplxLen] ;
char ngaycap_gplx[ngaycapLen] ;
char handen_gplx[handenLen] ;
char buffer[buffer_file];
char tencongty[tencongtyLen];
char diachi[diachiLen];
char bien_soxe[bien_soxeLen];
///////////////////////////////////////////

struct SYSTEM_FLAG flag_system;
struct time_gsm *timeSet;

/*------------------------------------------------------------------------------
 delays number of tick Systicks (happens every 1 ms)
 *------------------------------------------------------------------------------*/
void SysTick_Handler(void) {
	msTicks++;
	if (interruptTimer_0 < 1000)
		interruptTimer_0++;
	else {
		interruptTimer_0 = 0;
		toggle = ~toggle;
		GPIOSetValue(LED_STATUS, toggle);
		if (timer_gps < counter_gps)
			timer_gps++;
		else {
			timer_gps = 0;
			flag_system.request_data = 1;
			flag_system.gps_detect = 0; // moi lan request data thi coi nhu gps_detect=0
		}
		if (timer_send_gps < counter_send_gps)
			timer_send_gps++; // cho nay cai dat thong so thoi gian de kiem tra gps
		else {
			timer_send_gps = 0;
			flag_system.send_data = 1;     // send data o day
			flag_system.write_data = 1;
		}
		if (++timer_read_sms >= 10) {
			timer_read_sms = 0;
			flag_modem.read_sms = 1;
		}

	}
	if (timer_gprs < counter_timeout) {
		timer_gprs++;
		flag_system.timeout_gprs = 0;
	} else {
		timer_gprs = 0;
		flag_system.timeout_gprs = 1;
	}

	if (timer_check_sms < counter_timeout_sms) {
		timer_check_sms++;
		flag_system.timeout_sms = 0;
	} else
		flag_system.timeout_sms = 1;

}
int n;
char readData[11];
void UART2_IRQHandler(void) {
//	char c;
//	if((LPC_UART2->LSR & LSR_RDR) != 0){ // Nothing received so just block
//	c = LPC_UART2->RBR; // Read Receiver buffer register
//	}
//	UART2_Sendchar(c);
	uint8_t IIRValue, LSRValue;
	uint8_t data;
	//char readData[11];
	IIRValue = LPC_UART2 ->IIR;
	IIRValue >>= 1; /* skip pending bit in IIR */
	IIRValue &= 0x07; /* check bit 1~3, interrupt identification */
	if (IIRValue == IIR_RLS) /* Receive Line Status */
	{
		LSRValue = LPC_UART2 ->LSR;
		if (LSRValue & LSR_RDR) /* Receive Data Ready */
		{
			data = LPC_UART2 ->RBR;
		}
	} else if (IIRValue == IIR_RDA) /* Receive Data Available */
	{

		data = LPC_UART2 ->RBR;
		if (data == 0x25) {
			//UART2_PrintString(readData);
			//UART2_Sendchar('E');
			Read232(readData);
			//UART2_PrintString(readData);
			n = 0;
			//memset(readData, 0, sizeof(readData));
		} else {
			readData[n] = data;
			//UART2_Sendchar(data);

			//UART2_PrintString(n);
			n++;
		}
		//UART2_Sendchar(data);
	}

}
void UART0_IRQHandler(void) {
	uint8_t IIRValue, LSRValue;
	uint8_t data;
	unsigned char trangthai_gui;
	IIRValue = LPC_UART0 ->IIR;
	IIRValue >>= 1; /* skip pending bit in IIR */
	IIRValue &= 0x07; /* check bit 1~3, interrupt identification */
	if (IIRValue == IIR_RLS) /* Receive Line Status */
	{
		LSRValue = LPC_UART0 ->LSR;
		if (LSRValue & LSR_RDR) /* Receive Data Ready */
		{
			data = LPC_UART0 ->RBR;
			//UART2_Sendchar(temp);
		}
	} else if (IIRValue == IIR_RDA) /* Receive Data Available */
	{
		//sprintf(tempValue,";%d",flag_gprs.send_gprs_ok);
		//UART2_PrintString(tempValue);

		data = LPC_UART0 ->RBR;
#if _DEBUG==1
		UART2_Sendchar(data);
		UART3_Sendchar(data);
#endif
		if (data == 0x0A) {
			if (flag_system.start_rx_gsm == 2) {
				rx_buffer0[rx_gsm_index] = NULL;
				process_gsm_data();
				rx_gsm_index = 0;
				flag_system.start_rx_gsm = 0;
			} else if (rx_gsm_index == 0)
				flag_system.start_rx_gsm = 1;
			else if (rx_buffer0[0] == '+' && rx_buffer0[1] == 'C'
					&& rx_buffer0[2] == 'M' && rx_buffer0[3] == 'G'
					&& rx_buffer0[4] == 'R')
				flag_system.start_rx_gsm = 2;
			else {
				rx_buffer0[rx_gsm_index] = NULL;
				if (rx_buffer0[0] != NULL ) {
					process_gsm_data();
					rx_gsm_index = 0;
					flag_system.start_rx_gsm = 0;
				} else {
					flag_system.start_rx_gsm = 1;
				}
			}

		} else if (flag_system.start_rx_gsm != 0 && data != 0x0D) {
			rx_buffer0[rx_gsm_index] = data;
			if (++rx_gsm_index >= gsm_bufferLen) {
				rx_gsm_index = 0;
				process_gsm_data();
				flag_system.start_rx_gsm = 0;
			}
		}

	}

}

void UART1_IRQHandler(void) {
	uint8_t IIRValue, LSRValue;
	uint8_t data;
	IIRValue = LPC_UART1 ->IIR;
	IIRValue >>= 1; /* skip pending bit in IIR */
	IIRValue &= 0x07; /* check bit 1~3, interrupt identification */
	if (IIRValue == IIR_RLS) /* Receive Line Status */
	{
		LSRValue = LPC_UART1 ->LSR;
		if (LSRValue & LSR_RDR) /* Receive Data Ready */
		{
			data = LPC_UART1 ->RBR;
			//UART2_Sendchar(temp);
		}
	} else if (IIRValue == IIR_RDA) /* Receive Data Available */
	{
		data = LPC_UART1 ->RBR;
		//UART2_PrintString("?");
		if (flag_system.request_data) {
			if (data == '$') {
				flag_system.start_rx_gps = 1;
			} else if (data == '*') // && flag_system.start_rx_gps
					{
				flag_system.start_rx_gps = 0;
				rx_buffer1[rx_wr_index1] = NULL;

				if (rx_buffer1[0] == 'G' && rx_buffer1[1] == 'P'
						&& rx_buffer1[2] == 'R' && rx_buffer1[3] == 'M'
						&& rx_buffer1[4] == 'C') {

					process_gps_data();

				}
				del_buffer(gps);
			} else if (flag_system.start_rx_gps == 1) {
				rx_buffer1[rx_wr_index1] = data;
				if (++rx_wr_index1 > RX_BUFFER_SIZE1) {
					rx_wr_index1 = 0;
					flag_system.start_rx_gps = 0;

				}
//               if (++rx_counter1 == RX_BUFFER_SIZE1)
//                  {
//                  rx_counter1=0;
//                  rx_buffer_overflow1=1;
//                  };

			}

		}

	}
}

void TIMER0_IRQHandler(void) {
//
//    /* Set new timer interrupt, toggle led status and write value to the led port */
	uint8_t var_key, var_door;
	TIMER0_interrupt();
//	couter0++;
//	if (couter0 > 6) {
//		couter0 = 0;
//		toggle = ~toggle;
//		//GPIOSetValue(BUZZER, toggle);
//		//LPC_TIM0->IR = 1;			/* clear interrupt flag */
//	}

//    toggle = ~toggle;
//    GPIOSetValue(BUZZER, toggle);
//
	var_key = GPIOGetValue(KEY_IN);      //Read the button value
	var_door = GPIOGetValue(DOOR_IN);
	if (var_key == 0)      //KEY_IN
			{
		// GPIOSetValue(BUZZER, HIGH);
		flag_system.status_key = 2;
	} else {
		//GPIOSetValue(BUZZER, LOW);
		flag_system.status_key = 1;
	}
	if (var_door == 0)      //KEY_IN
			{
		//GPIOSetValue(BUZZER, HIGH);
		flag_system.status_door = 2;
	} else {
		//GPIOSetValue(BUZZER, LOW);
		flag_system.status_door = 1;
	}

}

void TIMER1_IRQHandler(void) {
//
//    /* Set new timer interrupt, toggle led status and write value to the led port */
	TIMER1_interrupt();
	couter0++;
	if (couter0 > 6) {
		couter0 = 0;
		toggle = ~toggle;
		//GPIOSetValue(BUZZER, toggle);
		//LPC_TIM0->IR = 1;			/* clear interrupt flag */
	}
//	toggle = ~toggle;
//	GPIOSetValue(BUZZER, toggle);

//    toggle = ~toggle;
//    GPIOSetValue(BUZZER, toggle);
}

///start EEPROM
//int writeFlash(char* data) {
//void writeFlash(char* data) {
//	int x;
//	unsigned char src_buffer[256];
//	if (u32IAP_PrepareSectors(17, 17) == IAP_STA_CMD_SUCCESS) {
//		/*	Copy data from RAM to Flash, number of bytes to be written should be 256|512|1024|4096 */
//		memset(src_buffer, 0, 256); //zero init src buffer
//		for (x = 0; x < 256; x++) {
//			src_buffer[x] = data[x]; // copy phone_1 into 256 bytes buffer
//		}
//		u32IAP_CopyRAMToFlash(DATA_START_SECTOR, (uint32_t)&src_buffer, sizeof(src_buffer));
////		if (u32IAP_CopyRAMToFlash(DATA_START_SECTOR, (uint32_t) &src_buffer,
////				sizeof(src_buffer)) == IAP_STA_CMD_SUCCESS) {
////			return 1;
////		} else {
////			return 0;
////		}
//	}
//
//}

//void clearFlash() {
//
//	u32IAP_EraseSectors(17, 17) == IAP_STA_CMD_SUCCESS;
//	delay_ms(100);
//	u32IAP_PrepareSectors(17, 17) == IAP_STA_CMD_SUCCESS;
//
////	if (u32IAP_PrepareSectors(17, 17) == IAP_STA_CMD_SUCCESS) {
////		if (u32IAP_EraseSectors(17, 17) == IAP_STA_CMD_SUCCESS) {
////			return 1;
////		} else {
////			return 0;
////		}
////	}
//
//}

//void CopyRAMToFlash(char* phone_save) {
//	int x;
//	//char* fone="0934131426";
//	//delay_ms(3000);
//	if (u32IAP_PrepareSectors(17, 17) == IAP_STA_CMD_SUCCESS) {
//		/*	Copy data from RAM to Flash, number of bytes to be written should be 256|512|1024|4096 */
//		unsigned char src_buffer[256]; // at least 256 bytes buffer to write into flash
//		memset(src_buffer, 0, 256); //zero init src buffer
//		for (x = 0; x < 256; x++) {
//			src_buffer[x] = phone_save[x]; // copy phone_1 into 256 bytes buffer
//		}
//		u32IAP_CopyRAMToFlash(DATA_START_SECTOR, (uint32_t) &src_buffer, 256); //sizeof(phone_1));
//	}
//	//UART2_PrintString("coppy_run \r\n");
//	/* Read back data from Flash */
//	//unsigned char phone_2[256];
//	//memcpy(phone_2, (void*)0x00018000,256);
//}

void EraseSectors() {
	if (u32IAP_EraseSectors(17, 17) == IAP_STA_CMD_SUCCESS) {
		UART2_PrintString("clear successful \r\n");
	} else {
		UART2_PrintString("clear unsuccessful \r\n");
	}
	if (u32IAP_PrepareSectors(17, 17) == IAP_STA_CMD_SUCCESS)
		UART2_PrintString("clear end \r\n");
}

//////END

void upload_info() {
	unsigned char x, j;
	unsigned char phone_2[256];
	memcpy(phone_2, (void*) 0x00018000, 256);
	delay_ms(100);
	for (x = 0; x < phoneLen; x++) {
		if (phone_2[x] == NULL || phone_2[x] == 0xFF)
			break;
		if (phone_2[x] > 0x39 || phone_2[x] < 0x30)
			break;
		phone_1[x] = phone_2[x];
	}
	phone_1[x] = NULL;
	if (phone_1[0] == NULL || phone_1[0] == 0xF)
		sprintf(phone_1, "000");
//// start phoneDrive
	for (j = 0; j < phoneLen; j++) {
		if (phone_1[j] == NULL || phone_1[j] == 0xFF)
			break;
		phoneDrive[j] = phone_1[j];
	}
	phoneDrive[j] = NULL;
	///end
	if (apn[0] == NULL || apn[0] == 0xff) {
//		sprintf(apn, "v-internet");
//		sprintf(userName, "");
//		sprintf(passApn, "");
		sprintf(apn, "m3-world");
		sprintf(userName, "mms");
		sprintf(passApn, "mms");
	}
	if (ipServer[0] == NULL || ipServer[0] == 0xff) {
		sprintf(ipServer, "112.213.84.16");
		sprintf(tcpPort, "11511");
	}
	if (timeCheck[0] == NULL || timeCheck[0] == 0xff)
		counter_send_gps = 20;		// gui len server

}
/////in1
unsigned char find_file(char *stopTime) {
	char filename[12];  // ex: 3050.txt
	char filename_stop[12];
	char path[30];
	char link[50];
	char ngay_in[10];
	char thoigian_in[10];
	char latitude_in[15];
	char longitude_in[15];
	char speed_in[5];
	unsigned char in_vantoc = 0;
	char dangvuot_tocdo = 0;
	unsigned char solan_dongcua = 0;
	unsigned char solan_vuot = 0;
	char key_in[3];
	//bit key_i = 0;
	//bit key_save = 0;

//
	float speed_tong;
	char start_data;
	unsigned char hour_start, min_start;
	unsigned char hour_stop, min_stop;
	unsigned char gio_laixe = 0, phut_laixe = 0, gio_laixe_in = 0,
			phut_laixe_in = 0;

	char tam[7];

	unsigned char res;
	unsigned int i, k, u, m, n, file_txt;

	flag_system.led_check = 1;     // chop led de bat dau doc file

	timeSet = (struct time_gsm *) stopTime; // kiem tra tu thoi diem hien tai tro ve truoc

	strncpy(year_str, timeSet->year, 2);
	strncpy(month_str, timeSet->month, 2);
	strncpy(day_str, timeSet->day, 2);
	strncpy(hour_str, timeSet->hour, 2);
	strncpy(min_str, timeSet->min, 2);
	strncpy(sec_str, timeSet->sec, 2);
	hour_stop = atoi(hour_str);
	min_stop = atoi(min_str);

	hour_start = 0;
	min_start = 0;

	if (hour_stop > 23 || min_stop > 59) {
		sprintf(buffer, "Gia tri cai dat thoi gian khong hop le\r");
		UART2_PrintString(buffer);
		delay_ms(1000);
		flag_system.led_check = 0;
		return 0;
	}
	sprintf(buffer, "%s\r", tencongty);
	UART2_PrintString(buffer);
	delay_ms(500);
	sprintf(buffer, "%s\r", diachi);
	UART2_PrintString(buffer);
	delay_ms(500);
	sprintf(buffer, "********************\r");
	UART2_PrintString(buffer);
	delay_ms(500);
	sprintf(buffer, "VIN: %s\r", so_vin);
	UART2_PrintString(buffer);
	delay_ms(500);
	sprintf(buffer, "BKS: %s\r", bien_soxe);
	UART2_PrintString(buffer);
	delay_ms(500);
	sprintf(buffer, "--------------------\r");
	UART2_PrintString(buffer);
	delay_ms(500);
	sprintf(buffer, "LX:%s\r", ten_laixe);
	UART2_PrintString(buffer);
	delay_ms(500);
	sprintf(buffer, "GPLX:%s\r", so_gplx);
	UART2_PrintString(buffer);
	delay_ms(500);
	sprintf(buffer, "Cap ngay:%s\r", ngaycap_gplx);
	UART2_PrintString(buffer);
	delay_ms(500);
	sprintf(buffer, "Han den:%s\r", handen_gplx);
	UART2_PrintString(buffer);
	delay_ms(500);
	sprintf(buffer, "--------------------\r");
	UART2_PrintString(buffer);
	delay_ms(500);
	sprintf(buffer, "Thoi diem in: %s:%s:%s\n\r\t      %s/%s/%s\r", hour_str,
			min_str, sec_str, day_str, month_str, year_str);

	UART2_PrintString(buffer);
	delay_ms(500);

//	sprintf(link, "0:/DEFAULT");

//	sprintf(filename, "00.txt");
//	sprintf(filename_stop, "%s.txt", min_str);
//	for (i = hour_start; i < hour_stop + 1; i++) // quet 24 gio de tim bat dau tu thoi diem k

//			{
//		sprintf(path, "%s/20%s/%s/%s/%02d", link, year_str, month_str, day_str,
//				i);
//		//res = f_opendir(&dir, path);
//
//		if (!res) {
//			// for(;;)
//			for (file_txt = 30; file_txt < 59; file_txt++) {
//				res = f_readdir(&dir, &finfo);
//				if ((res != FR_OK) || !finfo.fname[0])
//					break;
//				if (i == hour_stop) {
//					if (strcmp(&(finfo.fname[0]), filename_stop) > 0)
//						break;
//				}
//
//				if (strcmp(&(finfo.fname[0]), filename) > 0) // so sanh file (ten file = so phut) doc ra > file cai dat thi cho in ra
//						{
//					sprintf(path, "%s/20%s/%s/%s/%02d/%s", link, year_str,
//							month_str, day_str, i, &(finfo.fname[0]));
//
//					if ((res = f_open(&file, path, FA_READ)) != FR_OK)
//						return 1;
//
//					if ((res = f_read(&file, buffer, sizeof(buffer) - 1,
//							&nbytes)) == FR_OK) {
//						buffer[nbytes + 1] = NULL;
//						n = 0;
//						for (m = 0; m < nbytes + 1; m++)  // doc du lieu tu file
//								{
//
//							if (buffer[m] == NULL || buffer[m] == ' ')
//								break;
//
//							if (buffer[m] == '<')     // bat dau chuoi
//									{
//								start_data = 1;
//								n = m + 1;
//
//							} else if (buffer[m] == '>')     // ket thuc chuoi
//									{
//								start_data = 0;
//								data_gps[m - n] = NULL;
//								for (k = 0; k < gpsLen; k++) // lay ngay thang nam
//										{
//									if (data_gps[k] == ',') {
//										ngay_in[k] = NULL;
//										break;
//									}
//									ngay_in[k] = data_gps[k];
//								}       // end cua ngay
//								u = k + 1;
//								for (k = u; k < gpsLen; k++)    // lay thoi gian
//										{
//									if (data_gps[k] == ';') {
//										thoigian_in[k] = NULL;
//										break;
//									}
//									thoigian_in[k - u] = data_gps[k];
//								}       // end cua thoi gian
//								u = k + 1;
//								for (k = u; k < gpsLen; k++)    // lay lat
//										{
//									if (data_gps[k] == ';') {
//										latitude_in[k - u] = NULL;
//										break;
//									}
//									latitude_in[k - u] = data_gps[k];
//
//								}
//								u = k + 1;
//								for (k = u; k < gpsLen; k++)   // lay long
//										{
//									if (data_gps[k] == ';') {
//										longitude_in[k - u] = NULL;
//										break;
//									}
//
//									longitude_in[k - u] = data_gps[k];
//
//								}
//								u = k + 1;
//								for (k = u; k < gpsLen; k++)   // lay speed
//										{
//									if (data_gps[k] == ';') {
//										speed_in[k - u] = NULL;
//										break;
//									}
//									speed_in[k - u] = data_gps[k];
//								}
//								u = k + 1;
//								for (k = u; k < gpsLen + 1; k++)   // lay door
//										{
//									if (data_gps[k] == ';') {
//										key_in[k - u] = NULL;
//										// door_in[k-u] = NULL;
//										break;
//									}
//									key_in[k - u] = data_gps[k];   // door
//								}
//								speed_tong = atoi(speed_in) * 1.852;
//
//								if (in_vantoc < 5) {
//
//									delay_ms(300);
////									if (speed_tong > speed_max) {
////										solan_vuot++;
////										sprintf(path, "%f", speed_tong); // muon tam path de dung thoi gian
////										for (k = 0; k < 5; k++) {
////											if (path[k] == '.'|| path[k]==NULL)break;
////											tam[k] = path[k];
////											// on_off_buzz=ON;       // bao coi
////											// delay_ms(50);
////											// on_off_buzz=OFF;
////										}
////										tam[k] = NULL;
////										sprintf(sms_reply,
////												"Lan %d: %s\n\r%s,%s\n\rTDCP/TDTT:%d/%03d km/h\r ",
////												solan_vuot, thoigian_in,
////												latitude_in, longitude_in,
////												speed_max, atoi(tam));
////										print_data(sms_reply);
////										in_vantoc++;
////										if (in_vantoc == 10) {
////											delay_ms(200);
////											sprintf(buffer,
////													"--------------------\r");
////											print_data(buffer);
////											delay_ms(200);
////										}
////										memset(sms_reply, 160, NULL );
////										//  on_off_buzz=ON;       // bao coi
////										//    delay_ms(50);
////										//    on_off_buzz=OFF;
////
////									}
//
//								}
//								//key_i = atoi(key_in);
//
//								//if (key_i != key_save) {
//								//	key_save = key_i;
//									//solan_momay++;
//								//	solan_dongcua++;
//								//}
//								//dongcua = solan_dongcua;
//								//vuot_km = in_vantoc;
//
//							}   // end ket thuc chuoi
//
//							else if (start_data) {
//								data_gps[m - n] = buffer[m];
//								if ((m - n) > 50)
//									break; // du lieu dai bat thuong thi coi nhu du lieu loi
//							}
//
//						//next_read:   // cho nay them vao 24/01/13
//
//					} // end for
//
//				}
//			}
//			if (file_txt == 59)
//				break;
//		}
//
//	}
//
//}

}
/////end

void init_program(void) {
TimerInit(0, TIMER0_INTERVAL);
TimerInit(1, TIMER1_INTERVAL);
enable_timer(0);
enable_timer(1);
if (SysTick_Config(SystemCoreClock / 1000)) //{ /* Setup SysTick Timer for 1 msec interrupts  */
		{
	while (1)
		; /* Capture error */
}

UART0_Init(9600);
UART1_Init(9600);
UART2_Init(9600);
UART3_Init(9600);
KHN_SDCARD_INIT();
LED_Init();
BUZZER_Init();
key_init();
gsm_on();
}
//int main(void) {
//	int x;
//	char* dataCollection;
//	char line[256]="Khanhhoi;0913742108;213432534435;324123412341;0946309067;0913742108;213432534435;324123412341;324123412341";
//	char* printData;
//	UART2_Init(9600);
//	UART2_PrintString("xxxx");
//	//dataCollection = strtok(line, ";");
//
//	get_data_from_flash(line);
//
//	UART2_PrintString(flash_data.company);
//	UART2_PrintString(flash_data.address);
//}

int main(void) {
uint8_t location;
uint8_t buffer[200];

uint8_t vitri, resend_gprs, counter_init = 0, i;
unsigned char a, b, c, d;
char day, month, year, hour, min, sec; //, week_day;

char hour_tam[3];

char min_tam[3];
char sec_tam[3];
char year_tam[3];
char month_tam[3];
char day_tam[3];
char month_tam_1[3];
init_program();
//delay_ms(500);
//Init_SomeThing();
//LED_Config();
delay_ms(500);
upload_info();
//delay_ms(500);
//start_up_gsm();

//flash_led();

for (i = 0; i < 5; i++) {
	flash_led();
	delay_ms(1000);
}
GPIOSetValue(BUZZER, HIGH);
delay_ms(50);
GPIOSetValue(BUZZER, LOW);
flag_modem.flash_coppy = 1;

//disk_initialize(0);
while (1) {
	if (flag_modem.modem == not_connect) {
		start_up_gsm();
		///if(flag_modem.flash_coppy == 1)
		//{
		//flag_modem.flash_coppy = 0;
		//CopyRAMToFlash(phone_1);
		//}
		delay_ms(1000);
		init_gsm();
		//UART3_Sendchar('A');
		//UART3_PrintString("ket thuc main vao while");
		if (flag_gprs.status_gprs != ok_status) {

			flag_system.led_check = 1;
			flag_gprs.status_gprs = init_tcp(apn, userName, passApn,
					flag_gprs.status_gprs);
			resend++;
			if (resend >= 2 && flag_gprs.status_gprs != ok_status) {
				flag_gprs.deactive_pdp = 1;
				resend = 0;
			} else if (flag_gprs.status_gprs == ok_status)
				counter_init = 0;
			flag_system.led_check = 0;
		}
	}
	if (flag_modem.modem == connected) {
		if (flag_modem.error == 1)
			flag_modem.modem = not_connect;
		else
			flag_modem.modem = connected;
		if (flag_system.read_sms == 1) {
			UART0_PrintString("AT+CMGR=1\r");
			flag_system.read_sms = 0;
			delay_ms(1000);
		} else if (flag_modem.cmgr == 1) {
			flag_modem.cmgr = 0;
			//clearFlash();
			EraseSectors();
			if (strlen(userCall_new) > 10)
				process_command();
			delay_ms(1000);
			send_sms_func(sms_reply);
			delay_ms(1000);
			UART0_PrintString("AT+CMGDA=\"DEL ALL\"\r");
			flag_system.send_data = 0;
			delay_ms(1000);
			//writeFlash(phone_1);
			write_basic_infor(phone_1);
			//CopyRAMToFlash(phone_1);
			delay_ms(500);
			UART0_PrintString("AT\n\r");
			delay_ms(1000);
			flag_modem.modem = not_connect;
			flag_modem.flash_coppy = 1;

		} else if (flag_system.send_data_flag == smsMode) {
			flag_system.send_data_flag = 0;
			flag_system.send_data = 0;
			send_sms_func(sms_reply);
			delay_ms(1000);
			UART0_PrintString("AT+CMGDA=\"DEL ALL\"\r");
			delay_ms(3000);
		} else if (flag_modem.ring) {
			ring_func();
			for (i = 0; i < phoneLen; i++) {
				if (phone_1[i] == NULL || phone_1[i] == 0xFF)
					break;
				phoneDrive[i] = phone_1[i];
			}
			phoneDrive[i] = NULL;

		} else if (flag_modem.upinfo == 1) {
			flag_modem.upinfo = 0;	// test
			//upload_info();
		}
		if (flag_system.send_data || flag_system.change_status) {

			if (flag_gprs.deactive_pdp) {
				sprintf(buffer, "%c", ESC);
				UART0_PrintString(buffer);
				delay_ms(1000);
				UART0_PrintString("AT+CIPSHUT\r");
				delay_ms(1000);
				if (++counter_init >= 5)    // test
						{
					counter_init = 0;

					start_up_gsm();
					flag_modem.modem = not_connect;
				}
				flag_gprs.deactive_pdp = 0;
				flag_gprs.connect_ok = 0;
				flag_gprs.status_gprs = cstt_error;
			} else if (ipServer[0] != NULL
					&& ipServer[0] != 0xFF&& tcpPort[0] != NULL) // thieu xac nhan cac thong so
							{for (vitri = 0; vitri < strlen(imei); vitri++) // kiem tra thanh phan ky tu trong imei
					{             // dung tam bien vitri de lam bien dem
						if (imei[vitri] < 0x30 || imei[vitri] > 0x39) {
							if (check_imei() == 0) // check lai imei neu thay ok thi bat dau gui
							{
								vitri = 0;      // gan i = 0 de cho gui tiep
							} else
							vitri = 1;    // gan = 1 de stop gui
							break;
						}
					}
					if (vitri == 0 || vitri == strlen(imei)) // neu = 0 thi cho gui vi imei dung
					{
//							sprintf(data_gps,
//									"$%s,56,%s,%s,N,%s,E,%s,%d,%d,%d,%d,%s,%s,%d,%d,%s,%s$\r\n",
//									version, imei, latitude, longitude,
//									speed_gps, oil_value,
//									flag_system.status_door,
//									flag_system.status_key,
//									flag_system.card_status, so_vin, phone_1,
//									flag_system.cold_hot, flag_system.sleep,gps_date_string,
//									gps_time_string);
						// trang thai IN3

//							switch (WriteData(1, 17022014, 160000)) {
//								case 1:
//								break;
//								case 2:
//								break;
//							}
//							f_puts(data_gps, &file);
//#if _DEBUG==1
//								UART2_PrintString("Write sdcard\r\n");
//#endif
//								//f_puts(" The gioi that rong lon\r\n",&file);
//								Close();
//								// sdcard end
//
						if (connectServer(ipServer, tcpPort) == ok_status) {
							flag_gprs.connect_ok = 1;   // ket noi ok

							memset(data_gps, gpsLen, NULL);// xoa NULL het GPS data

							sprintf(data_gps,
									"$$KHN$%s,56,%s,%s,N,%s,E,%s,%d,%d,%d,%d,%s,%s,%d,%d,%s$\r\n",
									version, imei, latitude, longitude,
									speed_gps, oil_value,
									flag_system.status_key,
									flag_system.status_door,
									flag_system.card_status, so_vin, phone_1,
									flag_system.cold_hot, flag_system.sleep,time_gps);//,gps_time_string);//gps_date_string,
							//gps_time_string);//time_gps_send);
							if (send_data_gprs(data_gps) != ok_status)// send data
							{
								//UART2_PrintString("loi xay ra\r\n");
								flag_gprs.deactive_pdp = 1;// neu send bi loi thi reset lai gprs
							}
							//UART2_PrintString("Khong loi khi gui \r\n");
							delay_ms(1000);
							UART0_PrintString("AT+CIPCLOSE\r");
							resend_gprs = 0;
							flag_gprs.connect_ok = 0;
						} else {
							flag_gprs.connect_ok = 0;  // ket noi bi loi
							flag_gprs.deactive_pdp = 1;
						}
					}   // end cua vitri=0
///start_sdcar
//						switch (WriteData(1, 17022014, 160000)) {
//							case 1:
//							break;
//							case 2:
//							break;
//						}
//						if(f_puts(data_gps, &file)>-1){
//							Close();
//#if _DEBUG==1
//							UART2_PrintString("Write sdcard\r\n");
//#endif
//						}else{
//#if _DEBUG==1
//							UART2_PrintString("CAN NOT Write sdcard\r\n");
//#endif
//						}

					//f_puts(" The gioi that rong lon\r\n",&file);

					////
				}

				flag_system.send_data = 0;
				flag_system.change_status = 0;

			}

		}

			//// timer
	if (flag_system.write_data == 1) {

		if (flag_system.gps_detect) {
			if (strlen(gps_time_string) != 6 || strlen(gps_date_string) != 6) {
				UART2_PrintString("TIMER_ERROR\r");
				delay_ms(1000);
				goto exit_write_data;
			}
			hour_tam[0] = gps_time_string[0];
			hour_tam[1] = gps_time_string[1];
			hour_tam[2] = NULL;

			min_tam[0] = gps_time_string[2];
			min_tam[1] = gps_time_string[3];
			min_tam[2] = NULL;

			sec_tam[0] = gps_time_string[4];
			sec_tam[1] = gps_time_string[5];
			sec_tam[2] = NULL;

			day_tam[0] = gps_date_string[0];
			day_tam[1] = gps_date_string[1];
			day_tam[2] = NULL;

			month_tam[0] = gps_date_string[2];
			month_tam[1] = gps_date_string[3];
			month_tam[2] = NULL;

			year_tam[0] = gps_date_string[4];
			year_tam[1] = gps_date_string[5];
			year_tam[2] = NULL;

			if (atoi(hour_tam) > 23 || atoi(min_tam) > 59 || atoi(sec_tam) > 59
					|| atoi(day_tam) > 31 || atoi(month_tam) > 12
					|| atoi(year_tam) > 99) {

				memset(data_gps, gpsLen, NULL );
				sprintf(data_gps, "ERROR: %s %s\r", gps_time_string,
						gps_date_string);
				UART2_PrintString(data_gps);
				delay_ms(1000);
				goto exit_write_data;
			}
			a = atoi(hour_tam) + 7;
			if (a >= 24)        // neu gio vuot qua 1 ngay moi
					{
				a = a - 24;    // gio
				b = atoi(day_tam);
				b++;           // ngay
				c = atoi(month_tam);  // thang
				d = atoi(year_tam);
				switch (atoi(month_tam)) {
				case 1:
					if (b > 31) {
						b = 1;
						c++;
					}
					break;
				case 2:
					if (b > 29) {
						b = 1;
						c++;
					}
					break;
				case 3:
					if (b > 31) {
						b = 1;
						c++;
					}
					break;
				case 4:
					if (b > 30) {
						b = 1;
						c++;
					}
					break;

				case 5:
					if (b > 31) {
						b = 1;
						c++;
					}
					break;

				case 6:
					if (b > 30) {
						b = 1;
						c++;
					}
					break;
				case 7:
					if (b > 31) {
						b = 1;
						c++;
					}
					break;

				case 8:
					if (b > 31) {
						b = 1;
						c++;
					}
					break;

				case 9:
					if (b > 30) {
						b = 1;
						c++;
					}
					break;
				case 10:
					if (b > 31) {
						b = 1;
						c++;
					}
					break;

				case 11:
					if (b > 30) {
						b = 1;
						c++;
					}
					break;
				case 12:
					if (b > 31) {
						b = 1;
						c++;
						d++;
					}
					break;

				}

				sprintf(time_gps, "%02d/%02d/%02d,%02d:%s:%s", d, c, b, a,
						min_tam, sec_tam);
				year = d;
				month = c;
				day = b;
				hour = a;
				min = atoi(min_tam);
				sec = atoi(sec_tam);
			} else {
				sprintf(time_gps, "%s/%s/%s,%02d:%s:%s", year_tam, month_tam,
						day_tam, a, min_tam, sec_tam);
				hour = a;
				min = atoi(min_tam);
				sec = atoi(sec_tam);
				day = atoi(day_tam);
				month = atoi(month_tam);
				year = atoi(year_tam);
			}
			memset(data_gps, gpsLen, NULL );
			sprintf(data_gps, "AT+CCLK=\"%s+00\"\r", time_gps);
			UART0_PrintString(data_gps);
			//UART2_PrintString(time_gps);
			delay_ms(500);
			// UART2_PrintString(time_gps);

		} else {
			UART0_PrintString("at+cclk?\r");
			delay_ms(1000);
		}
//		                flag_system.status_door =! door;
//		                flag_system.status_key =! key;

		for (vitri = 0; vitri < gpsLen; vitri++)
			data_gps[vitri] = NULL;
		sprintf(data_gps, "<%s;%s;%s;%s;%d;%d>\r\n", time_gps, latitude,
				longitude, speed_gps, flag_system.status_key,
				flag_system.status_door);
		//printf("ATu %s\r", data_gps);

		//SDCARD

//		               //  i = init_mmc();
//
//		                   if (!i)
//		                    {
//		                        a = creat_file(time_gps);
//		                        card_detect=1;
//		                        flag_system.card_status=1;
//		                        if (a!=0)
//		                        {
//		                            printf("ATc+%d\r", a);
//		                            delay_ms(1000);
//		                            flag_system.card_status=0;
//		                            card_detect=0;
//		                        }
//		                    }
//		                    else
//		                    {
//		                        card_detect=0;
//		                        printf("AT2+3\r");    // trang thai loi card
//		                        delay_ms(1000);
//		                        flag_system.card_status=0;
//
//		                    }

		///start_sdcar
		switch (WriteData(1, 17022014, 160000)) {
		case 1:
			break;
		case 2:
			break;
		}
		if (f_puts(data_gps, &file) > -1) {
			Close();
			flag_system.card_status = 1;
#if _DEBUG==1
			UART2_PrintString("Write sdcard\r\n");

#endif
		} else {
			flag_system.card_status = 0;
#if _DEBUG==1
			UART2_PrintString("CAN NOT Write sdcard\r\n");

#endif
		}  //END
		   //READ sdcard
//			switch (ReadData(1, 17022014, 160000)) {
//			case 1:
//				break;
//			case 2:
//				break;
//			}
//			char buffer[256];
//			while (f_gets(buffer, sizeof buffer, &file)) {
//				UART2_PrintString(buffer);
//			}
//
//			////////////////END
		exit_write_data: flag_system.write_data = 0;
		timer_send_gps = 0; // xoa thoi gian gui len data va cho ghi vao the nho

		delay_ms(100);
	}
	//////////end

}
}

//int main(void) {
//	char line[82];
//	InitPort();
//	SystemInit();
//	KHN_SDCARD_INIT();
//	init_timer(TIMER0_INTERVAL);
//	enable_timer( 0 );
//	UART1_Init();	//GPS
//	UART0_Init();	//SIM900D
//	UART2_Init(9600);	//RS232
//	//{COde demo sdcard
//	disk_initialize(0);
//		// This line of code will return Fil data in &file.Data is in Folder 117032014, filename 16110
//		if(ReadData(1,17032014,161100)) return 0;
//		while (f_gets(line, sizeof line, &file)){
//			DebugPrint(line);
//			DebugPrint("\r\n");
//		}
//		//remeber close file
//		Close();
//						//while(1){}
//		switch(WriteData(1,17022014,160000))	{
//		case 1:
//			break;
//		case 2:
//			break;
//		}
//		f_puts("$GSHT,2,xx,<NGUYEN VAN A,AN234343,00:01:29;105.34344,21.34343,21.34343,04:02:13,105.3464433,21.32343,80>,xxx#\r\n",&file);
//		//f_puts(" The gioi that rong lon\r\n",&file);
//		Close();
////}end sdcard demo
//	while (1) {
//		short_delay(1000);
//		Leds_invert();
//		Read232("READ01031214#");
//		Write232(Buff232.type, "abaabbbbcccccc");
//		//UART0_PrintString("AT\r\n");
//	}
//	return 0;
//}

