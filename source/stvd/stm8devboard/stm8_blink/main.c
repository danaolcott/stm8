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

problems:
blue board - left no trigger interrupt
blue board - PA5 - lcd reset - no output, pin 5 not soldered?
blue board - sck ok, no output on MOSI pin, pin 3 on mcu

*/

 
//#include <stddef.h>
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

static ButtonType_t lButton = 0x00;

//variables in main.
static unsigned int gameLoopCounter = 0x00;
uint8_t length = 0x00;
static char printBuffer[GAME_PRINT_BUFFER_SIZE] = {0x00};
uint16_t cycleCounter = 0x00;
uint8_t launchResult = 0x00;
uint8_t missileFlag = 0x00;
uint16_t gameHighScore = 0x00;
uint16_t gameCurrentScore = 0x00;



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
    Game_init();
    
    system_enableInterrupts();
    
    //clear the eeprom
    //EEPROM_clearEEPROM(0x00);
    
	while (1)
    {
        //check for button presses - ISR
        //user button and center button
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
                    break;
                }
                
                case BUTTON_USER:
                {
                    launchResult = Game_missilePlayerLaunch();
                    if (launchResult == 1)
                    Sound_playPlayerFire();
                    break;
                }
            }
            
            Button_clearFlag();
        }
        
        //check button presses - polling - up, down, left right
        //left - PD0
        if (!(PD_IDR & BIT_0))
        {
            //do something
            Game_playerMoveLeft();
        }
        //up - PD2
        else if (!(PD_IDR & BIT_2))
        {
            //do something
        }

        //right - PD3
        else if (!(PD_IDR & BIT_3))
        {
            //do something
            Game_playerMoveRight();
        }
        
        //down - PD4
        else if (!(PD_IDR & BIT_4))
        {
            //do something
        }
        
        
        //check flag enemy missile launch
		if (!(gameLoopCounter % 10))
		{
			launchResult = Game_missileEnemyLaunch();
			if (launchResult == 1)
				Sound_playEnemyFire();
		}
		
		//check flag player hit
		if (Game_flagGetPlayerHitFlag() == 1)
		{
			Game_flagClearPlayerHitFlag();
			Game_playExplosionPlayer_withSound();
		}
		
		//check flag - enemy hit flag
		if (Game_flagGetEnemyHitFlag() == 1)
		{
			Game_flagClearEnemyHitFlag();
			Sound_playEnemyExplode();
		}
		
		//check level up flag
		if (Game_flagGetLevelUpFlag() == 1)
		{
			Game_flagClearLevelUpFlag();	//clear the flag
			Game_levelUp();					//level up			
			Sound_playLevelUp();	//play sound
		}

		//check flag - game over
		if (Game_flagGetGameOverFlag() == 1)
		{
			Sound_playGameOver();
            
            //update the cycle counter - eeprom
            cycleCounter = EEPROM_updateCycleCount();
            
            //check to see if the game score is more than the high score
            gameCurrentScore = Game_getGameScore();
            gameHighScore = EEPROM_getHighScore();
            
            if (gameCurrentScore > gameHighScore)
            {
                gameHighScore = gameCurrentScore;
                EEPROM_updateHighScore(gameHighScore);
                
                //set the highscore in game - displayed below
                Game_setHighScore(gameHighScore);
            }
            
            
                        
            //loop until button press
            while (Game_flagGetGameOverFlag() == 1)
            {
                Game_playGameOver();
                
                //draw the new cycle counter
                lcd_drawString(1, 0, "Game#:");
                length = lcd_decimalToBuffer(cycleCounter, printBuffer, GAME_PRINT_BUFFER_SIZE);
                lcd_drawStringLength(1, 50, printBuffer, length);
                
                //check to see if anything pressed....
                //check for any button flags
                lButton = Button_getFlag();
                if (lButton > 0)
                {
                    Game_flagClearGameOverFlag();
                    system_disableInterrupts();
                    Game_init();
                    system_enableInterrupts();
                }

                GPIO_led_red_toggle();
                timer_delay_ms(500);
            }
        }

		//move enemy and missile				
		Game_enemyMove();					//move enemy
		missileFlag = Game_missileMove();	//move all missiles

		//update display with interrupts disabled
		system_disableInterrupts();      	//stop the timer
		lcd_clearFrameBuffer(0, 0);			//clear the ram buffer
		Game_playerDraw();					//update player image
		Game_enemyDraw();					//draw enemy
		Game_missileDraw();					//draw missiles
		lcd_updateFrameBuffer();			//update the display
		
		//display the header info - score, level, num players
		lcd_drawString(0, 0, "S:");
		length = lcd_decimalToBuffer(Game_getGameScore(), printBuffer, GAME_PRINT_BUFFER_SIZE);
		lcd_drawStringLength(0, 18, printBuffer, length);

		lcd_drawString(0, 60, "L:");
		length = lcd_decimalToBuffer(Game_getGameLevel(), printBuffer, GAME_PRINT_BUFFER_SIZE);
		lcd_drawStringLength(0, 74, printBuffer, length);
		
		switch(Game_getNumPlayers())
		{
			case 3:	lcd_drawImagePage(0, 90, BITMAP_PLAYER_ICON3);	break;
			case 2:	lcd_drawImagePage(0, 90, BITMAP_PLAYER_ICON2);	break;
			case 1:	lcd_drawImagePage(0, 90, BITMAP_PLAYER_ICON1);	break;
		}

		//reenable interrupts
		system_enableInterrupts();

        //loop cound and short delay
		gameLoopCounter++;
        GPIO_led_green_toggle();
        timer_delay_ms(100);
    }
}
