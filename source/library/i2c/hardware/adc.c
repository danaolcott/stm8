/*
7/20/20
ADC Controller file

The purpose of this file is to configure and read
ADC1 channels 16, 7, VrefInt, and the factory 
calibrated Vref readings.  The factory calibrated
vref is compated to the read Vref to compute the equivalent
full scale reading based on the factory calibrated Vdd.

The temperature sensor on PB2 is the MCP9700 and uses 
a simple relationship from mv to temp as follows:
Temp(C) = (reading(mv) - 500) / 10
ie, a reading of 732 mv, would be equivalent to 23.2 C.

Temp sensor - ADC1, CH16, PB2
Voltage divider supplied by VrefInt, ADC1, CH7, PD7
Vref - output on PD6.

Note:
Vref-factory - factory calibrated internal ref voltage
based on input value Vdd of 3.00v.


*/

#include "main.h"
#include "adc.h"


/////////////////////////////////////////////
//Configure PB2 at ADC.  CH16, single conversion
//mode, 12 bit resolution.  Note: Channel factory
//does not initialize with anything.
//
void adc_init(ADC_Channel ch)
{
    ADC_DeInit(ADC1);
    ADC_Init(ADC1, ADC_ConversionMode_Single, ADC_Resolution_12Bit, ADC_Prescaler_1);
    ADC_SamplingTimeConfig(ADC1, ADC_Group_SlowChannels, ADC_SamplingTime_384Cycles);   //ch7 and 16
    ADC_SamplingTimeConfig(ADC1, ADC_Group_FastChannels, ADC_SamplingTime_384Cycles);   //vref and ts?
    
    ADC_Cmd(ADC1, ENABLE);
    
    //enable the specific channel
    switch(ch)
    {
        case ADC_CHANNEL_7:      ADC_ChannelCmd(ADC1, ADC_Channel_7, ENABLE);    break;
        case ADC_CHANNEL_16:     ADC_ChannelCmd(ADC1, ADC_Channel_16, ENABLE);   break;
        case ADC_CHANNEL_VREF:   ADC_ChannelCmd(ADC1, ADC_Channel_Vrefint, ENABLE);   break;
        case ADC_BATTERY:        ADC_ChannelCmd(ADC1, ADC_Channel_Vrefint, ENABLE);   break;
        case ADC_FACTORY:        break;
        default:                 ADC_ChannelCmd(ADC1, ADC_Channel_16, ENABLE);   break;
        
        
    }
    
    ADC_VrefintCmd(ENABLE);     //enable reading vref.
    

}

////////////////////////////////////////////////
//Read adc channel.  For factory cal reading, 
//return the value stored in the registers for 
//MSB and LSB factory cal.

uint16_t adc_read(ADC_Channel ch)
{
    uint16_t result = 0x00;
    unsigned long temp = 0x00;
    unsigned long factory = 0x00;
    
    
    //initialize the adc with the specific channel
    if (ch == ADC_FACTORY)
    {
        result = (((uint16_t)ADC_VREFINT_MSB) & 0xFF) << 8;
        result |= (((uint16_t)ADC_VREFINT_LSB) & 0xFF);
    }
        
    else    
    {
        adc_init(ch);   
        ADC_SoftwareStartConv(ADC1);
        while(!ADC_GetFlagStatus (ADC1,ADC_FLAG_EOC));
        ADC_ClearFlag (ADC1,ADC_FLAG_EOC);
        
        result = ADC_GetConversionValue(ADC1);
    }
    
    return result;    
}


/////////////////////////////////////////////////
//Read the channel and return result in mv
//full scale = (vref / factory) * 0xFFF
//To convert to mv, use the full scale reading and 
//VrefIntFactory_vdd as the reference.  ie, 3000mv
//
uint16_t adc_read_mv(ADC_Channel ch)
{
    unsigned long vref, factory = 0x00;
    unsigned long fullScale = 0x00;
    unsigned long reading;
    unsigned long mv = 0x00;
    
    vref = (unsigned long)adc_read(ADC_CHANNEL_VREF);
    factory = (unsigned long)adc_read(ADC_FACTORY);
    reading = (unsigned long)adc_read(ch);
    fullScale = (vref * 0xFFF) / factory; 

    mv = reading * ADC_VREFINT_VDD / fullScale;
    
    //battery channel just reads the vref, so baseed on
    //supply of 3300 baseline, variations in the vref
    //should reflect the battery voltage
    //
    if (ch == ADC_BATTERY)
    {
        //battery uses read / factory * 3300
        mv = reading * ADC_VREFINT_VDD / factory;
    }
    
    return (uint16_t)mv;
    
    
}

////////////////////////////////////////////
//Read the temp sensor on CH16 and output
//the tmp in F.  Returns value as 10ths of a deg
//ie, 70.5 deg F  = 705
int adc_readTempF(void)
{
    signed long result, temperature;
    result = (signed long)adc_read_mv(ADC_CHANNEL_16);
    temperature = ((result - 500) * 9 / 5) + 320;
    
    return (int)temperature;
    
}




///////////////////////////////////////////////
//Enable the Vref output on Pin PD6
void adc_enableVrefOutput(void)
{
    //enable the vref output on PD6 - RI_IOSR3 - CH9E
    SYSCFG_RIIOSwitchConfig(RI_IOSwitch_9,ENABLE);    
    COMP_VrefintToCOMP1Connect(ENABLE);
    COMP_VrefintOutputCmd(ENABLE);
}

//////////////////////////////////////////////
//Disable the Vref output on Pin PD6
void adc_disableVrefOutput(void)
{
    //enable the vref output on PD6 - RI_IOSR3 - CH9E
    SYSCFG_RIIOSwitchConfig(RI_IOSwitch_9,DISABLE);    
    COMP_VrefintToCOMP1Connect(DISABLE);
    COMP_VrefintOutputCmd(DISABLE);
}





