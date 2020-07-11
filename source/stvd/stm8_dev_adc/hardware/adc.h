/*
7/5/20
ADC Controller file
*/


typedef enum {
	ADC_CH7,
	ADC_CH16,
	ADC_VREF
}ADC_Channel_t;



#ifndef __ADC_H
#define __ADC_H


void ADC_init(void);
uint16_t ADC_read (ADC_Channel_t channel);
uint8_t ADC_isConversionComplete(void);

#endif

