/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 */
///////////////////////////////////////////////////
/*
STM8_QPN
The purpose of this project is to try to implement
a simple blinky state machine using QPN

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

#include "qpn_port.h"
#include "main.h"
#include "bsp.h"


//////////////////////////////////////////////////
//Event Queues
//Define the event queue size for each AO
static QEvt l_blinkyQSto[5];
static QEvt l_buttonQSto[5];

//////////////////////////////////
//Control Blocks for each AO in the system
//arranged in order of priority

/* QF_active[] array defines all active object control blocks --------------*/
//
//these are the relative priorities for various AOs in the
//system.  We only have one AO, so it's ok to put it here, but
//if you had more that one, you might want to make a diff
//file for this.  AO's are listed in the control block
//based on thier relative priorities.  The zero index
//is zero priority and reserved.  the highest index is the
//highest priority
QActiveCB const Q_ROM QF_active[] = {
    { (QActive *)0,           		(QEvt *)0,        	0U                      },	//this has to be zero for each entry
    { (QActive *)&AO_Button,   		l_buttonQSto,     	Q_DIM(l_buttonQSto)     },	//button - low
    { (QActive *)&AO_Blinky,   		l_blinkyQSto,     	Q_DIM(l_blinkyQSto)     }	//blinky - high
};

/* make sure that the QF_active[] array matches QF_MAX_ACTIVE in qpn_port.h */
Q_ASSERT_COMPILE(QF_MAX_ACTIVE == Q_DIM(QF_active) - 1);


//////////////////////////////////////
//Main program
main()
{
    //init the peripherals - this could be in BSP_init()
    system_disableInterrupts();
    system_clock_config();
    system_peripheral_clock_config();
    system_init();
    
    GPIO_init();
    SPI_init();
    lcd_init();
    TIM2_init();
    TIM4_init();
    DAC_init();
    Sound_init();
    EEPROM_init();

    //set initial display state
    lcd_clear(0x00);
    lcd_clearBackground(0xAA);    
    lcd_clearFrameBuffer(0x00, 0);
    lcd_drawString(3, 16, "test");
    

    BSP_init();
        
    system_enableInterrupts();

    Blinky_ctor();
    Button_ctor();
    
    return QF_run();
}












