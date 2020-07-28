/*
7/20/20
ADC Controller File
*/


#ifndef __ADC_H
#define __ADC_H

#include "main.h"

#define ADC_VREFINT_MSB         0x06
#define ADC_VREFINT_LSB         *((volatile uint8_t*)0x4910)
#define ADC_VREFINT_VDD         3000

typedef enum
{
    ADC_CHANNEL_7,
    ADC_CHANNEL_16,
    ADC_CHANNEL_VREF,
    ADC_BATTERY,
    ADC_FACTORY
}ADC_Channel;

void adc_init(ADC_Channel ch);
uint16_t adc_read(ADC_Channel ch);
uint16_t adc_read_mv(ADC_Channel ch);
int adc_readTempF(void);
void adc_enableVrefOutput(void);
void adc_disableVrefOutput(void);


#endif

