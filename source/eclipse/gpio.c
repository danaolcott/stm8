/*///////////////////////////////////////////////////////
3/5/20
GPIO Controller File
Configure pins for leds red and green
PB5 - red
PB6 - green

//Buttons - PD0 - PD5
//Joystick: EXTI0-EXTI5
//order: left, center, up, right, down, user button - bits 0-5
//
//Configure user button as interrupt - PD5
//Configure center joystick button as interrupt - PD1
//
*////////////////////////////////////////////////////////

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
//joystick button - PD1
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

    //configure the user button on PD5 as interrupt
    //configure the joystick button on PD1 as interrupt
    //input, falling edge trigger
    //Bits 1 and 5 - DDR = 0, CR1 = 1, CR2 = 1
    PD_DDR &=~ 0x22;
    PD_CR1 |= 0x22;
    PD_CR2 |= 0x22;

    //configure the interrupt for PD5
    //EXTI_CR1, EXTI_CR2, EXTI_CONF1
    //falling edge trigger

    //EXTI_CR1 - this deals with only bits 0-3
    //for Ports A-D.  Note: writing 0x08 to this
    //register sets falling edge trigger on Bit 1
    //ie, PD1 (center joystick button)
    EXTI_CR1 = 0x08;

    //EXTI_CR2 - this deals with only bits 7-4
    //for ports A-D.  Writing 0x08 to this register
    //sets falling edge trigger interrupt for bit 5
    //ie, PD5 (user button)
    EXTI_CR2 = 0x08;

    //EXTI_CONF1 - determines if you use line interrupt
    //or port interrupt.  Bit3 clear sets line interrupt
    //on PortD, bits 7-4.  Bit2 clear sets line interrupt
    //on PortD bits 0-3
    EXTI_CONF1 &=~ BIT_3;
    EXTI_CONF1 &=~ BIT_2;

    
    //EXTI_SR1 - EXTI line pending interrupt bits
    //write a 1 to bits 1 and 5 for PD1 and PD5
    //to clear the interrupt
    
    EXTI_SR1 = 0x22;

}

/////////////////////////////////////////////////////
//Interrupt service routine for the joystick button - PD1
//EXTI1_ISR - joystick button PD1
//
void GPIO_EXTI1_ISR(void)
{
    EXTI_SR1 |= BIT_1;          	//clear the interrupt
    GPIO_led_red_toggle();		    //do something
}



/////////////////////////////////////////////////////
//Interrupt service routine for the user button - PD5
//EXTI5_ISR - user button
//
void GPIO_EXTI5_ISR(void)
{
    EXTI_SR1 |= BIT_5;          	//clear the interrupt
    GPIO_led_green_toggle();		//do something
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







