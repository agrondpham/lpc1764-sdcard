#include "led.h"
/*
void LED_Init(void) {
	GPIOSetDir(LED_STATUS, OUTPUT);
}
void BUZZER_Init(void) {
	GPIOSetDir(BUZZER, OUTPUT);
}
void buzzer_on(void) {
	GPIOSetValue(BUZZER, HIGH);
}
void buzzer_off(void) {
	GPIOSetValue(BUZZER, LOW);
}

//for key of car
void key_init() {
	GPIOSetDir(KEY_IN, INPUT);
	GPIOSetPull(KEY_IN, PULLUP);
}
/// door
void door_init() {
	GPIOSetDir(DOOR_IN, INPUT);
	GPIOSetPull(DOOR_IN, PULLUP);
}
/// col_hot
void col_init() {
	GPIOSetDir(COL_HOT, INPUT);
	GPIOSetPull(COL_HOT, PULLUP);
}
*/
void GPIO_init()
{
	GPIOSetDir(LED_STATUS, OUTPUT);
	GPIOSetDir(BUZZER, OUTPUT);
	GPIOSetDir(GSM_POWER, OUTPUT);
	GPIOSetDir(GSM_RES, OUTPUT);
	GPIOSetDir(RES_GPS, OUTPUT);

	GPIOSetDir(KEY_IN, INPUT);
	GPIOSetPull(KEY_IN, PULLUP);
	GPIOSetDir(DOOR_IN, INPUT);
	GPIOSetPull(DOOR_IN, PULLUP);
	GPIOSetDir(COL_HOT, INPUT);
	GPIOSetPull(COL_HOT, PULLUP);


}
void flash_led() {
	GPIOSetValue(LED_STATUS, HIGH);
	delay_ms(1000);
	GPIOSetValue(LED_STATUS, LOW);
}


