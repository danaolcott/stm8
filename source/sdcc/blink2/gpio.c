/*
GPIO Controller File


*/

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#include "register.h"
#include "gpio.h"

//////////////////////////////////////////
//GPIO_init
void GPIO_init(void)
{
    //configure PB5 as output
    PB_DDR = BIT_5;
    PB_CR1 = BIT_5;
    PB_CR2 = BIT_5;

    //init low
    PB_ODR = 0x00;
}


////////////////////////////////////////
//LED_On - PB5 is Low
void LED_On(void)
{
    PB_ODR &=~ BIT_5;
}

/////////////////////////////////////
//LED_Off - PB5 is High
void LED_Off(void)
{
    PB_ODR |= BIT_5;
}

////////////////////////////////////////
//LED_Toggle - Toggle PB5
void LED_Toggle(void)
{
    PB_ODR ^= BIT_5;
}


////////////////////////////////////////
//User button connected to PB4
//Wired with 10k pullup, return 0 if pressed
//and 1 if not pressed
//
uint8_t Button_Read(void)
{
    uint8_t value = (PB_IDR & BIT_4) >> 4;
    return value;
}




