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
#include "lcd.h"


//globals
volatile uint8_t gpio_userButtonFlag = 0;
volatile uint8_t gpio_centerButtonFlag = 0;

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

    //configure PD1 and PD5 as input, falling
    //edge trigger, use EXTI line interrupts
    PD_DDR &=~ 0x22;
    PD_CR1 |= 0x22;
    PD_CR2 |= 0x22;
    
    GPIO_interrupt_config();
}



//////////////////////////////////////////////
//Configure pin interrupts to run on Port D
//EXTI_CRx, EXTI_CONF1, others?
//Consider 2 cases: PD0 - PD5 on interrupts,
//Case 2: PD1 and PD5 on interrupts.  Each case,
//configure as falling edge trigger
//
//NOTE: CHeck ITC_SPRx - section 12.9.2
//This sets the priority of the interrupts
void GPIO_interrupt_config(void)
{
    //EXTI_CR1 - interrupt sensitivity for 
    //bits 3, 2, 1, 0 of Port A-E
    //Falling edge trigger only - 10, 1010 1010
    //PD1 - center button
    EXTI_CR1 = 0x08;
        
    //EXTI_CR2 - interrupt sensitivity for 
    //bits 7, 6, 5, 4 for Port A-E
    //Falling edge trigger only - Bits 5 and 4 = 0000 1010
    //PD5 - user button
    EXTI_CR2 = 0x08;
    
    //EXTI_CONF1 - selects for Portx - if EXTIn or EXTIx
    //is mapped to the interrupt.  ie, if you use 
    //EXTI0...5, or EXTID
    //bit 3 - PortD 7-4 - 0 = EXTI7-EXTI4 interrupt gen
    //bit 2 - PortD 3-0 - 0 = EXTI3-EXTI0 interrupt gen
    EXTI_CONF1 &=~ BIT_3;       //Use EXTIn for PortD
    EXTI_CONF1 &=~ BIT_2;       //Use EXTIn for PortD
    
    //EXTI_SR1 - EXTI line pending interrupt bits
    //clear by writing a 1 to bits 1 and 5
    //for PD1 and PD5
    EXTI_SR1 = 0x22;
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


////////////////////////////////////////////////
//EXTI1_ISR - joystick center
void GPIO_EXTI1_ISR(void)
{
    EXTI_SR1 |= BIT_1;          //clear the interrrupt
    gpio_centerButtonFlag = 1;       //polled in main loop
}


////////////////////////////////////////////////
//EXTI5_ISR - user button
void GPIO_EXTI5_ISR(void)
{
    EXTI_SR1 |= BIT_5;          //clear the interrupt
    gpio_userButtonFlag = 1;         //polled in main loop
}



