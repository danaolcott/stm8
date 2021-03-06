/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 */
/*
STM_Dev_ADC - This is a simple ADC project
The purpose of this project is to init the
adc peripheral to read the temperature on 
the development board.  There are also a few 
voltage deviders to read to estimate the battery
level and the temperature using a thermistor.

The hardware and display files in the project
are also used in other projects.

From the schematic:
ADC1_IN16_Temp - PB2 - Temp sensor MCP9700AT
ADC1_IN7_Temp - PD7 - Voltage divider, R15 10k and 
    thermistor, test point TP14

VREFINT - PD6 - supply voltage for thermistor, PD6.
    Digikey PN: BC3395CT-ND
    Should be able to read this for use in 
    computing the voltage supplied to the 
    thermistor
    

*/


#include "register.h"
#include "system.h"
#include "gpio.h"
#include "spi.h"
#include "lcd.h"
#include "timer.h"
#include "dac.h"
#include "button.h"
#include "eeprom.h"
#include "bitmap.h"
#include "adc.h"


static uint8_t result = 0x00;
uint8_t printBuffer[16];
uint8_t length = 0;
uint16_t adcResult = 0x00;
uint8_t counter = 0x00;
int temperature = 0x00;


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
    EEPROM_init();
    ADC_init();
    
    lcd_clear(0x00);
    lcd_clearFrameBuffer(0x00, 1);
    
    system_enableInterrupts();

	while (1)
    {
        //user button
        if (!(PD_IDR & BIT_1)) 
        {
            GPIO_led_green_toggle();
        }
        
        //center button
        if (!(PD_IDR & BIT_5)) 
        {
            GPIO_led_red_toggle();
            
            //output something to the dac
            for (counter = 0x00 ; counter < 200 ; counter++)
            {
                DAC_set(0xFFF);
                timer_delay_ms(1);
                DAC_set(0x00);
                timer_delay_ms(1);
                
            }
        }
        
        
        //display the result on the lcd
        lcd_clear(0x00);
        lcd_drawString(0, 0, "ADC Project");

        adcResult = ADC_read(ADC_CH16);
        length = lcd_decimalToBuffer(adcResult, printBuffer, 16);
        lcd_drawString(1, 0, "CH16:");
        lcd_drawStringLength(1, 40, printBuffer, length);
        
        adcResult = ADC_read(ADC_VREF);
        length = lcd_decimalToBuffer(adcResult, printBuffer, 16);
        lcd_drawString(2, 0, "VRF:");
        lcd_drawStringLength(2, 40, printBuffer, length);
      
        adcResult = ADC_read(ADC_FACTORY);
        length = lcd_decimalToBuffer(adcResult, printBuffer, 16);
        lcd_drawString(3, 0, "FAC:");
        lcd_drawStringLength(3, 40, printBuffer, length);
        
        lcd_drawString(5, 0, "Temp Sensor");
        
        adcResult = ADC_read_mv(ADC_CH16);
        length = lcd_decimalToBuffer(adcResult, printBuffer, 16);
        lcd_drawString(6, 0, "mv:");
        lcd_drawStringLength(6, 40, printBuffer, length);
        
        temperature = ADC_readTemperatureF();
        lcd_drawString(7, 0, "T:");        
        
        if (temperature < 0)
        {
            temperature = temperature * -1;
            length = lcd_decimalToBuffer((uint16_t)(temperature / 10), printBuffer, 16);
            lcd_drawString(7, 40, "-");
            lcd_drawStringLength(7, 48, printBuffer, length);
            lcd_drawString(7, 48 + 16, ".");
            length = lcd_decimalToBuffer((uint16_t)(temperature % 10), printBuffer, 16);
            lcd_drawStringLength(7, 48 + 24, printBuffer, length);
        }
        else
        {
            length = lcd_decimalToBuffer((uint16_t)(temperature / 10), printBuffer, 16);
            lcd_drawString(7, 40, "+");
            lcd_drawStringLength(7, 48, printBuffer, length);
            lcd_drawString(7, 48 + 16, ".");
            length = lcd_decimalToBuffer((uint16_t)(temperature % 10), printBuffer, 16);
            lcd_drawStringLength(7, 48 + 24, printBuffer, length);
        }
                
        GPIO_led_green_toggle();
        timer_delay_ms(500);
    }
    
}