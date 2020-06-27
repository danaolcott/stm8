/*
 * button.c
 *
 *  Button active object.  this has one state - running
 *  that polls the button and posts an event
 *  BUTTON_PRESS_SIG when pressed
 */

#include "qpn_port.h"
#include "bsp.h"
#include "main.h"
#include "button.h"
#include "register.h"

///////////////////////////////////////////
//Local Objects
typedef struct ButtonTag { /* the Blinky active object */
    QActive super;      /* derive from QActive */
    uint8_t btnClick;

} Button;

///////////////////////////////////
//Global Object
Button AO_Button;

//globals for other use
static uint32_t send;
static uint8_t signal;
static uint8_t center;
static uint8_t user;
static uint8_t up;
static uint8_t down;
static uint8_t left;
static uint8_t right;
static uint8_t flag;



////////////////////////////////////////
/* hierarchical state machine ... */
static QState Button_initial	(Button * const me);
static QState Button_Active    	(Button * const me);


/////////////////////////////////////////////
//constructor function
void Button_ctor(void)
{
	Button *me = &AO_Button;

	//init AO_Button variables
	me->btnClick = 0;

    QActive_ctor(&me->super, Q_STATE_CAST(&Button_initial));
}

/* HSM definition ----------------------------------------------------------*/
QState Button_initial(Button * const me)
{
    return Q_TRAN(&Button_Active);
}
/*..........................................................................*/
QState Button_Active(Button * const me)
{
    QState status;
    
    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG: {

            //poll for button presses - 50 times per second
        	QActive_armX((QActive *)me, 0U, BSP_TICKS_PER_SEC/20U);
            status = Q_HANDLED();
            break;
        }

        case Q_EXIT_SIG:
        {
            QActive_disarm(&me->super);     //disarm the timer
            status = Q_HANDLED();
            break;
        }

        case Q_TIMEOUT_SIG:
        {
            //read the state of the buttons and joystick
            flag = 0;
            user = PD_IDR & BIT_1;
            center = PD_IDR & BIT_5;
            left = PD_IDR & BIT_0;
            up = PD_IDR & BIT_2;
            right = PD_IDR & BIT_3;
            down = PD_IDR & BIT_4;
            
            //set the signal
            if (!user)
            {
                signal = BUTTON_PRESS_USER_SIG;
                flag = 1;
            }  
            else if (!center)
            {
                signal = BUTTON_PRESS_CENTER_SIG;
                flag = 1;
            }
            else if (!up)
            {
                signal = BUTTON_PRESS_UP_SIG;
                flag = 1;
            }
            else if (!down)
            {
                signal = BUTTON_PRESS_DOWN_SIG;
                flag = 1;
            }
            else if (!left)
            {
                signal = BUTTON_PRESS_LEFT_SIG;
                flag = 1;
            }
            else if (!right)
            {
                signal = BUTTON_PRESS_RIGHT_SIG;
                flag = 1;
            }
                        
        	if (flag == 1)
        	{
        		//index the button click
        		++me->btnClick;

        		//post the button press sig with number of button clicks.
        		//all events hold a 32bit value (defined in qpn_port.h).
        		//pack button clicks and other info in the event

        		//Pass No Data
        		//QACTIVE_POST((QActive *)&AO_Blinky, BUTTON_PRESS_SIG, 0U);

        		//pass the number of button clicks to the button press signal event post
        		send = ((uint32_t)me->btnClick) << 24;
        		send |= ((uint32_t)me->btnClick) << 16;
        		send |= ((uint32_t)me->btnClick) << 8;
        		send |= ((uint32_t)me->btnClick);

        		//Pass 32bit value
            	QACTIVE_POST((QActive *)&AO_Blinky, signal, send);

            	//Note:
            	//To decode the data on the receiver end:
            	//In Button_Press_Sig handler,
            	//Simply do the following:

            	//uint8_t value1 = (uint8_t)Q_PAR(me);
            	//uint8_t value4 = (uint8_t)(Q_PAR(me) >> 24);
            	//uint8_t value3 = (uint8_t)(Q_PAR(me) >> 16);
            	//uint8_t value2 = (uint8_t)(Q_PAR(me) >> 8);

            	//where me is the recieving object to the event.
            	//values 1 - 4 are contained in variable "send"

        	}

        	//rearm the timer
        	QActive_armX((QActive *)me, 0U, BSP_TICKS_PER_SEC/20U);
            status = Q_HANDLED();
            break;
        }

        default:
        {
            status = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status;
}
