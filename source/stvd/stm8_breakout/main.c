/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 */
/*
Dana Olcott, June 16, 2020
Breakout Game using the STM8 Game Development Board
The purpose of the is project is to implement a simple
verion of the Breakout game.


*/

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
#include "eeprom.h"
#include "bitmap.h"
#include "game.h"


//Globals
static ButtonType_t lButton = BUTTON_UP;

///////////////////////////////////////
//Main loop
main()
{
    //init the peripherals
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
    EEPROM_init();
    game_init();
    
    system_enableInterrupts();

    lcd_clear(0x00);
    lcd_drawString(0, 16, "Hello0");
    
    lcd_clearFrameBuffer(0x00, 0);
    lcd_drawIcon(10, 10, &bmimgPlayer_24, 1);
    
    lcd_drawIcon(10, 30, &bmimgBall_8, 1);
    
    //clear just the background
    lcd_clearBackground(0xAA);
    
    //draw the player, tiles, ball

    lcd_clearFrameBuffer(0x00, 0);
    game_player_draw(0);
    game_ball_draw(0);
    game_tile_draw(0);    
    lcd_updateFrameBuffer();
    
    
    
	while (1)
    {
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
                
                //do something
                case BUTTON_CENTER:
                {
                    GPIO_led_red_toggle();
                    break;
                }
                
                case BUTTON_USER:
                {
                    GPIO_led_red_toggle();
                    break;
                }
            }
            Button_clearFlag();
        }
        
        //check button presses - polling - up, down, left right
        //left - PD0
        if (!(PD_IDR & BIT_0))
        {
            lcd_drawString(2, 16, "LEFT    ");
        }
        //up - PD2
        else if (!(PD_IDR & BIT_2))
        {
            lcd_drawString(2, 16, "UP      ");
        }

        //right - PD3
        else if (!(PD_IDR & BIT_3))
        {
            lcd_drawString(2, 16, "RIGHT   ");
        }
        
        //down - PD4
        else if (!(PD_IDR & BIT_4))
        {
            lcd_drawString(2, 16, "DOWN    ");
        }
        


        //poll flags - game ball missed
        if (game_getBallMissedFlag() == 1)
        {
            game_clearBallMissedFlag();
            Sound_playBallMissed();
            
            //reset the game
            game_init();            
        }



        //move the objects on the screen
        game_player_move_auto();
        game_ball_move();
        
        lcd_clearFrameBuffer(0x00, 0);
        game_player_draw(0);
        game_ball_draw(0);
        game_tile_draw(0);    
        lcd_updateFrameBuffer();

        GPIO_led_green_toggle();
        timer_delay_ms(100);
    }
}





