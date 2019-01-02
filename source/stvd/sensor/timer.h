/*
Timer Head File

*/

#ifndef __TIMER_H
#define __TIMER_H

void delay_ms(uint32_t delay);
void Timer4_init(void);
void Timer4_start(void);
void Timer4_stop(void);
void Timer4_ISR(void);


void Timer2_init(void);
void Timer2_start(void);
void Timer2_stop(void);
void Timer2_ISR(void);





#endif


