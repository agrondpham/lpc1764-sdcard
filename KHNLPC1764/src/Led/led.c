#include "led.h"

void LED_Init(void) 
{	
	GPIOSetDir(LED_STATUS, OUTPUT); 
}
void BUZZER_Init(void) 
{	
	GPIOSetDir(BUZZER, OUTPUT); 
}
void buzzer_on(void){
	GPIOSetValue(BUZZER, HIGH);
}
void buzzer_off(void){
	GPIOSetValue(BUZZER, LOW); 
}
void flash_led(){
		GPIOSetValue(LED_STATUS, HIGH);
		delay_ms(1000);
		GPIOSetValue(LED_STATUS, LOW);
}