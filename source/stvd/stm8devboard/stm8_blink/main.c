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
#include "spi.h"
#include "lcd.h"
#include "timer.h"

//prototypes


uint8_t txData[3] = {0xAA, 0xCC, 0x81};

/////////////////////////////////////////
//Main
main()
{
    system_disableInterrupts();
    
    
    system_init();
    system_clock_config();              //16mhz, internal
    system_peripheral_clock_config();   //spi, timer, etc
    
    GPIO_init();
    SPI_init();
    lcd_init();
    TIM2_init();
    TIM4_init();
    
    system_enableInterrupts();

	while (1)
    {
        GPIO_led_red_toggle();
        TIM2_start();        
        timer_delay_ms(100);
    
        GPIO_led_red_toggle();
        TIM2_stop();        
        timer_delay_ms(100);
    }
}



