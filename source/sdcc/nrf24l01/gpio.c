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
//Configure PB4 button as interrupt, trigger
//on falling, pullup enabled.
//
void GPIO_init(void)
{
    //configure PB5 as output
    PB_DDR = BIT_5;
    PB_CR1 = BIT_5;
    PB_CR2 = BIT_5;


	///////////////////////////////////////////////////////
    //PB4 - input, pullup - PB_CR1 = 1 for pullup, PB_DDR = 0 input
	//Button with interrupts.  See 66 in the ref. manual.  
	//Registers EXTI_CR1 and EXTI_CR2.  EXTI1 - PortB - PortB_IRQ
    PB_DDR &=~ BIT_4;       //input
    PB_CR1 |= BIT_4;        //pullup
    PB_CR2 |= BIT_4;        //interrupt enabled

	//Interrupt Config
	//CCR reg = 0x28 - default, set I1 and I0 to 1.
	//required to enable external interrupts
    //PortB, falling edge - bit 3 high, bit 2 low
    EXTI_CR1 |= BIT_3;
    EXTI_CR1 &=~ BIT_2;

	//EXTI_CR2 - PortD and PortE - don't care
	
    //init all outputs low
    PB_ODR = 0x00;
}


////////////////////////////////////////
//External Interrupt
void EXTI1_ISR(void)
{
	LED_Toggle();
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




