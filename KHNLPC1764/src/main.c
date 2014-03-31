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

#include "LPC17xx.h"
#include <cr_section_macros.h>
#include <NXP/crp.h>
//#include "uart.h"
//#include "timer.h"
//#include "rs232.h"
#include "sdcard/khn_gsht.h"
#include "sdcard/Fatfs/ff.h"
#include "sdcard/Fatfs/diskio.h"

#include <stdio.h>

#include "uart/uart.h"
#include "gpio/gpio.h"
#include "timer/timer.h"
#include "gsm/gsm.h"

#include "common.h"
#include "led/led.h"
#include "gps/gps.h"
#include "Tcp/tcp.h"

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
unsigned char resend;

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

///////////////////////////////////////////

struct SYSTEM_FLAG flag_system;

/*------------------------------------------------------------------------------
 delays number of tick Systicks (happens every 1 ms)
 *------------------------------------------------------------------------------*/
void SysTick_Handler(void) {
	uint8_t var;
	msTicks++; /* increment counter necessary in Delay() */
	var = GPIOGetValue(KEY_IN);      //Read the button value
	if (var == 0) {
		//GPIOSetValue(BUZZER, HIGH);
	} else {
		//GPIOSetValue(BUZZER, LOW);
	}
	if (interruptTimer_0 < 1000)
		interruptTimer_0++;
	else {
		if (timer_gps < counter_gps)
			timer_gps++;
		else {
			flag_system.request_data = 1;
			flag_system.gps_detect = 0; // moi lan request data thi coi nhu gps_detect=0
		}
		interruptTimer_0 = 0;
		if (timer_send_gps < counter_send_gps)
			timer_send_gps++; // cho nay cai dat thong so thoi gian de kiem tra gps
		else {
			flag_system.send_data = 1;     // send data o day
			flag_system.write_data = 1;
		}

	}
	if (timer_gprs < counter_timeout)
	        {
	            timer_gprs++;
	            flag_system.timeout_gprs=0;
	        }
	        else flag_system.timeout_gprs=1;

}
void UART2_IRQHandler(void) {
	Read232("READ01031214#");
}
void UART0_IRQHandler(void) {
	uint8_t IIRValue, LSRValue;
	uint8_t data;
	unsigned char trangthai_gui;
	IIRValue = LPC_UART0->IIR;
	IIRValue >>= 1; /* skip pending bit in IIR */
	IIRValue &= 0x07; /* check bit 1~3, interrupt identification */
	if (IIRValue == IIR_RLS) /* Receive Line Status */
	{
		LSRValue = LPC_UART0->LSR;
		if (LSRValue & LSR_RDR) /* Receive Data Ready */
		{
			data = LPC_UART0->RBR;
			//UART2_Sendchar(temp);
		}
	} else if (IIRValue == IIR_RDA) /* Receive Data Available */
	{
		//sprintf(tempValue,";%d",flag_gprs.send_gprs_ok);
		//UART2_PrintString(tempValue);

		data = LPC_UART0->RBR;
		UART2_Sendchar(data);
		switch (data) {
		case 0x0A: {
			trangthai_gui = 0;
			++flag_system.start_rx_gsm;
			if (flag_system.start_rx_gsm == 1) {
				del_buffer(gsm);
			} else if (flag_system.start_rx_gsm == 2) {
				if (rx_buffer0[0] == NULL)
					flag_system.start_rx_gsm = 1;
				else if (rx_buffer0[0] == '+' && rx_buffer0[1] == 'C'
						&& rx_buffer0[2] == 'M' && rx_buffer0[3] == 'G'
						&& rx_buffer0[4] == 'R') {
					flag_system.start_rx_gsm = 0;
					process_gsm_data();

				} else {
					if (rx_buffer0[0] == 'A' && rx_buffer0[1] == 'T') {
						flag_system.start_rx_gsm = 1; // tro lai ban dau bo wa 2 gia tri LF
						del_buffer(gsm);
					} else {
						flag_system.start_rx_gsm = 0;
						process_gsm_data();
					}
				}
			} else if (flag_system.start_rx_gsm == 3) {
				flag_system.start_rx_gsm = 0;
				process_gsm_data();
			}
			break;
		}     // end case 0x0a
		case 0x0D:
			break;     // enter thi bo qua
		default: {
			if (flag_system.start_rx_gsm != 0) {
				rx_buffer0[rx_wr_index0] = data;
				if (++rx_wr_index0 == RX_BUFFER_SIZE0)
					rx_wr_index0 = 0;
				if (++rx_counter0 == RX_BUFFER_SIZE0) {
					rx_counter0 = 0;
					rx_buffer_overflow0 = 1;
					flag_system.start_rx_gsm = 0;
					del_buffer(gsm);  // tran bufffer thi ko xu ly
				};
				if (rx_buffer0[0] == '>') {
					trangthai_gui = 1;
					flag_system.start_rx_gsm = 0;
					del_buffer(gsm);
				}
			} else if (flag_system.start_rx_server != 0) {
				data_server[pointer_server] = data;
				if (++pointer_server >= data_serverLen) {
					flag_system.start_rx_server = 0;
				}
			}
			break;
		} // end for default
		}

		//UART2_Sendchar(temp);
	}

}

