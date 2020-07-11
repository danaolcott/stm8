/*
7/5/20
ADC Controller File
Configures the ADC peripheral to read single
conversion mode, 16 adc cycles for 2 ADC channels
used to read the temperature sensor on the dev board

*/

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#include "register.h"
#include "adc.h"


////////////////////////////////////////////
//ADC_init()
//ADC1_IN16_Temp - PB2 - Temp sensor MCP9700AT
//ADC1_IN7_Temp - PD7 - Voltage divider, R15 10k and 
//Use default CH16 - temp sensor
//
//Configure for CH7, 16, and VRef INT
void ADC_init(void)
{
    //no interrupt, 12bit resolution, disable peripheral
    ADC_CR1 &=~ ADC_CR1_ADON_BIT;

    //sample times - normal channels, fadc = ck, software
    //triffer, 16 adc cycles
    ADC_CR2 = 0x02;
    
    //sample times - vrefint and ts 16adc cycles, default CH7
    ADC_CR3 = 0x40;
    ADC_CR3 |= 0x07;        //default CH7
    
    //Disable DMA for single conversion
    ADC_SQR1 |= BIT_7;

    //SQR# - set either ch7 or 16 - default ch7
    ADC_SQR4 = 0x80;        //ch7
        
    ADC_TRIGR1 |= BIT_4;    //enable the internal vref
    ADC_TRIGR1 |= BIT_5;    //enable the internal ref for temp sensor
    
    //Disable the schmit triggers
    ADC_TRIGR2 = 0x01;                //disable ch16 trigger
    ADC_TRIGR4 = 0x80;                //disable ch7 trigger

    //enable the ADC
    ADC_CR1 |= ADC_CR1_ADON_BIT;
}


///////////////////////////////////////////////////
uint16_t ADC_read (ADC_Channel_t channel)
{

    uint16_t result = 0x00;
    uint16_t low = 0x00;
    uint16_t high = 0x00;
    uint8_t addValue = 0x00;
    uint8_t regValue = 0x00;
    
    //set the sample time for vref and ts
    ADC_CR3 = 0x40;
    
    //set the channel and SQR# registers - only 1 at a time
    switch(channel)
    {
        case ADC_CH7:
        {
            addValue = 0x07;
            ADC_SQR2 = 0x00;       //ch16
            ADC_SQR4 = 0x80;       //ch7
            break;
        }
        
        case ADC_CH16:
        {
            addValue = 0x10;
            ADC_SQR2 = 0x01;       //ch16
            ADC_SQR4 = 0x00;       //ch7
            break;
        }
        
        //work in progress TODO: fix this.
        case ADC_VREF:
        {
            addValue = 0x1C;
            break;
        }
        
        default: addValue = 0x07;           break;    
    }
    
    ADC_CR3 |= addValue;                //set the channel
    ADC_SR &=~ ADC_SR_EOC_BIT;          //clear the EOC flag
    ADC_CR1 |= ADC_CR1_START_BIT;       //set the start bit

    while (!ADC_isConversionComplete());    //wait
    
    //read result - MSB then LSB
    high = (ADC_DRH & 0x0F);
    low = ADC_DRL;
    result = (high << 8) | low;

    return result;
}



//////////////////////////////////////////////
//Returns 1 if the conversion is complete
uint8_t ADC_isConversionComplete(void)
{
    return (ADC_SR & BIT_0);
}



