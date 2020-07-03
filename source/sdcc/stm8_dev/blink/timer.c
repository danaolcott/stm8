/*
4/10/20
Timer Controller File
Configure 2 timers, one for the system clock
and delays and the other for audio, 11khz
Using TIM2 and TIM4
These are just software timers with interrupt
These likely will run similar to those on the
other stm8 103..? board
*/

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#include "register.h"
#include "timer.h"
#include "gpio.h"
#include "system.h"

volatile unsigned long timerTick = 0x00;



///////////////////////////////////////////////////
void timer_delay_ms(unsigned long delay)
{
    volatile unsigned long temp;
    temp = timerTick + delay;
    while (temp > timerTick);
}




//////////////////////////////////////////////
//Configure Timer2 to run at 11khz and interrupt
//on overflow.  Will be used by the sound controller
//Interrupt uses IRQ19
void TIM2_init(void)
{
    TIM2_CR1 |= BIT_2;      //interrupt generated on counter overflow
    TIM2_IER |= BIT_0;      //update interrupt enabled
    TIM2_SR1 &=~ BIT_0;     //clear update interrupt flag
    TIM2_CNTRH = 0x00;       //clear the timer counter
    TIM2_CNTRL = 0x00;       //clear the timer counter
    //2^3, set to 1mhz
    TIM2_PSCR |= 0x03;      //prescaller, check this, trial and error    
    TIM2_ARRH = 0x00;       //auto reload, trial and error    
    TIM2_ARRL = 0xBA;       //auto reload, trial and error

    TIM2_CR1 |= BIT_0;      //enable counter    
}



//////////////////////////////////////////////
//Configure TIM4 to timeout at 1khz
//See Section 21.5
//Make sure to enable the peripheral clock for
//TIM4 before doing anything with this
void TIM4_init(void)
{
    timerTick = 0x00;       //master counter
    
    TIM4_CR1 |= BIT_2;      //interrupt generated on counter overflow
    TIM4_IER |= BIT_0;      //update interrupt enabled
    TIM4_SR1 &=~ BIT_0;     //clear update interrupt flag
    TIM4_CNTR = 0x00;       //clear the timer counter
    TIM4_PSCR |= 0x08;      //prescaller, check this, trial and error
    TIM4_ARR = 0x3D;        //auto reload, trial and error, 0x3D gives 1khz
    TIM4_CR1 |= BIT_0;      //enable counter
}



void TIM2_start(void)
{
    TIM2_CNTRH = 0x00;      //clear the timer counter
    TIM2_CNTRL = 0x00;      //clear the timer counter
    TIM2_SR1 &=~ BIT_0;     //clear update interrupt flag
    TIM2_IER |= BIT_0;      //update interrupt enabled
    TIM2_CR1 |= BIT_0;      //enable counter    
}


void TIM2_stop(void)
{
    TIM2_CR1 &=~ BIT_0;     //disable the counter
    TIM2_IER &=~ BIT_0;     //update interrupt disabled
    TIM2_SR1 &=~ BIT_0;     //clear update interrupt flag
    TIM2_CNTRH = 0x00;      //clear the timer counter
    TIM2_CNTRL = 0x00;      //clear the timer counter
}




void TIM2_ISR(void)
{
    TIM2_SR1 &=~ BIT_0;     //clear update interrupt flag
    //do something
}


void TIM4_ISR(void)
{
    TIM4_SR1 &=~ BIT_0;     //clear update interrupt flag
    timerTick++;
}








