#ifndef __BSP_LED_H__
#define __BSP_LED_H__
#include "imx6ul.h"

#define led_red         0      // 	GPIO1_IO04
#define led_green         1      //  GPIO4_IO20
#define led_blue         2      //  GPIO4_IO19	


void led_init(void);
void led_red_on(void);
void led_red_off(void);
void led_green_on(void);
void led_green_off(void);
void led_blue_on(void);
void led_blue_off(void);
void led_red_switch(int led, int status);

#endif // __BSP_LED_H__