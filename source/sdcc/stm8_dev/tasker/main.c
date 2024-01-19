/*
Dana Olcott
Jan 18, 2024
STM8 Development Board - Simple tasker project

The purpose of this project is to implement a simple
tasker using a linked list and a timer.
*/

#include <stdint.h>
#include <stddef.h>
#include "register.h"
#include "system.h"
#include "gpio.h"
#include "timer.h"
#include "spi.h"
#include "dac.h"
#include "sound.h"
#include "font_table.h"
#include "lcd.h"
#include "bitmap.h"
#include "eeprom.h"
#include "list.h"
#include "task.h"
//////////////////////////////////////////
//Interrupt Service Routines
//ISR functions are in the corresponding
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
    GPIO_EXTI1_ISR();        //Button ISR
}

void EXTI5_InterruptHandler(void) __interrupt(EXTI5_IRQ)
{
    GPIO_EXTI5_ISR();        //Button ISR
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
    SPI_init();
    DAC_init();
    Sound_init();
    Sound_on();
    lcd_init();
    EEPROM_init();

    system_enableInterrupts();


    //clear game flags, etc.
    GPIO_led_green_off();
    GPIO_led_red_off();

    //initialize the list with task 1 and timeout 1000ms
    //task1_function and task2_function are in task.h/.c

    list_init("task1", 1000, &task1_function);

    //append new list items for each task
    list_appendNode("task2", 2000, &task2_function);

    while (1)
    {
        //run the tasker, should never exit this function
        //list_timeout() function is in the timer timeout
        //interrupt function.
        list_run();
    }
}





