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

#include "RTC/rtc.h"

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
#define counter_check_sim900 10000

extern int co_gps;
uint8_t rx_gps_index;
volatile uint32_t send_data_server;
volatile uint32_t toggle = 0;
volatile uint32_t couter0 = 0;
uint8_t data[100];
uint16_t rx_gsm_index;
unsigned char resend;

//char phone_1[phoneLen];

char viettel[] = "VNM and VIETTEL";
char VN_mobile[] = "VN MOBIFONE";
char VN_vina[] = "VN VINAPHONE";
char vina[] = "VINAPHONE";
char mobile[] = "MOBIFONE";

unsigned int interruptTimer_0;
unsigned int led_counter;

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
unsigned int timer_check_sim900 = 0;
char so_vin[so_vinLen] = "";
char time_gps_card[8];
char day_gps_card[8];

char year_str[3];
char month_str[3];
char day_str[3];
char hour_str[3];
char min_str[3];
char sec_str[3];

char ten_laixe[ten_laixeLen];
char so_gplx[so_gplxLen];
char ngaycap_gplx[ngaycapLen];
char handen_gplx[handenLen];
char buffer[buffer_file];
char tencongty[tencongtyLen];
char diachi[diachiLen];
char bien_soxe[bien_soxeLen];

extern volatile uint32_t alarm_on;
RTCTime local_time, alarm_time, current_time;

///////////////////////////////////////////

struct SYSTEM_FLAG flag_system;
//struct time_gsm *timeSet;

/*------------------------------------------------------------------------------
 delays number of tick Systicks (happens every 1 ms)
 *------------------------------------------------------------------------------*/
