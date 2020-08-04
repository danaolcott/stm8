/*
7/5/20
ADC Controller file
*/


typedef enum {
	ADC_CH7,
	ADC_CH16,
	ADC_VREF,
    ADC_FACTORY
}ADC_Channel_t;



#ifndef __ADC_H
#define __ADC_H

//internal ref voltage registers
#define ADC_VREFINT_MSB         0x06
#define ADC_VREFINT_LSB         *((volatile uint8_t*)0x4910)

void ADC_dummyDelay(uint16_t delay);

void ADC_init(void);
uint16_t ADC_read(ADC_Channel_t channel);
uint16_t ADC_read_mv(ADC_Channel_t channel);

uint16_t ADC_readFactoryVref(void);
uint8_t ADC_isConversionComplete(void);

#endif

