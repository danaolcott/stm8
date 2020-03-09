/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 */
 
/*
Dana Olcott
3/4/2020

Project that initializes various peripherals on the 
STM8L151K6 processor.  The pinouts are shown on the 
STM8 devboard from DanasBoatshop.com.  In general, the 
board has the following:

User LEDs - 
User Button -
Joystick - 
LCD, SPI enabled + 2 GPIOs, 102x64
ADC Temp sensors - 2
DAC output
Timer output
I2C output for use with BME280 breakout board

*/

 
#include <stddef.h>
#include <stdint.h>
#include "register.h"
#include "system.h"
#include "gpio.h"
 

//prototypes
void dummy_delay(uint16_t delay);

/////////////////////////////////////////
//Main
main()
{
    //configure peripherals
    GPIO_init();
    system_init();
    system_clock_config();
    system_peripheral_clock_config();

    system_disableInterrupts();
    system_enableInterrupts();
        
	while (1)
    {
        GPIO_led_green_toggle();
        GPIO_led_red_toggle();
        dummy_delay(50000);
    }
}



//////////////////////////////////////////////
//dummy_delay(uint16_t delay)
void dummy_delay(uint16_t delay)
{
    volatile uint16_t temp = delay;
    while (temp > 0)
    {
        temp--;
    }
}


