/*
STM8 Development Board - Blink Project
The purpose of this project is to flash the 
leds on the STM8 developent board by Danasboatshop.com

Project that initializes various peripherals on the 
STM8L151K6 processor.  The pinouts are shown on the 
STM8 devboard from DanasBoatshop.com.  A schematic is shown in the
Github repository.  This project will implement many of the 
peripherals and build a simple game Space Invaders.

Note: Many of the game files sounds, images, etc are used
from the MC9S08QE8 dev board project.

Several of the peripherals are listed below:

User LEDs - red and green
User Button - button - right
Joystick - up/down/left/right/center
LCD, SPI enabled + 2 GPIOs, 102x64
ADC Temp sensors - 2 - using temp sensor and thermistor
DAC output - used for sound output
Timer output - system timer, 1khz, and sound timer 11khz
I2C output for use with BME280 breakout board - to be installed


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



