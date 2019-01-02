/*
Timer Controller File

*/

#include "register.h"
#include "timer.h"


static volatile uint32_t TimeTick = 0x00;


//////////////////////////////////////
//Delay Function that Uses Timer4
void delay_ms(uint32_t delay)
{
	TimeTick = delay;		//set the timeout
	while (TimeTick > 0);	//wait - decremented in isr
}


////////////////////////////////////////
//Timer_init
//Configure Timer4 to Timeout at 1khz
//Prescale = 64 (2^6) and reload = 0xF8
//based on CPU = 16mhz
void Timer4_init(void)
{    
    TIM4_CR1 = URS_BIT;			//interrupt overflow / disable counter
    TIM4_IER = UIE_BIT;			//enable update interrupt
    TIM4_CNTR = 0x00;			//reset the counter
    TIM4_PSCR = 0x06;			//prescale bits 0-2 = 64
    TIM4_ARR = 0xF8;			//auto reload - trim to 1khz
    TIM4_CR1 |= CEN_BIT;		//enable the counter
}


///////////////////////////////////////
//Start the timer
void Timer4_start(void)
{
    TIM4_IER = UIE_BIT;			//enable interrupt - overflow
    TIM4_SR &=~ UIF_BIT;		//clear the interrupt flag
    TIM4_CNTR = 0x00;			//reset the counter
    TIM4_CR1 |= CEN_BIT;		//enable the counter
}


/////////////////////////////////////////
//Stop the timer
void Timer4_stop(void)
{
    TIM4_CR1 &=~ CEN_BIT;		//disable the counter
    TIM4_IER &=~ UIE_BIT;		//disable interrupts - overflow
    TIM4_SR &=~ UIF_BIT;		//clear the interrupt flag
    TIM4_CNTR = 0x00;			//reset the counter
}


//////////////////////////////////////////
//Timer4 ISR - Configured to run at 1khz
void Timer4_ISR(void)
{
	TimeTick--;
}




///////////////////////////////////////////
///////////////////////////////////////////
//Timer2
//Setup Timer2 same as TIM4, overflow on some
//given frequency.
void Timer2_init(void)
{
    TIM2_CR1 = URS_BIT;			//interrupt overflow / disable counter
    TIM2_IER = UIE_BIT;			//enable update interrupt

    TIM2_CNTRH = 0x00;			//reset the counter
    TIM2_CNTRL = 0x00;			//reset the counter

    TIM2_PSCR = 0x06;			//prescale bits 0-2 = 64

    TIM2_ARRH = 0x00;			//auto reload - trim to 1khz
    TIM2_ARRL = 0xF8;			//auto reload - trim to 1khz

    TIM2_CR1 |= CEN_BIT;		//enable the counter

}

void Timer2_start(void)
{
    TIM2_IER = UIE_BIT;			//enable interrupt - overflow
    TIM2_SR1 &=~ UIF_BIT;		//clear the interrupt flag

    TIM2_CNTRH = 0x00;			//reset the counter
    TIM2_CNTRL = 0x00;			//reset the counter

    TIM2_CR1 |= CEN_BIT;		//enable the counter
}


void Timer2_stop(void)
{
    TIM2_CR1 &=~ CEN_BIT;		//disable the counter
    TIM2_IER &=~ UIE_BIT;		//disable interrupts - overflow
    TIM2_SR1 &=~ UIF_BIT;		//clear the interrupt flag
    TIM2_CNTRH = 0x00;			//reset the counter
    TIM2_CNTRL = 0x00;			//reset the counter
}

void Timer2_ISR(void)
{
//	TimeTick--;
}









