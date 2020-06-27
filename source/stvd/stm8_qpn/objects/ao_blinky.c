/*****************************************************************************
* Product: Simple Blinky example
* Last updated for version 5.3.0
* Last updated on  2014-04-18
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, www.state-machine.com.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Alternatively, this program may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GNU General Public License and are specifically designed for
* licensees interested in retaining the proprietary status of their code.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* Contact information:
* Web:   www.state-machine.com
* Email: info@state-machine.com
*****************************************************************************/

//////////////////////////////////////////////////////////
//Blinky AO - 
//Controls the red and green LEDs.
//Blinky Off - Red on, Green flashing
//Blinky On - Green on, Red flashing
//
//Also uses timer module to blink the leds.  I can't seem
//to get timeout 1, 2, 3 to work, just 0.  Not sure why.
//Using these other timers, the system locks up.

//Button presses, up, down... etc toggles between blinky states


#include "qpn_port.h"
#include "bsp.h"
#include "main.h"
#include "gpio.h"
#include "sound.h"
#include "lcd.h"


///////////////////////////////////////////
//Local Objects
typedef struct BlinkyTag { /* the Blinky active object */
    QActive super;      /* derive from QActive */
    uint8_t count;
} Blinky;


///////////////////////////////////
//Global Object
Blinky AO_Blinky;


////////////////////////////////////////
/* hierarchical state machine ... */
static QState Blinky_initial(Blinky * const me);
static QState Blinky_off    (Blinky * const me);
static QState Blinky_on     (Blinky * const me);


/////////////////////////////////////////////
//constructor function
void Blinky_ctor(void)
{
	Blinky *me = &AO_Blinky;
    QActive_ctor(&me->super, Q_STATE_CAST(&Blinky_initial));
    me->count = 0;

}

/* HSM definition ----------------------------------------------------------*/
QState Blinky_initial(Blinky * const me) {

	//set up the initial state of leds
    GPIO_led_red_off();
    GPIO_led_green_off();

    //transition to off state
    return Q_TRAN(&Blinky_off);
}

////////////////////////////////////////////////////////
//Blinky off state - red on, green flashing at 2 hz
QState Blinky_off(Blinky * const me) {
    QState status;
    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG: {

        	me->count++;		//index the counter

        	//arm the timer.
        	QActive_armX((QActive *)me, 0U, BSP_TICKS_PER_SEC/2U);

            //entry - green off, red on
            GPIO_led_green_off();
            GPIO_led_red_on();
            
            lcd_clear(0x00);
            lcd_clearBackground(0xAA);
            lcd_drawString(3, 16, "BlinkyOff");

            
            status = Q_HANDLED();
            break;
        }

        case Q_EXIT_SIG:
        {
            QActive_disarm(&me->super);     //disarm the timer
            status = Q_HANDLED();
            break;
        }

        //posted from the button - any key
        case BUTTON_PRESS_USER_SIG:
        case BUTTON_PRESS_CENTER_SIG:
        case BUTTON_PRESS_UP_SIG:
        case BUTTON_PRESS_DOWN_SIG:
        case BUTTON_PRESS_LEFT_SIG:
        case BUTTON_PRESS_RIGHT_SIG:
        {
            status = Q_TRAN(&Blinky_on);
            break;
        }
        
        case Q_TIMEOUT_SIG:
        {
        	//toggle one of the leds
            GPIO_led_green_toggle();                                    //toggle green
        	QActive_armX((QActive *)me, 0U, BSP_TICKS_PER_SEC/2U);      //rearm the timer
            status = Q_HANDLED();
            break;
        }


        default: {
            status = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status;
}
/*..........................................................................*/
////////////////////////////////////////////////////////////////
//Blinky on state - green on , red flashing
//
QState Blinky_on(Blinky * const me)
{
    QState status;
    switch (Q_SIG(me))
    {
        case Q_ENTRY_SIG:
        {
            GPIO_led_green_on();
            GPIO_led_red_off();
            
            //draw something on the lcd - Blinky On!!
            lcd_clear(0x00);
            lcd_clearBackground(0xAA);
            lcd_drawString(3, 16, "BlinkyOn");
            
        	QActive_armX((QActive *)me, 0U, BSP_TICKS_PER_SEC/2U);  //Q_TIMEOUT_SIG
            status = Q_HANDLED();
            break;
        }

        case Q_EXIT_SIG:
        {
            QActive_disarm(&me->super);
            status = Q_HANDLED();
            break;
        }

        case Q_TIMEOUT_SIG:
        {
        	GPIO_led_red_toggle();
            
            //play a sound on timeout
            Sound_on();
            Sound_playBallBounceTile();

        	QActive_armX((QActive *)me, 0U, BSP_TICKS_PER_SEC/2U);  //timer 1
            status = Q_HANDLED();
            break;
        }

        //button signal from AO_Button contained a 32bit value
        //decode the lower 8 bits, number of button clicks
        //posted from the button - any key
        case BUTTON_PRESS_USER_SIG:
        case BUTTON_PRESS_CENTER_SIG:
        case BUTTON_PRESS_UP_SIG:
        case BUTTON_PRESS_DOWN_SIG:
        case BUTTON_PRESS_LEFT_SIG:
        case BUTTON_PRESS_RIGHT_SIG:
        {
            status = Q_TRAN(&Blinky_off);
            break;
        }
                
        default: {
            status = Q_SUPER(&QHsm_top);
            break;
        }
    }

    return status;
}



