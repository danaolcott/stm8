/*
GPIO Controller File
*/

#ifndef __GPIO_H
#define __GPIO_H


void GPIO_init(void);
void LED_On(void);
void LED_Off(void);
void LED_Toggle(void);

void EXTI1_ISR(void);			//GPIO ISR

#endif

