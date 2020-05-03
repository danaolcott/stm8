/*
3/5/20
GPIO Controller File
Configure pins for leds red and green
PB5 - red
PB6 - green

//Joystick: EXTI0-EXTI5
//order: left, center, up, right, down, user button - bits 0-5

*/

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#include "register.h"
#include "gpio.h"
#include "button.h"



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
    
    //configure the input pins PD0-PD5
    //interrupt, pullups enabled, falling
    //edge trigger
    PD_DDR &=~ 0x3F;
    PD_CR1 |= 0x3F;
    PD_CR2 |= 0x3F;

    GPIO_interrupt_config();
}



//////////////////////////////////////////////
//Configure pin interrupts to run on Port D
//EXTI_CRx, EXTI_CONF1, others?
//PD0 - PD5 to run on interrupts, falling 
//edge trigger.  EXTI0 - EXTI5

//NOTE: CHeck ITC_SPRx - section 12.9.2
//This sets the priority of the interrupts
void GPIO_interrupt_config(void)
{
    //EXTI_CR1 - interrupt sensitivity for 
    //bits 3, 2, 1, 0 of Port A-E
    //Falling edge trigger only - 10, 1010 1010
    EXTI_CR1 = 0xAA;        //PD0-PD3 falling edge
    
    //EXTI_CR2 - interrupt sensitivity for 
    //bits 7, 6, 5, 4 for Port A-E
    //Falling edge trigger only - Bits 5 and 4 = 0000 1010
    EXTI_CR2 = 0x0A;
    
    //EXTI_CR3 - not sure about this one, for use 
    //with EXTID.

    //EXTI_CONF1 - selects for Portx - if EXTIn or EXTIx
    //is mapped to the interrupt.  ie, if you use 
    //EXTI0...5, or EXTID
    //bit 3 - PortD 7-4 - 0 = EXTI7-EXTI4 interrupt gen
    //bit 2 - PortD 3-0 - 0 = EXTI3-EXTI0 interrupt gen
    EXTI_CONF1 &=~ BIT_3;       //Use EXTIn for PortD
    EXTI_CONF1 &=~ BIT_2;       //Use EXTIn for PortD
    

    //EXTI_SR1 - EXTI line pending interrupt bits
    //clear by writing a 1
    //clear the interrupts by writting a 1
    //to bits 5-0
    EXTI_SR1 = 0x3F;
    
    //EXTI_SR2 - EXTID - bit 1 - 
    //Dont think there is a need for this one.
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

//order: left, center, up, right, down, user button - bits 0-5
////////////////////////////////////////////////
//EXTI0_ISR - joystick left
void GPIO_EXTI0_ISR(void)
{
    //do something
    Button_setFlag(BUTTON_LEFT);
    EXTI_SR1 |= BIT_0;          //write 1 to clear the interrupt
}


////////////////////////////////////////////////
//EXTI1_ISR - joystick center
void GPIO_EXTI1_ISR(void)
{
    //do something
    Button_setFlag(BUTTON_CENTER);
    EXTI_SR1 |= BIT_1;          //write 1 to clear the interrupt

}


////////////////////////////////////////////////
//EXTI2_ISR - joystick up
void GPIO_EXTI2_ISR(void)
{
    //do something
    Button_setFlag(BUTTON_UP);
    EXTI_SR1 |= BIT_2;          //write 1 to clear the interrupt

}

////////////////////////////////////////////////
//EXTI3_ISR - joystick right
void GPIO_EXTI3_ISR(void)
{
    //do something
    Button_setFlag(BUTTON_RIGHT);    
    EXTI_SR1 |= BIT_3;          //write 1 to clear the interrupt

}

////////////////////////////////////////////////
//EXTI4_ISR - joystick down
void GPIO_EXTI4_ISR(void)
{
    //do something
    Button_setFlag(BUTTON_DOWN);
    EXTI_SR1 |= BIT_4;          //write 1 to clear the interrupt

}


////////////////////////////////////////////////
//EXTI5_ISR - user button
void GPIO_EXTI5_ISR(void)
{
    //do something
    Button_setFlag(BUTTON_USER);
    EXTI_SR1 |= BIT_5;          //write 1 to clear the interrupt
}

