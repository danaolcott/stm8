/*
Main.h

Header file for use with routines accessed in main.c
but available to the interrupt files


*/

#ifndef __MAIN_H
#define __MAIN_H

#include "stm8l15x.h"


void interrupt_enable(void);
void interrupt_disable(void);



#endif