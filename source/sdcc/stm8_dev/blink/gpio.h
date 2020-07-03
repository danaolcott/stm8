/*
3/5/20
GPIO Controller File
*/


#ifndef __GPIO_H
#define __GPIO_H


void GPIO_init(void);

void GPIO_led_red_on(void);
void GPIO_led_green_on(void);
void GPIO_led_red_off(void);
void GPIO_led_green_off(void);
void GPIO_led_red_toggle(void);
void GPIO_led_green_toggle(void);


#endif

