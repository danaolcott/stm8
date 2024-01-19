/*
 * task.c
Task functions called in the tasker / linked list
 */


#include <stddef.h>
#include <stdlib.h>

#include "task.h"
#include "gpio.h"
#include "lcd.h"

void task1_function(void)
{
    uint8_t buffer[16];
    uint8_t length;
    static uint8_t counter = 0;
    GPIO_led_green_toggle();

    lcd_drawString(0, 0, "Task 1:");
    lcd_clearPage(1, 0x00);
    length = lcd_decimalToBuffer(counter, buffer, 16);
    lcd_drawStringLength(1, 0, buffer, length);

    //test for a button press
    if (gpio_userButtonFlag == 1)
    {
        counter = 0;
        gpio_userButtonFlag = 0;
    }

    counter++;
}

void task2_function(void)
{
    uint8_t buffer[16];
    uint8_t length;
    static uint8_t counter = 0;
    GPIO_led_red_toggle();

    lcd_drawString(3, 0, "Task 2:");
    lcd_clearPage(4, 0x00);
    length = lcd_decimalToBuffer(counter, buffer, 16);
    lcd_drawStringLength(4, 0, buffer, length);

    counter++;
}

