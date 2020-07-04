/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 */
/*
The purpose of this program is to get the i2c
peripheral working to communicate with the 
BMP280 temp/pressure sensor.  Hardware files from
other projects were added for completeness

*/

#include <stdint.h>
#include "register.h"
#include "system.h"
#include "gpio.h"
#include "spi.h"
#include "i2c.h"
#include "lcd.h"
#include "timer.h"
#include "dac.h"
#include "button.h"
#include "eeprom.h"
#include "bitmap.h"
#include "bmp280.h"


static ButtonType_t lButton = 0x00;
static uint8_t result = 0x00;
uint8_t rx[10] = {0x00};
static BMP280_Data bmpData;
uint8_t printBuffer[16];
uint8_t length = 0;
uint16_t pInt = 0;
uint16_t pFrac = 0;
uint8_t counter = 0x00;
static unsigned long basePressure = 101600;
static int deltaFt = 0x00;
int tempFt = 0;


main()
{
    system_disableInterrupts();
    system_clock_config();              //16mhz, internal
    system_peripheral_clock_config();   //spi, timer, etc    
    system_init();                      //routing interface - after periph clock config.
    
    GPIO_init();
    SPI_init();
    I2C_init();
    lcd_init();
    TIM2_init();
    TIM4_init();
    DAC_init();
    EEPROM_init();
    BMP280_init();

    lcd_clear(0x00);
    lcd_clearFrameBuffer(0x00, 1);
    
    system_enableInterrupts();

    BMP280_read(&bmpData);      //initial readings
    
    //read the base pressure from eeprom
    BMP280_getBasePressure(&basePressure);

	while (1)
    {
        //poll button push for update base pressure
        //user button and center updates and writes to eeprom
        if ((!(PD_IDR & BIT_1)) && (!(PD_IDR & BIT_5))) 
        {
            basePressure = bmpData.cPressurePa;
            
            BMP280_setBasePressure(basePressure);
            
            GPIO_led_red_toggle();
            timer_delay_ms(100);
            GPIO_led_red_toggle();
            timer_delay_ms(100);
        }
        

        BMP280_read(&bmpData);

        deltaFt = ((signed long)basePressure - (signed long)bmpData.cPressurePa) / 3;
        
        //write the pressure and temp to the lcd
        lcd_clear(0x00);

        //temperature
        lcd_drawString(0, 0, "T:");
        length = lcd_decimalToBuffer((uint16_t)bmpData.cTemperatureF, printBuffer, 16);
        lcd_drawStringLength(0, 16, printBuffer, 16);
        
        //pressure
        lcd_drawString(1, 0, "P:");
        pInt = ((uint16_t)(bmpData.cPressurePa / 1000));
        pFrac = ((uint16_t)(bmpData.cPressurePa % 1000));
                
        length = lcd_decimalToBuffer(pInt, printBuffer, 16);
        lcd_drawStringLength(1, 16, printBuffer, length);
        
        lcd_drawString(1, 40, ".");
        length = lcd_decimalToBuffer(pFrac, printBuffer, 16);
        lcd_drawStringLength(1, 48, printBuffer, length);
        
        //basepressure
        lcd_drawString(3, 0, "BP:");
        pInt = ((uint16_t)(basePressure / 1000));
        pFrac = ((uint16_t)(basePressure % 1000));
                
        length = lcd_decimalToBuffer(pInt, printBuffer, 16);
        lcd_drawStringLength(3, 24, printBuffer, length);
        
        lcd_drawString(3, 48, ".");
        length = lcd_decimalToBuffer(pFrac, printBuffer, 16);
        lcd_drawStringLength(3, 56, printBuffer, length);
        
        
        //elevation
        lcd_drawString(4, 0, "FT:");
        if (deltaFt < 0)
        {
            lcd_drawString(4, 24, "-");
            tempFt = (deltaFt * -1);
            length = lcd_decimalToBuffer(tempFt, printBuffer, 16);
            lcd_drawStringLength(4, 32, printBuffer, length);
        }
        else
        {
            lcd_drawString(4, 24, "+");
            tempFt = deltaFt;
            length = lcd_decimalToBuffer(tempFt, printBuffer, 16);
            lcd_drawStringLength(4, 32, printBuffer, length);
        }
    
        GPIO_led_green_toggle();
        timer_delay_ms(500);
    }
    
}