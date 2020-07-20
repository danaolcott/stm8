/*
7/20/20
ADC Controller file

Temp sensor - ADC1, CH16, PB2


*/

#include "main.h"
#include "adc.h"


/////////////////////////////////////////////
//Configure PB2 at ADC.  CH16, single conversion
//mode, 12 bit resolution
void adc_init(void)
{
    ADC_DeInit(ADC1);
    ADC_Init(ADC1, ADC_ConversionMode_Single, ADC_Resolution_12Bit, ADC_Prescaler_1);
    ADC_SamplingTimeConfig(ADC1, ADC_Group_SlowChannels, ADC_SamplingTime_384Cycles);
    
    ADC_Cmd(ADC1, ENABLE);
    ADC_ChannelCmd(ADC1, ADC_Channel_16, ENABLE);
}

////////////////////////////////////////////////
//Read CH16 and return the result
uint16_t adc_read(void)
{
    uint16_t result = 0x00;
    
    ADC_SoftwareStartConv(ADC1);
    while(!ADC_GetFlagStatus (ADC1,ADC_FLAG_EOC));
    ADC_ClearFlag (ADC1,ADC_FLAG_EOC);
 
    result = ADC_GetConversionValue(ADC1);
        
    return result;
    
}


