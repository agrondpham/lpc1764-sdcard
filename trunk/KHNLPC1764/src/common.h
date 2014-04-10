#ifndef __COMMON_H
#define __COMMON_H
#include "LPC17xx.h"
#include <stdio.h>
/*	Flash sectors to be used for data storage */
#define	DATA_START_SECTOR	0x00000000	/* Start Sector 17 */
#define	DATA_END_SECTOR		0x00000FFF	/* End Sector 17 */

#define _DEBUG 1

struct MODEM {
	unsigned long modem :1;    // connect hoac disconnect
	unsigned long sim :1;
	unsigned long cmti :1;
	unsigned long cmgr :1;
	unsigned long cmgs :1;
	unsigned long ok :1;
	unsigned long error :1;
	unsigned long cclk :1;
	unsigned long ring :1;
	unsigned long send_sms :1;
	unsigned long flag_modem :1;
	unsigned long cusd :1;
	unsigned long busy_call :1;
	unsigned long no_answer :1;
	unsigned long no_dial_tone :1;
	unsigned long no_carrier :1;
	unsigned long read_sms :1;
	unsigned long run_gprs :1;
	unsigned long op_selec :1;
	unsigned long upinfo :1;
	unsigned long flash_coppy :1;

};
// cac bien dung de check trang thai cac flag he thong (hien tai cau hinh 32 flag)
extern struct MODEM flag_modem;

struct SYSTEM_FLAG {
	unsigned long start_rx_gps :1;
	unsigned long start_rx_gsm :2;
	unsigned long send_data :1;
	unsigned long write_data :1;
	unsigned long request_data :1;
	unsigned long timeout_gprs :1;
	unsigned long timeout_gps :1;
	unsigned long timeout_sms :1;
	unsigned long startTimeGps :1;
	unsigned long gps_detect :1;
	unsigned long buzz_accept :1;
	unsigned long send_data_flag :1;
	unsigned long led_check :1;
	unsigned long imei_check :1;
	unsigned long check_out_sms :1;
	unsigned long phoneNumber_N32_check :1;
	unsigned long status_door :2;
	unsigned long status_key :2;
	unsigned long in_out :1;
	unsigned long start_pc :1;
	unsigned long config_mode :1;
	unsigned long config_process :1;
	unsigned long reset_device :2;
	unsigned long card_status :1;
	unsigned long mode :1;
	unsigned long start_rx_server :1;
	unsigned long server_data_command :1;
	unsigned long sleep :1;
	unsigned long print :1;
	unsigned long change_status :1;
	unsigned long card_status_data :1;
	unsigned long cold_hot :1;
	unsigned long read_sms :1;

};
extern struct SYSTEM_FLAG flag_system;

//FOR GPRS
struct GPRS {
	unsigned int status_gprs :4;     // trang thai ket noi voi gprs
	unsigned int close_ok :1;
	unsigned int shut_ok :1;
	unsigned int deactive_pdp :1;
	unsigned int connect_ok :1;
	unsigned int send_gprs_ok :1;
	unsigned int tcp_error :1;
};
extern struct GPRS flag_gprs;

struct FlashInfo {
	unsigned char company[50];    // connect hoac disconnect
	unsigned char address[150];    // connect hoac disconnect
	unsigned char vin_No[50];
	unsigned char id_device[50];
	unsigned char ownerName[50];    // connect hoac disconnect
	unsigned char phone[20];    // connect hoac disconnect
	unsigned char license[20];
	unsigned char license_iss_date[15];
	unsigned char license_exp_date[15];
};
typedef struct FlashInfo flash_data_type;
extern flash_data_type flash_data;

struct time_gsm {
	char year[3];
	char month[3];
	char day[3];
	char hour[3];
	char min[3];
	char sec[3];
};

extern struct time_gsm *timeSet;

#define OFF 0
#define ON  1
#define counter_timeout 15000
#define ESC		0x1B
#define ctrl_z		0x1A
#define counter_timeout_sms 5000
#define gpsLen      			200
#define RX_BUFFER_SIZE1 	200
#define counter_gps 			10
#define not_connect 		0	
#define connected   		1
#define ok_status   		2
#define connect_error   3
#define send_error  		4
///GPS
#define gps_time_len    7
#define gps_date_len    7

#define speedLen 5
#define latLen  12
#define longLen 13

#define so_gplxLen  20
#define ngaycapLen  20
#define handenLen   20
#define ten_laixeLen         20

#define so_vinLen      20
#define bien_soxeLen    15
#define ten_laixeLen         20
#define tencongtyLen        50
#define diachiLen           25


extern char gps_time_string[gps_time_len];
extern char latitude[latLen];   //    ="1049.3361"
extern char longitude[longLen];   // ="10641.8130"
extern char speed_gps[speedLen];
extern char gps_date_string[gps_date_len];
//TCP
extern uint32_t timer_gprs;
//GPIO
//#define  KEY_IN			0,19
#define  KEY_IN					0,19
#define  DOOR_IN				0,18
extern char rx_buffer1[RX_BUFFER_SIZE1];
extern unsigned int interruptTimer_0;
extern volatile uint32_t msTicks;
extern unsigned int timer_gps;
extern unsigned int timer_send_gps;
extern unsigned int timer_read_sms;
extern unsigned int timer_check_sms;

extern unsigned int counter_send_gps;
extern char data_gps[gpsLen];

//GSM
#define smsLen      160
#define smsMode    			1
#define apnLen    			15
#define userNameLen    	5
#define passWordLen    	5
#define ipServerLen    	16
#define tcpPortLen     	6
#define phoneLen    13
extern char phone_1[phoneLen];
extern char sms_reply[smsLen];
extern char apn[apnLen];
extern char userName[userNameLen];
extern char passApn[passWordLen];
extern char ipServer[ipServerLen];
extern char tcpPort[tcpPortLen];
extern char userCall[phoneLen];
extern char userCall_new[phoneLen];
extern char data_sms[smsLen];

extern char ten_laixe[ten_laixeLen] ;
extern char so_gplx[so_gplxLen]  ;
extern char ngaycap_gplx[ngaycapLen] ;
extern char handen_gplx[handenLen] ;
extern char tencongty[tencongtyLen];
extern char diachi[diachiLen];
extern char so_vin[so_vinLen] ;
extern char bien_soxe[bien_soxeLen];

#define buffer_file     300
extern char buffer[buffer_file];

extern void process_command();
extern void EraseSectors();

//Function
void key_init();
void delay_ms(uint32_t dlyTicks);
//void get_data_from_flash(char data[256]);
void send_sms_func(char *smsString);
//extern void clearFlash();
#endif
