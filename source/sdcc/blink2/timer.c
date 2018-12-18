/*
Timer Controller File

*/



//Interrupt Service Routines
//
void Timer4_InterruptHandler(void) __interrupt(TIM4_IRQ)
{
    PB_ODR ^= BIT_5;

    //clear the interrupt flag
    TIM4_SR &=~ UIF_BIT;
}


////////////////////////////////////////
//Timer_init
//Configure Timer4 to Timeout at 1khz
//
void Timer4_init(void)
{
    __asm__("rim");				//enable global interrupts
    
    TIM4_CR1 = URS_BIT;			//interrupt overflow / disable counter
    TIM4_IER = UIE_BIT;			//enable update interrupt
    TIM4_CNTR = 0x00;			//reset the counter
    TIM4_PSCR = 0x07;			//prescale bits 0-2 = 128
    TIM4_ARR = 0xFF;			//auto reload

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


