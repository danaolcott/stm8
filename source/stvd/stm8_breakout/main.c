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
static uint8_t length = 0;
static uint8_t printBuffer[16] = {0x00};
static uint8_t toggle = 0;
static uint8_t remaining = 0x00;

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
    
    Sound_off();
    
    system_enableInterrupts();

    //set initial display state
    lcd_clear(0x00);
    lcd_clearBackground(0xAA);    
    lcd_clearFrameBuffer(0x00, 0);

    game_player_draw(0);
    game_ball_draw(0);
    game_tile_draw(0);    
    lcd_updateFrameBuffer();

	while (1)
    {
        //buttons - center and user - interrupts
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
                
                //center button - toggle game mode - auto only
                case BUTTON_CENTER:
                {
                    if (game_getGameMode() == GAME_MODE_AUTO)
                    {
                        game_setGameMode(GAME_MODE_MANUAL);
                        game_init();
                    }
                    break;
                }

                //user button - toggle sound
                case BUTTON_USER:
                {
                    Sound_toggle();
                    break;
                }
            }
            Button_clearFlag();
        }

        ///////////////////////////////////////////////////////
        //check button presses - polling - up, down, left right
        //left - PD0
        if (!(PD_IDR & BIT_0))
        {
            //left - move player left
            if (game_getGameMode() == GAME_MODE_MANUAL)
                game_player_move_left();
                
        }
        //up - PD2
        else if (!(PD_IDR & BIT_2))
        {
            //do something
        }

        //right - PD3
        else if (!(PD_IDR & BIT_3))
        {
            //right - move player right
            if (game_getGameMode() == GAME_MODE_MANUAL)
                game_player_move_right();
        }
        
        //down - PD4
        else if (!(PD_IDR & BIT_4))
        {
            //do something
        }
        
        //poll flags - game over flag - press any key
        if (game_getGameOverFlag() == 1)
        {
            game_clearGameOverFlag();            
            Sound_playGameOver();
            
            //clear with center or user buttons
            while (!Button_getFlag())
            {
                //flash, and do something
                GPIO_led_green_toggle();
                GPIO_led_red_toggle();
                
                //display game over and clear
                if (toggle == 1)
                {
                    lcd_drawString(2, 12, "       ");
                    lcd_drawString(3, 12, "       ");
                    lcd_drawString(4, 16, "       ");
                    lcd_drawString(5, 16, "       ");
                    lcd_drawString(6, 18, "       ");
                    
                    toggle = 0;
                }
                else
                {
                    lcd_drawString(2, 12, "   Game");
                    lcd_drawString(3, 12, "   Over");
                    lcd_drawString(5, 16, "  Press");
                    lcd_drawString(6, 18, " Button");

                    toggle = 1;
                }
                timer_delay_ms(500); 
            }
            
            Button_clearFlag();
            
            //reset the game
            game_init();
            
        }
        
        //poll flags - game ball missed
        if (game_getBallMissedFlag() == 1)
        {
            game_clearBallMissedFlag();
            Sound_playBallMissed();
            game_ball_init();
            remaining = game_decrementPlayer();
            
            //check the number of players remaining
            if (!remaining)
                game_setGameOverFlag();
        }
        
        //poll flags - level up flag - all tiles clear
        if (game_getLevelUpFlag() == 1)
        {
            game_clearLevelUpFlag();    //clear the flag
            Sound_playLevelUp();        //sound play level up
            game_incrementGameLevel();
            game_tile_init();
            game_ball_init();

            //display something... and loop until
            //button press flag is set, clear it here
            //any button should set it
            while (!Button_getFlag())
            {
                //display new level
                if (toggle == 1)
                {
                    //lcd_decimalToBuffer(unsigned int val, char *buffer, uint8_t size);
                    lcd_drawString(3, 40, "Level");
                    length = lcd_decimalToBuffer(game_getGameLevel(), printBuffer, 16);
                    lcd_drawStringLength(5, 56, printBuffer, length);
                    
                    toggle = 0;
                }
                
                else
                {
                    
                    lcd_drawString(3, 16, "        ");
                    lcd_drawString(5, 16, "        ");
                    toggle = 1;
                }
                
                timer_delay_ms(500);
                GPIO_led_red_toggle();
            }
            
            Button_clearFlag();            
        }


        //move the objects on the screen
        if (game_getGameMode() == GAME_MODE_AUTO)
        {
            game_player_move_auto();
        }
        
        game_ball_move();
        
        lcd_clearFrameBuffer(0x00, 0);
        game_player_draw(0);
        game_ball_draw(0);
        game_tile_draw(0);    
        lcd_updateFrameBuffer();
        
        //draw the score, level, etc
        lcd_clearPage(0, 0x00);
        lcd_drawString(0, 0, "S:");
        lcd_drawString(0, 64, "L:");
        
        length = lcd_decimalToBuffer(game_getGameScore(), printBuffer, 16);
        lcd_drawStringLength(0, 16, printBuffer, length);

        length = lcd_decimalToBuffer(game_getGameLevel(), printBuffer, 16);
        lcd_drawStringLength(0, 80, printBuffer, length);
        
        //draw player icons - 3, 2, 1 players
        switch(game_getNumPlayers())
        {
            case 1: lcd_drawIconPage(0, 90, &bmimgPlayerIcon_1);    break;
            case 2: lcd_drawIconPage(0, 90, &bmimgPlayerIcon_2);    break;
            case 3: lcd_drawIconPage(0, 90, &bmimgPlayerIcon_3);    break;
            default: lcd_drawIconPage(0, 90, &bmimgPlayerIcon_3);    break;
        }


        GPIO_led_green_toggle();
        
        //game speed
        if (game_getGameLevel() == 1)
            timer_delay_ms(60);
        else if (game_getGameLevel() == 2)
            timer_delay_ms(55);
        else if (game_getGameLevel() == 3)
            timer_delay_ms(50);
        else
            timer_delay_ms(45);            
    }
}





