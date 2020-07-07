/*
ADC Controller File for the STM8S103F3P6 Processor
Dana Olcott 12/22/18

Configure ADC channels 3 and 4 on PD2 and PD3

*/

#ifndef __ADC_H_
#define __ADC_H_

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

typedef enum
{
    ADC_CHANNEL_3 = 0x03,
    ADC_CHANNEL_4 = 0x04
}ADC_Channel_t;


void ADC_init(void);
void ADC_wakeup(void);
uint16_t ADC_readRawData(ADC_Channel_t ch);
uint16_t ADC_readMilliVolts(ADC_Channel_t ch);





#endif

