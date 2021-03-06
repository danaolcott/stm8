/*
3/5/20
System configuration File.  Set up the clocks, pin
mapping, etc
*/


#ifndef __SYSTEM_H
#define __SYSTEM_H


void system_init(void);
void system_clock_config(void);
void system_peripheral_clock_config(void);
void system_enableInterrupts(void);
void system_disableInterrupts(void);

#endif

