#include "common.h"
#include "GPIO/GPIO.h"
//local
unsigned int counter_send_gps;
//global
char data_gps[gpsLen];
unsigned int timer_gps;
unsigned int timer_send_gps;
volatile uint32_t msTicks;
struct MODEM    flag_modem;
struct GPRS     flag_gprs;
char rx_buffer1[RX_BUFFER_SIZE1];

//for key of car
void key_init(){
	GPIOSetDir(KEY_IN, INPUT);
	GPIOSetPull(KEY_IN, PULLUP);
}

void delay_ms (uint32_t dlyTicks) 
	{
  uint32_t curTicks;

  curTicks = msTicks ;
  while ((msTicks - curTicks) < dlyTicks)
  ;
}
	
