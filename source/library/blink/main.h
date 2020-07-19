/*
Main.h

Header file for use with routines accessed in main.c
but available to the interrupt files


*/

#ifndef __MAIN_H
#define __MAIN_H

void tim2_isr(void);
void exti1_isr(void);
void exti5_isr(void);


#endif