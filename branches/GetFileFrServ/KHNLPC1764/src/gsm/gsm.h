#ifndef __GSM_H
#define __GSM_H
#include <stdio.h>
#include "../common.h"
#include "../uart/uart.h"
#include "../GPIO/GPIO.h"
#include "../LED/led.h"
#include "../sdcard/khn_gsht.h"

#include <string.h>
//Define variable is used
#define RX_BUFFER_SIZE0 1000
extern int isGetFile;
extern char rx_buffer0[RX_BUFFER_SIZE0];
extern unsigned char rx_wr_index0,rx_rd_index0,rx_counter0;
extern unsigned char rx_wr_index1;
extern unsigned char rx_rd_index1,rx_counter1;


//Something strange. Maybe it is common

 #define no_send     0 //can merge to complete .if 1 is complete,0 is no_send
 #define complete    0





#define imeiLen 		20
extern char imei[imeiLen];

extern char time_gps[20];
extern char time_gps_send[20];

//thu_tu_tin_nhan;
extern char phoneDrive[phoneLen];

 

//Function
void gsm_on(void);
unsigned char init_gsm();
void start_up_gsm();
void del_buffer(unsigned char number);
void upperCase ();
void process_SMS(void);
void process_gsm_data(void) ;
unsigned char check_imei();
void ring_func();
#endif
