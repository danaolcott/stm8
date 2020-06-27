/*****************************************************************************
* Product: Simple Blinky example, Vanilla kernel
* Last updated for version 5.3.0
* Last updated on  2014-04-14
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
#ifndef qpn_port_h
#define qpn_port_h


//system.h contains the functions for enable and disable interrupts
#include "system.h"


#define Q_NMSM
#define Q_NFSM

//this setting describes how your events will get posted.  If not
//defined, the default is zero.
//
//Size of single event parameter, 0, 1, 2, or 4 bytes.
//used for passing data
#define Q_PARAM_SIZE            4		//sizenumber of params for all events - 2 bytes
#define QF_TIMEEVT_CTR_SIZE     2		//0, 1, 2, 4; 0 = no time events

/* maximum # active objects--must match EXACTLY the QF_active[] definition */
#define QF_MAX_ACTIVE           2		//blinky and the button

/* interrupt disabling policy for task level */
//Define your functions for enable and disable interrupts
//
#define QF_INT_DISABLE()        system_disableInterrupts()
#define QF_INT_ENABLE()         system_enableInterrupts()

/* interrupt disabling policy for interrupt level */
/* #define QF_ISR_NEST */ /* nesting of ISRs not allowed */

//#include <intrinsics.h> /* contains prototypes for the intrinsic functions */
#include <stdint.h>     /* Exact-width types. WG14/N843 C99 Standard */
#include <stdbool.h>    /* Boolean type.      WG14/N843 C99 Standard */

#include "qepn.h"       /* QEP-nano platform-independent public interface */
#include "qfn.h"        /* QF-nano platform-independent public interface */
#include "qassert.h"    /* QP-nano assertions header file */

#endif /* qpn_port_h */
