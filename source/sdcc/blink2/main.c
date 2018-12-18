/*
Dana Olcott
12/12/18

Blink Project Using the STM8S103F3P6 Processor
Cheap board from ebay.
Pins Available:
PA1 - PA3
PB4 - PB5
PC3 - PC7
PD1 - PD6

Register Descriptions:
PB_ODR - output data register
PB_IDR - input data register
PB_DDR - data direction
PB_CR1 - control reg 1
PB_CR2 - control reg 2

See Table 21 - Page 107 in the ref manual
For output PP, fast mode, config ODR = 1, CR1 = 1, CR2 = 1

PB4 - User Button
PB5 - User LED
PC4 - PC7 - SPI
*/

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include "register.h"
#include "gpio.h"
#include "timer.h"

//////////////////////////////////////////////////
//Interrupt Handler Functions - Timer4_ISR
void Timer4_InterruptHandler(void) __interrupt(TIM4_IRQ)
{
    Timer4_ISR();			//Timer4 ISR
    TIM4_SR &=~ UIF_BIT;	//Clear the interrupt flag	
}


//EXTI1 - PortB
void EXTI1_InterruptHandler(void) __interrupt(EXTI1_IRQ)
{
	EXTI1_ISR();			//GPIO ISR
}



int main()
{
    GPIO_init();
    Timer4_init();

    while (1)
    {
		LED_Toggle();
		delay_ms(1000);
    }

    return 0;
}