void SysTick_Handler(void) {
	msTicks++;
	if (interruptTimer_0 < 1000)
		interruptTimer_0++;
	else {
		interruptTimer_0 = 0;
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

	if (flag_modem.modem == not_connect) {

		if (led_counter < 500)
			led_counter++;
		else {
			led_counter = 0;
			toggle = ~toggle;
			GPIOSetValue(LED_STATUS, toggle);

		}
	} else {
		if (led_counter < 2000)
			led_counter++;
		else {
			led_counter = 0;
			toggle = ~toggle;
			GPIOSetValue(LED_STATUS, toggle);

		}
	}

	if (timer_check_sim900 < counter_check_sim900)    //
	{
		timer_check_sim900++;
	}
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
		flag_system.status_key = 1;
	} else {
		//GPIOSetValue(BUZZER, LOW);
		flag_system.status_key = 2;
	}
	if (var_door == 0)      //KEY_IN
			{
		//GPIOSetValue(BUZZER, HIGH);
		flag_system.status_door = 1;
	} else {
		//GPIOSetValue(BUZZER, LOW);
		flag_system.status_door = 2;
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
	//unsigned char phone_2[256];
	//memcpy(phone_2, (void*) 0x00018000, 256);
//	delay_ms(100);
//	for (x = 0; x < phoneLen; x++) {
//		if (phone_2[x] == NULL || phone_2[x] == 0xFF)
//			break;
//		if (phone_2[x] > 0x39 || phone_2[x] < 0x30)
//			break;
//		phone_1[x] = phone_2[x];
//	}
//	phone_1[x] = NULL;
//	if (phone_1[0] == NULL || phone_1[0] == 0xF)
//		sprintf(phone_1, "000");
////// start phoneDrive
//	for (j = 0; j < phoneLen; j++) {
//		if (phone_1[j] == NULL || phone_1[j] == 0xFF)
//			break;
//		phoneDrive[j] = phone_1[j];
//	}
//	phoneDrive[j] = NULL;
//	///end
	READ_APN_IP_SPEED();
	delay_ms(1000);
	sprintf(apn, "%s", flash_data_APN_IP.apn_save);
	sprintf(userName, "%s", flash_data_APN_IP.userName_save);
	sprintf(passApn, "%s", flash_data_APN_IP.passWord_save);
	sprintf(ipServer, "%s", flash_data_APN_IP.ipServer_save);
	sprintf(tcpPort, "%s", flash_data_APN_IP.tcpPort_save);
	sprintf(passWord, "%s", flash_data_APN_IP.pass_save);
	speed_max = atoi(flash_data_APN_IP.speed_save);
	if (apn[0] == NULL || apn[0] == 0xff) {
//		sprintf(apn, "v-internet");
//		sprintf(userName, "");
//		sprintf(passApn, "");
		sprintf(apn, "m3-world");
		sprintf(userName, "mms");
		sprintf(passApn, "mms");
	}
	if (ipServer[0] == NULL || ipServer[0] == 0xff) {
		//sprintf(ipServer, "112.213.84.16");
		//sprintf(ipServer,"112.213.84.16");
		sprintf(ipServer, "112.213.94.122");

		sprintf(tcpPort, "11511");
	}
	if (timeCheck[0] == NULL || timeCheck[0] == 0xff)
		counter_send_gps = 5;		// gui len server
	flag_system.buzz_accept = 1;
}

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
	gps_init();

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
	unsigned char b, c, d;
	int a;
	char day, month, year, hour, min, sec; //, week_day;

	char hour_tam[3];

	char min_tam[3];
	char sec_tam[3];
	char year_tam[3];
	char month_tam[3];
	char day_tam[3];
	char month_tam_1[3];
	init_program();

	RTCInit();

//	alarm_time.RTC_Sec = 0;
//	alarm_time.RTC_Min = 0;
//	alarm_time.RTC_Hour = 0;
//	alarm_time.RTC_Mday = 1;
//	alarm_time.RTC_Wday = 0;
//	alarm_time.RTC_Yday = 1; /* alarm date 01/01/2007 */
//	alarm_time.RTC_Mon = 1;
//	alarm_time.RTC_Year = 2007;
//	RTCSetAlarm(alarm_time); /* set alarm time */

	NVIC_EnableIRQ(RTC_IRQn);

//delay_ms(500);
//Init_SomeThing();
//LED_Config();
	GPIOSetValue(RES_GPS, HIGH);
	GPIOSetValue(GSM_RES, LOW);

	delay_ms(500);
	GPIOSetValue(RES_GPS, LOW);
	GPIOSetValue(GSM_RES, HIGH);
	upload_info();
//delay_ms(500);
//start_up_gsm();

//flash_led();
	RTCStart();

	for (i = 0; i < 5; i++) {
		flash_led();
		//flash_gps();
		delay_ms(1000);
	}
	GPIOSetValue(BUZZER, HIGH);
	delay_ms(50);
	GPIOSetValue(BUZZER, LOW);
	flag_modem.flash_coppy = 1;
	flag_gprs.bit_data_flash_apn = 0;
	flag_gprs.bit_data_infor = 0;
	//// test
//	if (flag_modem.modem == not_connect) {
//		start_up_gsm();
//		///if(flag_modem.flash_coppy == 1)
//		//{
//		//flag_modem.flash_coppy = 0;
//		//CopyRAMToFlash(phone_1);
//		//}
//		delay_ms(1000);
//		init_gsm();
//		//UART3_Sendchar('A');
//		//UART3_PrintString("ket thuc main vao while");
//		if (flag_gprs.status_gprs != ok_status) {
//
//			flag_system.led_check = 1;
//			flag_gprs.status_gprs = init_tcp(apn, userName, passApn,
//					flag_gprs.status_gprs);
//			resend++;
//			if (resend >= 2 && flag_gprs.status_gprs != ok_status) {
//				flag_gprs.deactive_pdp = 1;
//				resend = 0;
//			} else if (flag_gprs.status_gprs == ok_status)
//				counter_init = 0;
//			flag_system.led_check = 0;
//		}
//	}
	/////////end

//disk_initialize(0);

	while (1) {

//		current_time = RTCGetTime();
//		sprintf(buffer,"%d/%d/%d",current_time.RTC_Hour,current_time.RTC_Min,current_time.RTC_Sec);
//		UART2_PrintString(buffer);
		if (timer_check_sim900 >= counter_check_sim900)   // 10s kiem tra 1 lan
		{
			current_time = RTCGetTime();
			//			sprintf(buffer, "%d/%d/%d", current_time.RTC_Hour,
			//					current_time.RTC_Min, current_time.RTC_Sec);
			sprintf(time_gps, "%02d/%02d/%d,%02d:%02d:%02d",
					current_time.RTC_Mday, current_time.RTC_Mon,
					current_time.RTC_Year % 100, current_time.RTC_Hour,
					current_time.RTC_Min, current_time.RTC_Sec);
			//UART2_PrintString(time_gps);
			start_up_gsm();
			timer_check_sim900 = 0;
			//UART0_PrintString("AT+CMGR=1\r");
			flag_system.read_sms = 1;

		}
		if (flag_modem.modem == not_connect) {
			start_up_gsm();
			///if(flag_modem.flash_coppy == 1)
			//{
			//flag_modem.flash_coppy = 0;
			//CopyRAMToFlash(phone_1);
			//}
			delay_ms(1000);
			init_gsm();
			delay_ms(1000);
			///APN
			if (flag_modem.op_selec) {
				flag_modem.op_selec = 0;

				if (strcmp(Selection, viettel) == 0) {
					sprintf(apn, "v-internet");
					sprintf(userName, "");
					sprintf(passApn, "");
				} else if (strcmp(Selection, mobile) == 0
						|| strcmp(Selection, VN_mobile) == 0) {
					sprintf(apn, "m-wap");
					sprintf(userName, "mms");
					sprintf(passApn, "mms");
				} else if (strcmp(Selection, vina) == 0
						|| strcmp(Selection, VN_vina) == 0) {
					sprintf(apn, "m3-world");
					sprintf(userName, "mms");
					sprintf(passApn, "mms");
				} else {
					sprintf(apn, "m-wap");
					sprintf(userName, "mms");
					sprintf(passApn, "mms");
				}
			}

			///END

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
				delay_ms(1000);			//}
				if (flag_modem.cmgr == 1) {
					flag_modem.cmgr = 0;
					//clearFlash();
					//EraseSectors();
					if (strlen(userCall_new) > 10)
						process_command();
					delay_ms(1000);
					UART0_PrintString("AT+CMGDA=\"DEL ALL\"\r");
					delay_ms(1000);
					send_sms_func(sms_reply);


					//delay_ms(1000);
					//UART0_PrintString("AT+CMGDA=\"DEL ALL\"\r");
					flag_system.send_data = 0;
					delay_ms(1000);
					//writeFlash(phone_1);
					if (strcmp(data_flash, ",,,,,,,,,\n\r") != 0) {
						if (flag_gprs.bit_data_infor == 1) {
							write_basic_infor(data_flash);
							flag_gprs.bit_data_infor = 0;
						}

					}
					if (strcmp(data_flash_APN, ",,,,,,,,,\n\r") != 0) {

						if (flash_data_APN_IP.userName_save == "") {
							strcpy(flash_data_APN_IP.userName_save, '0');//flash_data_APN_IP.userName_save  = "";
						}
						if (flash_data_APN_IP.passWord_save == "") {
							strcpy(flash_data_APN_IP.passWord_save, '0');
						}
						if (flag_gprs.bit_data_flash_apn == 1) {
							WRITE_APN_IP_SPEED(data_flash_APN);
							flag_gprs.bit_data_flash_apn = 0;
						}
					}

				}

				//CopyRAMToFlash(phone_1);
				delay_ms(500);
				//UART0_PrintString("AT\n\r");
				//delay_ms(1000);
				//flag_modem.modem = not_connect;
				//flag_modem.flash_coppy = 1;

			}
//			else if (flag_system.send_data_flag == smsMode) {
//				flag_system.send_data_flag = 0;
//				flag_system.send_data = 0;
//				send_sms_func(sms_reply);
//				delay_ms(1000);
//				UART0_PrintString("AT+CMGDA=\"DEL ALL\"\r");
//				delay_ms(3000);
//			}
			else if (flag_modem.ring) {
				ring_func();
//			for (i = 0; i < phoneLen; i++) {
//				if (phone_1[i] == NULL || phone_1[i] == 0xFF)
//					break;
//				phoneDrive[i] = phone_1[i];
//			}
//			phoneDrive[i] = NULL;

			}
			if (flag_system.buzz_accept){
			if (atoi(speed_gps) * 1.852 >= speed_max && flag_system.gps_detect) // vuot toc do  va co tin hieu GPS
					{
				if (atoi(speed_gps) < 80)GPIOSetValue(BUZZER, LOW);

				else GPIOSetValue(BUZZER,HIGH );

			}
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

							if (connectServer(ipServer, tcpPort) == ok_status) {
								//if (connectServer(flash_data_APN_IP.ipServer_save,flash_data_APN_IP.tcpPort_save) == ok_status) {
								flag_gprs.connect_ok = 1;// ket noi ok

								memset(data_gps, gpsLen, NULL);// xoa NULL het GPS data

								if(read_basic_infor()==1 ||READ_APN_IP_SPEED()==1) {

								} else {
									UART2_PrintString("Co loi xay ra khi in\r\n");
									UART2_PrintString("Ma loi :  bsi-01\r\n");
								}
								if (strcmp(latitude, "_") == 0) {
									strcpy(latitude,"");
								}
								if (strcmp(longitude, "_") == 0) {
									strcpy(longitude,"");
								}
								sprintf(data_gps,
										"$$KHN$%s,%s,%s,%s,N,%s,E,%s,%d,%d,%d,%d,%s,%s,%d,%d,%s$\r\n",
										version,flash_data.id_device, imei, latitude, longitude,
										speed_gps, oil_value,
										flag_system.status_key,
										flag_system.status_door,
										flag_system.card_status, flash_data.vin_No, flash_data.phone,
										flag_system.cold_hot,flag_system.sleep,time_gps); //,gps_time_string);//gps_date_string,
								//gps_time_string);//time_gps_send);flag_system.sleep
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
								flag_gprs.connect_ok = 0; // ket noi bi loi
								flag_gprs.deactive_pdp = 1;
							}
						}   // end cua vitri=0

					}

					flag_system.send_data = 0;
					flag_system.change_status = 0;

				}

			}

				/////// timer/////////////
		if (flag_system.write_data == 1) {

			if (flag_system.gps_detect) {
				if (strlen(gps_time_string) != 6
						|| strlen(gps_date_string) != 6) {
					UART2_PrintString("TIMER_ERROR\r");
					delay_ms(1000);
					goto exit_write_data;
				}
				khn_time_type datetime;

				datetime = convert_data_time(gps_date_string, gps_time_string,
						7);
				local_time.RTC_Sec = datetime.sec;
				local_time.RTC_Min = datetime.min;
				local_time.RTC_Hour = datetime.hour;
				local_time.RTC_Mday = datetime.day;
				//local_time.RTC_Wday = 03;
				//local_time.RTC_Yday = 12; /* current date 07/12/2006 */
				local_time.RTC_Mon = datetime.month;
				local_time.RTC_Year = datetime.year;
				RTCSetTime(local_time); /* Set local time */
				//sprintf(time_gps_card, "%02d%s%s", a, min_tam, sec_tam);

				memset(data_gps, gpsLen, NULL );
				//sprintf(data_gps, "AT+CCLK=\"%s+00\"\r", time_gps);
				//UART0_PrintString(data_gps);
				//UART2_PrintString(time_gps);
				delay_ms(500);
				// UART2_PrintString(time_gps);

			} else {
				//UART0_PrintString("at+cclk?\r");
				delay_ms(1000);
			}
//		                flag_system.status_door =! door;
//		                flag_system.status_key =! key;

			for (vitri = 0; vitri < gpsLen; vitri++)
				data_gps[vitri] = NULL;
			//$3.13,56,862118024728161,1052.0431,N,10641.1769,E,81.00,0,0,0,0,_,000,0,0,050414,080307
//		sprintf(data_gps, "<%s;%s;%s;%s;%d;%d>\r\n", time_gps, latitude,
//				longitude, speed_gps, flag_system.status_key,
//				flag_system.status_door);

//		sprintf(data_gps, "$3.13,56,%s,%s\r\n", imei,time_gps, latitude,
//				longitude, speed_gps, flag_system.status_key,
//				flag_system.status_door);

			if (flag_system.cold_hot == "") {
				flag_system.cold_hot = "_";
			}
			if (flag_system.sleep == "") {
				flag_system.sleep = "_";
			}
			if (strcmp(latitude, "") == 0) {
				strcpy(latitude, "_");
			}
			if (strcmp(longitude, "") == 0) {
				strcpy(longitude, "_");
			}
			current_time = RTCGetTime();
			sprintf(day_gps_card, "%02d%02d%02d", current_time.RTC_Mday,
					current_time.RTC_Mon, current_time.RTC_Year % 100);
			sprintf(time_gps_card, "%02d%02d%02d", current_time.RTC_Hour,
					current_time.RTC_Min, current_time.RTC_Sec);
			sprintf(data_gps,
					"$%s,56,%s,%s,N,%s,E,%s,%d,%d,%d,%d,%s,%s,%d,%d,%s,%s\r\n",
					version, imei, latitude, longitude, speed_gps, oil_value,
					flag_system.status_key, flag_system.status_door,
					flag_system.card_status, flash_data.vin_No,
					flash_data.phone, flag_system.cold_hot, flag_system.sleep,
					day_gps_card, time_gps_card);			//flash_data.phone
//
//#if _DEBUG==1
//			UART2_PrintString(data_gps);
//#endif

			///start_sdcar
			switch (WriteData(1, atoi(day_gps_card),
					current_time.RTC_Hour * 10000)) {
			case 1:
				break;
			case 2:
				break;
			}
			if (f_puts(data_gps, &file) > -1) {
				Close();
				flag_system.card_status = 1;
#if _DEBUG==1
				UART2_PrintString("write sdcard\r\n");
#endif
			} else {
				flag_system.card_status = 0;
#if _DEBUG==1
				UART2_PrintString("CAN NOT Write sdcard\r\n");

#endif
			}  //END

			exit_write_data: flag_system.write_data = 0;
			timer_send_gps = 0; // xoa thoi gian gui len data va cho ghi vao the nho

			delay_ms(100);
		}
		//////////end////////

	}
}

