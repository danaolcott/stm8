/*
Dana Olcott
June 29, 2020
STM8 Development Board - Blink Project
The purpose of this project is to flash the 
leds on the STM8 developent board by Danasboatshop.com

Project that initializes leds red and green as outputs,
user button and center joystick button as input falling
edge interrupts, and timers TIM2 and TIM4 to flash the LEDs.


*/

//#include <stddef.h>
#include <stdint.h>
#include "register.h"
#include "system.h"
#include "gpio.h"
#include "timer.h"



//////////////////////////////////////////
//Interrupt Service Routines
//ISR functions are in the cooresponding
//hardware files.
void Timer2_InterruptHandler(void) __interrupt(TIM2_IRQ)
{
    TIM2_ISR();			//Timer2 ISR
}


void Timer4_InterruptHandler(void) __interrupt(TIM4_IRQ)
{
    TIM4_ISR();			//Timer4 ISR
}

void EXTI1_InterruptHandler(void) __interrupt(EXTI1_IRQ)
{
    GPIO_EXTI1_ISR();        //Joystick Button ISR
}


void EXTI5_InterruptHandler(void) __interrupt(EXTI5_IRQ)
{
    GPIO_EXTI5_ISR();        //User Button ISR
}


/////////////////////////////////////////
//Main
main()
{
    system_disableInterrupts();
    system_clock_config();              //16mhz, internal
    system_peripheral_clock_config();   //spi, timer, etc    
    system_init();                      //routing interface - after periph clock config.

    GPIO_init();
    TIM2_init();
    TIM4_init();
    
    system_enableInterrupts();
        
	while (1)
    {
        GPIO_led_green_toggle();
        GPIO_led_red_toggle();
        timer_delay_ms(500);
    }

}