void UART1_IRQHandler(void) {
	uint8_t IIRValue, LSRValue;
	uint8_t data;
	IIRValue = LPC_UART1->IIR;
	IIRValue >>= 1; /* skip pending bit in IIR */
	IIRValue &= 0x07; /* check bit 1~3, interrupt identification */
	if (IIRValue == IIR_RLS) /* Receive Line Status */
	{
		LSRValue = LPC_UART1->LSR;
		if (LSRValue & LSR_RDR) /* Receive Data Ready */
		{
			data = LPC_UART1->RBR;
			//UART2_Sendchar(temp);
		}
	} else if (IIRValue == IIR_RDA) /* Receive Data Available */
	{
		data = LPC_UART1->RBR;
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
	TIMER0_interrupt();
	couter0++;
	if (couter0 > 6) {
		couter0 = 0;
		toggle = ~toggle;
		//GPIOSetValue(BUZZER, toggle);
		//LPC_TIM0->IR = 1;			/* clear interrupt flag */
	}

//    toggle = ~toggle;
//    GPIOSetValue(BUZZER, toggle);
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

void upload_info() {
	unsigned char i, j;
	if (apn[0] == NULL || apn[0] == 0xff) {
		sprintf(apn, "v-internet");
		sprintf(userName, "");
		sprintf(passApn, "");
	}
	if (ipServer[0] == NULL || ipServer[0] == 0xff) {
		sprintf(ipServer, "112.213.84.16");
		sprintf(tcpPort, "11511");
	}
	if (timeCheck[0] == NULL || timeCheck[0] == 0xff)
		counter_send_gps = 20;		// gui len server

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
	KHN_SDCARD_INIT();
	LED_Init();
	BUZZER_Init();
	key_init();
	gsm_on();
}

int main(void) {
	uint8_t location;
	uint8_t buffer[200];

	uint8_t vitri, resend_gprs, counter_init = 0, i;
	init_program();
	//delay_ms(500);
	//Init_SomeThing();
	//LED_Config();
	delay_ms(500);
	upload_info();
	delay_ms(500);
	start_up_gsm();

	flash_led();

	for (i = 0; i < 5; i++) {
		flash_led();
		delay_ms(1000);
	}

//disk_initialize(0);
	while (1) {
		if (flag_modem.modem == not_connect) {
			start_up_gsm();
			delay_ms(1000);
			init_gsm();
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

//			if(flag_modem.error == 1)flag_modem.modem = not_connect;
//			else flag_modem.modem = connected;
//
//			 if (flag_modem.cmgr ==1)
//        {
//          flag_modem.cmgr=0;
//
//					if(strlen(userCall)>10)	process_command();
//					delay_ms(1000);
//					send_sms_func(sms_reply);
//					delay_ms(1000);
//         UART0_PrintString("AT+CMGDA=\"DEL ALL\"\r");
//					flag_system.send_data =0;
//          delay_ms(1000);
//
//
//        }
//        else if (flag_system.send_data_flag == smsMode )
//				 {
//									flag_system.send_data_flag =0;
//										flag_system.send_data =0;
//
//                           send_sms_func(sms_reply);
//
//                           delay_ms(1000);
//                            UART0_PrintString("AT+CMGDA=\"DEL ALL\"\r");
//                            delay_ms(3000);
//          }
//			  else if (flag_modem.ring)
//        {
//           ring_func();
//         }
//
//							else if(flag_system.read_sms)
//
//							{
//
//									UART0_PrintString("AT+CMGR=1\r");
//									flag_system.read_sms=0;
//									delay_ms(1000);
//							}
//				else
			if (flag_system.send_data || flag_system.change_status) {

				if (flag_gprs.deactive_pdp) {
					sprintf(buffer, "%c", ctrl_z);
					//print_data(buffer);
					delay_ms(1000);
					UART0_PrintString("AT+CIPSHUT\r");
					delay_ms(1000);
					if (++counter_init >= 20)    // test
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
								{
					for (vitri = 0; vitri < strlen(imei); vitri++) // kiem tra thanh phan ky tu trong imei
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
							flag_gprs.connect_ok = 1;   // ket noi ok

							memset(data_gps, gpsLen, NULL); // xoa NULL het GPS data

							sprintf(data_gps,
									"$$KHN$%s,56,%s,%s,N,%s,E,%s,%d,%d,%d,%d,%s,%s,%d,%d,%s$\r\n",
									version, imei, latitude, longitude,
									speed_gps, oil_value,
									flag_system.status_door,
									flag_system.status_key,
									flag_system.card_status, so_vin, phoneDrive,
									flag_system.cold_hot, flag_system.sleep,
									time_gps_send);    // trang thai IN3
							switch (WriteData(1, 17022014, 160000)) {
							case 1:
								break;
							case 2:
								break;
							}
							f_puts(data_gps, &file);
							UART2_PrintString("Write sdcard");
							//f_puts(" The gioi that rong lon\r\n",&file);
							Close();

							if (send_data_gprs(data_gps) != ok_status) // send data
							{
								//UART2_PrintString("loi xay ra\r\n");
								flag_gprs.deactive_pdp = 1; // neu send bi loi thi reset lai gprs
							}
							//UART2_PrintString("Khong loi khi gui \r\n");
							delay_ms(1000);
							UART0_PrintString("AT+CIPCLOSE\r");
							resend_gprs = 0;
						} else {
							flag_gprs.connect_ok = 0;   // ket noi bi loi
							flag_gprs.deactive_pdp = 1;
						}
					}   // end cua vitri=0

				}

				flag_system.send_data = 0;
				flag_system.change_status = 0;

			}

		}

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

