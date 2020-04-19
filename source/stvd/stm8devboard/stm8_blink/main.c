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
void dac_init(void);
void dac_set(unsigned int value);


uint8_t txData[3] = {0xAA, 0xCC, 0x81};
unsigned int counter = 0x00;

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
    dac_init();

    
    system_enableInterrupts();
    
	while (1)
    {
        //toggle the counter from 0 to 4000
        dac_set(counter);
                
        if (counter < 4095)
            counter = 4095;
        else
            counter = 0x00;
            
        GPIO_led_red_toggle();
        timer_delay_ms(10);
    }
}



/////////////////////////////////////////
//Configure the dac to output, assumes the 
//peripheral clock enabled and the switch
//routes to the appropriate pin.  system.h
//Leave GPIO pin config alone.  If you set to 
//output, it wont' work.
//Also, have to enable the COMP1 and COMP2
//clock before calling this function to allow RI to work.
void dac_init(void)
{    
    //DAC_CR1 Configure
    DAC_CR1 |= DAC_BIT_CR1_TSEL0;       //software trigger
    DAC_CR1 |= DAC_BIT_CR1_TSEL1;
    DAC_CR1 |= DAC_BIT_CR1_TSEL2;
    
    DAC_CR1 |= DAC_BIT_CR1_TEN;         //enable trigger
    DAC_CR1 &=~ DAC_BIT_CR1_BOFF;       //disable output buffer
    DAC_CR1 |= DAC_BIT_CR1_EN;          //enable the dac output
}

/////////////////////////////////////////
//Dac Output - 12 bit value outptu to PB4
void dac_set(unsigned int value)
{
    unsigned int low = 0x00;
    unsigned int high = 0x00;
    
    low = value & 0xFF;
    high = (value >> 8) & 0x0F;
    
    //write the data - assuming right aligned
    DAC_RDHRH = high;
    DAC_RDHRL = low;
    
    //write data assuming left aligned
//    DAC_LDHRH = ((value >> 4) & 0xFF);
//    DAC_LDHRL = ((value << 4) & 0xF0);

    //odd - this bit does not go high....
    //set the software trigger bit, cleared by
    //hardware once the data is written
    DAC_SWTRIGR |= BIT_0;

    //wait, BIT_0 is cleared by hardware
    while (DAC_SWTRIGR & BIT_0){};
    
}


