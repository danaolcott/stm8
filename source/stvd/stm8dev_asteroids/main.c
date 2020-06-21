/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 
6/3/20
Dana Olcott
Running Asteroids on the STM8 Game Development Board.
The purpose of this project is to reuse much of the 
code that was written for the Space Invaders game to 
build a simple Asteroids game.  The project uses the 
STM8L151K6 processor.  The pinouts are shown on the 
STM8 devboard from DanasBoatshop.com.  A schematic is
shown in the Github repository.


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
#include "sprite.h"

#define GAME_PRINT_BUFFER_SIZE      20

//variables in main.
static unsigned int gameLoopCounter = 0x00;
uint8_t length = 0x00;
static char printBuffer[GAME_PRINT_BUFFER_SIZE] = {0x00};
uint16_t cycleCounter = 0x00;
uint8_t launchResult = 0x00;
uint8_t missileFlag = 0x00;
uint16_t gameHighScore = 0x00;
uint16_t gameCurrentScore = 0x00;
static uint8_t toggleDisplay = 0;
static uint8_t powerUpFlag = 1;

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
    EEPROM_init();
    
    //init for the game
    Sprite_Init();
    Sprite_SetGameOverFlag();

    system_enableInterrupts();
    
    //clear the eeprom
    //EEPROM_clearEEPROM(0x00);
    
	while (1)
    {
        //check for game over flag
        if (Sprite_GetGameOverFlag() == 1)
        {
            /////////////////////////////////////
            //Game Over - Not on first powerup
            if (!powerUpFlag)
            {
                Sound_playGameOver();                

                gameCurrentScore = Sprite_GetGameScore();
                gameHighScore = Sprite_GetGameHighScore();
                
                if (gameCurrentScore > gameHighScore)
                {
                    Sprite_SetGameHighScore(gameCurrentScore);
                    
                    //display new high score routine....
                    while (Sprite_GetGameOverFlag() == 1)
                    {
                        if (toggleDisplay == 1)
                        {
                            lcd_clearPage(2, 0x00);
                            lcd_clearPage(3, 0x00);
                            lcd_clearPage(4, 0x00);
                            lcd_clearPage(5, 0x00);
                            
                            lcd_drawString(2, 20, "New High");
                            lcd_drawString(3, 30, "Score");
                            length = lcd_decimalToBuffer(gameCurrentScore, printBuffer, GAME_PRINT_BUFFER_SIZE);
                            lcd_drawStringLength(5, 40, printBuffer, length);
                            
                            toggleDisplay = 0;
                        }
                        else
                        {
                            //clear pages display press button
                            lcd_clearPage(1, 0x00);
                            lcd_clearPage(2, 0x00);
                            lcd_clearPage(3, 0x00);
                            lcd_clearPage(5, 0x00);
                            
                            toggleDisplay = 1;
                        }

                        GPIO_led_red_toggle();
                        timer_delay_ms(1000);
                    }
                    
                    //Set the gameover flag because button press
                    //cleared it.
                    Sprite_SetGameOverFlag();
                }
            }
            else
            {
                //first powerup
                powerUpFlag = 0;
            }

            ////////////////////////////////////////////////
            //reset the game, load the high score, etc.
            Sprite_Init();
            Sprite_SetGameOverFlag();
            gameHighScore = Sprite_GetGameHighScore();
            cycleCounter = EEPROM_updateCycleCount();
            
            ///////////////////////////////////////////////////
            //Gameover flag is cleared in the button interrupt
            while(Sprite_GetGameOverFlag() == 1)
            {
                if (toggleDisplay == 1)
                {
                    Sprite_DisplayGameOver();
                    toggleDisplay = 0;
                }
                else
                {
                    //clear pages display press button
                    lcd_clearPage(6, 0x00);
                    lcd_clearPage(7, 0x00);
                    
                    lcd_drawString(6, 0, "Score:");
                    length = lcd_decimalToBuffer(gameHighScore, printBuffer, GAME_PRINT_BUFFER_SIZE);
                    lcd_drawStringLength(6, 52, printBuffer, length);
                    
                    lcd_drawString(7, 0, "Game#");
                    length = lcd_decimalToBuffer(cycleCounter, printBuffer, GAME_PRINT_BUFFER_SIZE);
                    lcd_drawStringLength(7, 44, printBuffer, length);
                    
                    toggleDisplay = 1;
                }

                GPIO_led_red_toggle();
                timer_delay_ms(1000);                
            }
        }
        
        //check flags - missile launch
        if (Sprite_GetMissileLaunchFlag() == 1)
        {
            Sprite_ClearMissileLaunchFlag(); 
            launchResult = Sprite_Missile_Launch();
            
            if (launchResult == 1)
                Sound_playPlayerFire();
        }
        
        //check flags - player explode flag
        if (Sprite_GetPlayerHitFlag() == 1)
        {
            Sprite_ClearPlayerHitFlag();
        }
        
        //check flags - asteroid hit flag
        if (Sprite_GetAsteroidHitFlag() == 1)
        {
            Sprite_ClearAsteroidHitFlag();
            Sound_playEnemyExplode();
        }
        
        //check flags - levelup flag
        if (Sprite_GetLevelUpFlag() == 1)
        {
            Sprite_ClearLevelUpFlag();
            Sound_playLevelUp();
        }
        
        
        /////////////////////////////////////////////////
        //Check the joystick up, down, left, right
        //left - PD0
        if (!(PD_IDR & BIT_0))
            Sprite_PlayerRotateCCW();

        //up - PD2
        else if (!(PD_IDR & BIT_2))
            Sprite_PlayerFireThruster();

        //right - PD3
        else if (!(PD_IDR & BIT_3))
            Sprite_PlayerRotateCW();
        
        //down - PD4
        else if (!(PD_IDR & BIT_4))
            Sprite_PlayerFlipRotation();
        
        ////////////////////////////////////////////
        //Move items on the display and update
        system_disableInterrupts();
        lcd_clearFrameBuffer(0, 0);
        Sprite_Player_Move();
        Sprite_Astroid_Move();
        Sprite_Missile_Move();
        Sprite_UpdateDisplay();
		system_enableInterrupts();

		gameLoopCounter++;
        GPIO_led_green_toggle();
        timer_delay_ms(100);
    }
}
