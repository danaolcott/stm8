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
void GPIO_init(void);
void LED_On(void);
void LED_Off(void);
void LED_Toggle(void);

uint8_t Button_Read(void);

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





