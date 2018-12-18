/*
GPIO Controller File
*/

#ifndef __GPIO_H
#define __GPIO_H


void GPIO_init(void);
void LED_On(void);
void LED_Off(void);
void LED_Toggle(void);

uint8_t Button_Read(void);


#endif

