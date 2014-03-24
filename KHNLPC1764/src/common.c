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

/*------------------------------------------------------------------------------
  delays number of tick Systicks (happens every 1 ms)
 *------------------------------------------------------------------------------*/
void SysTick_Handler(void) {
uint8_t var; 
	msTicks++;                        /* increment counter necessary in Delay() */
		var = GPIOGetValue(KEY_IN);      //Read the button value
 
  if (var == 0)                   
  {
     //GPIOSetValue(BUZZER, HIGH);  
  }
  else                            
  {
     //GPIOSetValue(BUZZER, LOW);
  }	
	 if (interruptTimer_0 < 1000) interruptTimer_0 ++;
	else
	{
		 if (timer_gps < counter_gps) timer_gps++; 
		 else  
     { 
       flag_system.request_data = 1; 
       flag_system.gps_detect=0;    // moi lan request data thi coi nhu gps_detect=0
      }
		 interruptTimer_0=0;
			if (timer_send_gps < counter_send_gps)     timer_send_gps++; // cho nay cai dat thong so thoi gian de kiem tra gps
      else   
      {
         flag_system.send_data=1;     // send data o day
         flag_system.write_data=1;
      }
		
	}
	

}
void delay_ms (uint32_t dlyTicks) 
	{
  uint32_t curTicks;

  curTicks = msTicks ;
  while ((msTicks - curTicks) < dlyTicks)
  ;
}
	
