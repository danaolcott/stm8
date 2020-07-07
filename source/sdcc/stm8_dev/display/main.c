/*
Dana Olcott
June 29, 2020
STM8 Development Board - Display Project

The purpose of this project is to initialize the
102x64 display and draw some lines and characters.  
A framebuffer is used to store an instance of the
display contents.  Note: The framebuffer caused memory
issues when compiling projects in STVD.  The fix was to
use a different memory model.

This project does the following:
Flash leds
Draw lines on the lcd
Play sounds when the joystick is moved

*/

 
//#include <stddef.h>
#include <stdint.h>
#include "register.h"
#include "system.h"
#include "gpio.h"
#include "timer.h"
#include "spi.h"
#include "button.h"
#include "dac.h"
#include "sound.h"
#include "lcd.h"
#include "bitmap.h"

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
static uint8_t counter = 0;
static uint8_t direction = 0;
static uint16_t tick = 0;
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
    Button_init();
    DAC_init();
    Sound_init();
    Sound_on();
    lcd_init();

    lcd_clearFrameBuffer(0x00, 0);
    lcd_clearBackground(0xAA);
    lcd_updateFrameBuffer();

    lcd_drawString(2, 0, "12345678901234");
    
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

        //draw line on the display
        lcd_clear(0x00);
        lcd_clearFrameBuffer(0x00, 0);

        lcd_drawLine(10, 10, counter, 60-counter, 1, 0);
        lcd_drawLine(10, 60, counter, counter, 1, 0);
        lcd_drawLine(80, 50, 60-counter, counter, 1, 0);

        lcd_updateFrameBuffer();


        //increasing
        if (direction == 0)
        {
            if (counter < 60)
                counter++;
            else
                direction = 1;
        }
        else
        {
            if (counter > 0)
                counter--;
            else
                direction = 0;
        }

        tick++;

        if (!(tick%10))
        {
            GPIO_led_green_toggle();
            GPIO_led_red_toggle();
        }

        timer_delay_ms(50);
    }

}



