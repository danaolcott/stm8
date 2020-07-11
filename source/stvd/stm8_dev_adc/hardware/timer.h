/*
4/10/20
Timer Controller File
*/


#ifndef __TIMER_H
#define __TIMER_H


void timer_delay_ms(unsigned long delay);

void TIM2_init(void);
void TIM4_init(void);

void TIM2_start(void);
void TIM2_stop(void);

void TIM2_ISR(void);
void TIM4_ISR(void);


#endif

