/*
Dana Olcott
12/12/18

STM8SF103
User LED on PB5
User Button on PB4

*/

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include "register.h"
#include "gpio.h"

//Interrupt Service Routines
//
void Timer4_InterruptHandler(void) __interrupt(TIM4_IRQ)
{
    static uint32_t counter = 0x00;
    //toggle something
    if (!(counter % 10))
        PB_ODR ^= BIT_5;

    //clear the interrupt flag
    TIM4_SR &=~ UIF_BIT;

    counter++;
}

//prototypes
void Delay(unsigned int val);

void Timer_init(void);

//variables
volatile uint32_t counter = 0x00;
int main()
{
    GPIO_init();
    Timer_init();

    while (1)
    {
        if (!Button_Read())
        {
            if (!(counter % 50))
                LED_On();
            else
                LED_Off();
        }

//        else
//            LED_On();

        counter++;
        Delay(10000);
    }

    return 0;
}



////////////////////////////////////
//Generic delay function.  Not calibrated
//this was just to get the quad-core xmas 
//tree up and running
//
void Delay(unsigned int val)
{
    volatile unsigned int temp = val;
    while(temp > 0)
        temp--;
}


////////////////////////////////////////
//Timer_init
//Configure Timer4 to Timeout at 1khz
/* use the following registers
//Timer4 - Simplest one
#define TIM4_CR1        *((volatile uint8_t*)0x5340)
#define TIM4_IER        *((volatile uint8_t*)0x5343)
#define TIM4_SR         *((volatile uint8_t*)0x5344)
#define TIM4_EGR        *((volatile uint8_t*)0x5345)
#define TIM4_CNTR       *((volatile uint8_t*)0x5346)
#define TIM4_PSCR       *((volatile uint8_t*)0x5347)
#define TIM4_ARR        *((volatile uint8_t*)0x5348)

*/


void Timer_init(void)
{
    //enable global interrupts
    __asm__("rim");
    
    //TIM4_CR1 - interrupt on overflow and disable counter
    TIM4_CR1 = URS_BIT;

    //enable update interrupt
    TIM4_IER = UIE_BIT;

    //clear the counter
    TIM4_CNTR = 0x00;

    //prescaler - bits 0-2 - 2 to 128
    TIM4_PSCR = 0x07;

    //auto-reload register
    TIM4_ARR = 0xFF;

    //finally, enable the counter
    TIM4_CR1 |= CEN_BIT;

}





