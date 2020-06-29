/*
Dana Olcott
June 29, 2020
STM8 Development Board - Blink Project
The purpose of this project is to flash the 
leds on the STM8 developent board by Danasboatshop.com

Project that initializes leds red and green as outputs
and timers TIM2 and TIM4 to flash the LEDs.  This project
also initializes the DAC output to play sound over
the speaker when the joystick is moved or button is pushed.


*/

 
//#include <stddef.h>
#include <stdint.h>
#include "register.h"
#include "system.h"
#include "gpio.h"
#include "timer.h"
#include "button.h"
#include "dac.h"
#include "sound.h"

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


//Button
void EXTI1_InterruptHandler(void) __interrupt(EXTI1_IRQ)
{
    GPIO_EXTI1_ISR();        //Button ISR
}

void EXTI5_InterruptHandler(void) __interrupt(EXTI5_IRQ)
{
    GPIO_EXTI5_ISR();        //Button ISR
}



//////////////////////////////////////////
//Globals
static ButtonType_t lButton = BUTTON_UP;



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
    Button_init();
    DAC_init();
    Sound_init();
    Sound_on();
    
    system_enableInterrupts();
        
	while (1)
    {
        //buttons - center and user - interrupts
        lButton = Button_getFlag();

        if (lButton > 0)
        {
            switch(lButton)
            {
                //do nothing
                case BUTTON_UP:     
                case BUTTON_DOWN:
                case BUTTON_LEFT:
                case BUTTON_RIGHT:                
                    break;
                
                //center button
                case BUTTON_CENTER:
                {
                    GPIO_led_green_toggle();
                    break;
                }

                //user button
                case BUTTON_USER:
                {
                    GPIO_led_red_toggle();
                    break;
                }
            }

            Button_clearFlag();
        }

        ///////////////////////////////////////////////////////
        //check button presses - polling - up, down, left right
        //left - PD0
        if (!(PD_IDR & BIT_0))
        {
            Sound_play(&soundSquare_4ms_250hz, 50, 1);
        }
        //up - PD2
        else if (!(PD_IDR & BIT_2))
        {
            Sound_play(&soundSquare_4ms_500hz, 50, 1);
        }

        //right - PD3
        else if (!(PD_IDR & BIT_3))
        {
            Sound_play(&soundSquare_4ms_1000hz, 50, 1);
        }
        
        //down - PD4
        else if (!(PD_IDR & BIT_4))
        {
            Sound_play(&soundSquare_4ms_1800hz, 50, 1);
        }
        



        GPIO_led_green_toggle();
        GPIO_led_red_toggle();
        timer_delay_ms(500);
    }

}



