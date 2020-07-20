/*
7/18/20
GPIO Controller File
*/


#ifndef __GPIO_H
#define __GPIO_H

void gpio_init(void);
void gpio_clearUserButtonFlag(void);
void gpio_clearCenterButtonFlag(void);
uint8_t gpio_getUserButtonFlag(void);
uint8_t gpio_getCenterButtonFlag(void);


void exti1_isr(void);
void exti5_isr(void);



#endif

