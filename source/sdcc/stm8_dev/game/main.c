/*
Dana Olcott
Jan 1, 2024
STM8 Development Board - Game Project

The purpose of this project is to make a game with 
a scrolling screen, similar to a ship flying over an
uneven ground surface shooting at incomming objects as 
they show up on the right side of the screen.


*/

#include <stdint.h>
#include "register.h"
#include "system.h"
#include "gpio.h"
#include "timer.h"
#include "spi.h"
#include "dac.h"
#include "sound.h"
#include "lcd.h"
#include "bitmap.h"
#include "game.h"

//////////////////////////////////////////
//Interrupt Service Routines
//ISR functions are in the cooresponding
//hardware files.
void Timer2_InterruptHandler(void) __interrupt(TIM2_IRQ)
{
    TIM2_ISR();			//Timer2 ISR
}


void Timer4_InterruptHandler(void) __interrupt(TIM4_IRQ)
{
    TIM4_ISR();			//Timer4 ISR
}

void EXTI1_InterruptHandler(void) __interrupt(EXTI1_IRQ)
{
    GPIO_EXTI1_ISR();        //Button ISR
}

void EXTI5_InterruptHandler(void) __interrupt(EXTI5_IRQ)
{
    GPIO_EXTI5_ISR();        //Button ISR
}



//////////////////////////////////////////
//Globals
Direction_t direction;
uint8_t collisionFlag = 0;
uint8_t num;


/////////////////////////////////////////
//Main
main()
{
    system_disableInterrupts();
    system_clock_config();              //16mhz, internal
    system_peripheral_clock_config();   //spi, timer, etc    
    system_init();                      //routing interface - after periph clock config.

    GPIO_init();
    TIM2_init();
    TIM4_init();
    SPI_init();
    DAC_init();
    Sound_init();
    Sound_on();
    lcd_init();

    system_enableInterrupts();

    //initialize the game, player, ground
    game_init();


    //clear game flags, etc.
    gpio_userButtonFlag = 0;
    gpio_centerButtonFlag = 0;
    collisionFlag = 0;
       
	while (1)
    {

        //move things - ground, player, enemy...
        game_updateGround();            //move the ground surface

        direction = game_readJoystick();                //read the joystick
        collisionFlag = game_playerMove(direction);     //move the player

        //collisionFlag - 0 no collision, 1 - hit the ground
        //2 - hit an enemy
        if (collisionFlag > 0)
        {
            Sound_playEnemyExplode();
            collisionFlag = 0;
            game_playerReset();

            //reduce the player count by 1, go into loop
            num = game_killPlayer();

            //test for remaining players
            if (!num)
            {
                gpio_centerButtonFlag = 0;
                GPIO_led_red_off();
                GPIO_led_green_off();

                //read the center button for press to reset
                while (!gpio_centerButtonFlag)
                {
                    //flash both leds
                    GPIO_led_red_toggle();
                    GPIO_led_green_toggle();
                    timer_delay_ms(500);
                }

                //reset the game
                game_init();
                gpio_centerButtonFlag = 0;
                gpio_userButtonFlag = 0;
            }

            //wait, reset player to default position.
            while (!gpio_centerButtonFlag)
            {
                GPIO_led_red_toggle();
                timer_delay_ms(200);
            }            
        }

        game_generateEnemy();           //test to make a new enemy
        game_moveEnemy();
        game_moveMissile();             //move missiles



        //test for buttons pressed - shoot missile
        //might be better to put the missile flag here
        //for missile hit enemy
        if (gpio_userButtonFlag == 1)
        {
            game_fireMissile();
            Sound_playPlayerShoot();
            gpio_userButtonFlag = 0;
        }


        //draw updated positions
        lcd_clearFrameBuffer(0x00, 0);
        game_drawGround(0);             //draw into frame buffer
        game_drawPlayer(0);             //draw into frame buffer
        game_drawEnemy(0);
        game_drawMissile(0);
        lcd_updateDisplay();            //copy buffer into display
        game_drawScore();               //overright the top line with score

        timer_delay_ms(50);

    }
}





