/*
7/18/20
Timer controller file that uses the stm8 
standard library for initializing the timer
peripheral.


*/

#include "main.h"
#include "timer.h"


volatile uint32_t gTimerTick = 0x00;

void delay_ms(uint32_t delay)
{
    volatile uint32_t temp = gTimerTick + delay;
    while (gTimerTick < temp);
}

/////////////////////////////////////////////
//Configure timer2 to timeout at 1ms and 
//generate an interrupt
//Assuming base clock = 1mhz, prescale = 2, period of 
//(clock / 1000) - 1 gives a timeout at 1ms. 
void tim2_init(void)
{
    uint16_t period = 999;
    TIM2_DeInit();
    
    //enable internal clock
    TIM2_InternalClockConfig();
    TIM2_TimeBaseInit(TIM2_Prescaler_2, TIM2_CounterMode_Up, period);
    
    //configure interrupt - see stm8_interrupt_vector.c
    TIM2_ITConfig(TIM2_IT_Update, ENABLE);

    //start the timer
    TIM2_Cmd(ENABLE);
}




void tim2_isr(void)
{
    //clear the interrupt
    TIM2_ClearITPendingBit(TIM2_IT_Update);

    gTimerTick++;
}










