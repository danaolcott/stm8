/*****************************************************************************
* Product: BSP for the Simple Blinky example, MSP-EXP430G2, Vanilla kernel
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
//
/////////////////////////////////////////////////////
//bsp.c / bsp.h.
//These files are used to init all the low level hardware
//peripherals, gpio, uart, spi, etc.
//See main.c, main.h for init of all AOs
//
//Note: This started out from an example for the MSP430
//Most of the items were removed.

#include "qpn_port.h"
#include "bsp.h"
#include "register.h"
#include "system.h"


/*--------------------------------------------------------------------------*/
#define BSP_SMCLK   1000000UL


/*..........................................................................*/
void BSP_init(void) 
{
    //do something - config SPI, GPIO, etc.
}
/*..........................................................................*/


/*--------------------------------------------------------------------------*/
void QF_onStartup(void) 
{
    //enable interrupts
    system_enableInterrupts();
    
}
/*..........................................................................*/
void QF_onIdle(void) 
{
    //idle state - do something
    

#ifdef NDEBUG
    /* adjust the low-power mode to your application */
//    __low_power_mode_1(); /* Enter LPM1 */
#else
    QF_INT_ENABLE();
#endif
}
/*..........................................................................*/
void Q_onAssert(char const Q_ROM * const file, int line) {
    (void)file;       /* avoid compiler warning */
    (void)line;       /* avoid compiler warning */
    QF_INT_DISABLE();  /* make sure that interrupts are disabled */
    for (;;) {
    }
}

/*****************************************************************************
* NOTE1:
* The MSP430 interrupt processing restores the CPU status register upon
* exit from the ISR, so if any low-power mode has been set before the
* interrupt, it will be restored upon the ISR exit. This is not what
* you want in QP, because it will stop the CPU. Therefore, to prevent
* this from happening, the macro __low_power_mode_off_on_exit() clears
* any low-power mode in the *stacked* CPU status register, so that the
* low-power mode won't be restored upon the ISR exit.
*/
