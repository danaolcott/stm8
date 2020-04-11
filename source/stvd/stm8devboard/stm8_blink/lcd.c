/*
4/6/20
LCD Controller File for the EA Display DOGS102N-6 LCD
102x64 Display with SPI Interface.

*/

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#include "register.h"
#include "lcd.h"
#include "spi.h"

#define abs(a)  ((a)<0?-(a):a)



/////////////////////////////////////////////////////
//Configure the CD and RST pins, and register settings
//PA4 - LCD_CD
//PA5 - LCD_RST
void lcd_init(void)
{
    PA_ODR &=~ BIT_4;
    PA_ODR &=~ BIT_5;
    
    PA_DDR |= BIT_4;
    PA_CR1 |= BIT_4;
    PA_CR2 &=~ BIT_4;

    PA_DDR |= BIT_5;
    PA_CR1 |= BIT_5;
    PA_CR2 &=~ BIT_5;

    lcd_reset();

    //Configure the registers

}



void lcd_dummyDelay(unsigned long delay)
{
    volatile unsigned long temp = delay;
    while (temp > 0)
    {
        temp--;     
    }
}

//////////////////////////////////////////////
//LCD Reset - toggle the RST pin
void lcd_reset(void)
{
    PA_ODR |= LCD_RST_PIN;      //high
    PA_ODR &=~ LCD_RST_PIN;     //low
    lcd_dummyDelay(5000);       //about 100ms
    PA_ODR |= LCD_RST_PIN;      //high
}

void lcd_writeCommand(uint8_t cmd)
{
    PA_ODR &=~ LCD_CMD_PIN;       //CMD = low
    SPI_writeByte(cmd);
}

void lcd_writeData(uint8_t data)
{
    PA_ODR |= LCD_CMD_PIN;       //data = high
    SPI_writeByte(data);
}

void lcd_writeDataBurst(uint8_t *data, uint16_t length)
{
    PA_ODR |= LCD_CMD_PIN;       //data = high
    SPI_writeArray(data, length);
}



/////////////////////////////////////////////
//Set Page - 0 to 7, top to bottom.
//Base address = 0xB0, page address is
//the lower 4 bits
void lcd_setPage(uint8_t page)
{
    uint8_t result = 0xB0 | (page & 0x0F);
    lcd_writeCommand(result);
}


////////////////////////////////////////////
//Set Column - x offset
void lcd_setColumn(uint8_t column)
{
    uint8_t low = 0x00 | (column & 0x0F);       //lower 4 bits
    uint8_t high = 0x10 | (column >> 4);        //upper 4 bits
    lcd_writeCommand(low);                      //set the low bits
    lcd_writeCommand(high);                     //set the high bits 
}


///////////////////////////////////////////
//LCD Clear - clear the display with a value
void lcd_clear(uint8_t value)
{
    uint8_t i, j;

    for (i = 0 ; i < LCD_NUM_PAGES ; i++)
    {
        lcd_setPage(i);         //increment the page
        lcd_setColumn(0);       //reset the x
        
        for (j = 0 ; j < LCD_WIDTH ; j++)
            lcd_writeData(value);
    }
}






