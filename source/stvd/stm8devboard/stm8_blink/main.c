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
#include "dac.h"
#include "sound.h"
#include "button.h"


static ButtonType_t lButton = 0x00;

/////////////////////////////////////////
//Main
main()
{
    system_disableInterrupts();
    system_clock_config();              //16mhz, internal
    system_peripheral_clock_config();   //spi, timer, etc    
    system_init();                      //routing interface - after periph clock config.

    GPIO_init();
    SPI_init();
    lcd_init();
    TIM2_init();
    TIM4_init();
    DAC_init();
    Sound_init();
    
    system_enableInterrupts();
    
	while (1)
    {
        //check for any button flags
        lButton = Button_getFlag();
        
        if (lButton > 0)
        {
            switch(lButton)
            {
                case BUTTON_UP:
                    Sound_play(&wavSoundEnemyExplode, 1);
                    break;
                case BUTTON_DOWN:
                    Sound_play(&wavSoundEnemyExplode, 2);
                    break;
                case BUTTON_LEFT:
                    Sound_play(&wavSoundPlayerFire, 1);
                    break;
                case BUTTON_RIGHT:
                    Sound_play(&wavSoundPlayerFire, 2);
                    break;
                case BUTTON_CENTER:
                    Sound_play(&soundTest2, 5000);
                    break;
                case BUTTON_USER:
                    Sound_play(&soundTest1, 5000);
                    break;
            }
            
            Button_clearFlag();
        }
        
        GPIO_led_green_toggle();
        timer_delay_ms(100);
    }
}



