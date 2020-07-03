/*
3/5/20
GPIO Controller File
Configure pins for leds red and green
PB5 - red
PB6 - green

//Buttons - PD0 - PD5
//Joystick: EXTI0-EXTI5
//order: left, center, up, right, down, user button - bits 0-5
//
//Configure user button and center button as interrupt
//configure joystick up,down, left , right as polling
//PD1 - joystick center, PD5 - user button
//
*/

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#include "register.h"
#include "gpio.h"



///////////////////////////////////
//Configure LED GPIOs as output - PB5 and PB6
//Table 21 in the ref manual:
//pushpull output, DDR = 1, CR1 = 1, CR2 = 0
//user joystick - PD0-PD4
//user button - PD5
//input, pullup, interrupted:
//DDR = 0, CR1 = 1, CR2 = 1
void GPIO_init(void)
{
    PB_ODR &=~ BIT_5;       //output low
    PB_ODR &=~ BIT_6;
    
    PB_DDR |= BIT_5;
    PB_CR1 |= BIT_5;
    PB_CR2 &=~ BIT_5;

    PB_DDR |= BIT_6;
    PB_CR1 |= BIT_6;
    PB_CR2 &=~ BIT_6;
    
}




void GPIO_led_red_on(void)
{
    PB_ODR |= BIT_5;
}

void GPIO_led_green_on(void)
{
    PB_ODR |= BIT_6;
}

void GPIO_led_red_off(void)
{
    PB_ODR &=~ BIT_5;
}

void GPIO_led_green_off(void)
{
    PB_ODR &=~ BIT_6;
}

void GPIO_led_red_toggle(void)
{
    PB_ODR ^= BIT_5;
}

void GPIO_led_green_toggle(void)
{
    PB_ODR ^= BIT_6;
}

