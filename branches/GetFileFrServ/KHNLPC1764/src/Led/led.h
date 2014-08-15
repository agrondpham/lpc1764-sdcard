#include "../common.h"
#include "../gpio/gpio.h"

#define  LED_STATUS 	1,28
#define  BUZZER			3,25


void GPIO_init(void);
void LED_Init(void);
void flash_led();

void BUZZER_Init(void);
void buzzer_on(void);
void buzzer_off(void);


