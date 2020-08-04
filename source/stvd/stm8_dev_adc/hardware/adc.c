/*
7/5/20
ADC Controller File
Configures the ADC peripheral to read single
conversion mode, ch7, 16, vref, and factory vref.  Also
reads the temperature sensor MCP9700 through ch16.  The 
millivolt reading is scaled based on the ratio of the the
factory calibrated vref to the measured vref.  Note that the
factory calibrated vref used a supply voltage of 3.00v +/- 10mv.



*/

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#include "register.h"
#include "adc.h"



////////////////////////////////////////////
//ADC_init()
//ADC1_IN16_Temp - PB2 - Temp sensor MCP9700AT
//
void ADC_init(void)
{
   
    //CR2 - set the prescale bits as needed
    //no need - assume prescale = 1
    
    ADC_CR2 |= 0x07;        //set slow channels to 384 clock cycles
    ADC_CR3 |= 0xE0;        //set fast channels to 384 clock cycles
    
    //enable the AD
    ADC_CR1 |= ADC_CR1_ADON_BIT;
        
    //enable the channel - ch16 - SQR register
    ADC_SQR2 |= BIT_0;      //channel 16
    
    //enable  the vref in 
    ADC_TRIGR1 |= BIT_4;    //enable the vref
    
}





//////////////////////////////////////////////
//read ch16
uint16_t ADC_read (ADC_Channel_t channel)
{
    uint16_t result = 0x00;
    uint16_t low = 0x00;
    uint16_t high = 0x00;
    uint8_t addValue = 0x00;
    uint8_t regValue = 0x00;
    
    //clear the sequence control registers
    ADC_SQR1 = 0x00;
    ADC_SQR2 = 0x00;
    ADC_SQR3 = 0x00;
    ADC_SQR4 = 0x00;
    
    //set the sequence control register based onthe channel
    switch(channel)
    {
        case ADC_CH7:   ADC_SQR4 |= BIT_7;    break;
        case ADC_CH16:  ADC_SQR2 |= BIT_0;    break;
        case ADC_VREF:  ADC_SQR1 |= BIT_4;    break;
        case ADC_FACTORY:                     break;    //do nothing
    }
    
    if (channel == ADC_FACTORY)
    {
        //get the reading from the registers
        high = ((uint16_t)(ADC_VREFINT_MSB)) << 8;
        low = (uint16_t)ADC_VREFINT_LSB;
        result = high | low;        
    }
    
    else
    {
        ADC_CR1 |= ADC_CR1_START_BIT;       //set the start bit

        //returns the EOC value BIT 0 - set when complete.
        while (!ADC_isConversionComplete());    //wait
                    
        ADC_SR &=~ BIT_0;               //clear the EOC bit
        
        //read result - MSB then LSB
        high = ((uint16_t)(ADC_DRH)) << 8;
        low = (uint16_t)ADC_DRL;
    
        result = high | low;
    }
    
    return result;
}




/////////////////////////////////////////////////
//Reads the channel and returns value in mv.
//ie, for 1.450v, the return value is 1450
//For reference, it reads the vref, factory
//scales the full scale reading based on the 
//ratio of vref to factory.  This is based on the
//factory ref = 3.0v according to the datasheet
//The scaled full scale reading is then used on 
//the target channel to compute the read value in mv.
uint16_t ADC_read_mv(ADC_Channel_t channel)
{
    uint32_t factory = 0x00;
    uint32_t vref = 0x00;
    uint32_t adcReading = 0x00;
    uint32_t fullScale = 0x00;
    uint32_t mv = 0x00;
    
    factory = ADC_readFactoryVref();
    vref = ADC_read(ADC_VREF);
    adcReading = ADC_read(channel);
    
    fullScale = vref * 0xFFF / factory;     //ratio that applies to 3.0v

    mv = adcReading * 3000 / fullScale;
    
    return (uint16_t)mv;
    
}




///////////////////////////////////////////////
//Reads the temperature on the MCP9700 through
//adc channel 16.  Returns value in 10ths of a 
//degree F.  ie, 24.6 F returns 246
int ADC_readTemperatureF(void)
{
    int32_t result = 0x00;
    int32_t temperature = 0x00;
    result = (int32_t)ADC_read_mv(ADC_CH16);
    temperature = ((result - 500) * 9 / 5) + 320;
    
    return (int)temperature;
}


/////////////////////////////////////////////////////
//Returns the contents stored in the Vref
//register set at factory and MSB = 0x06
uint16_t ADC_readFactoryVref(void)
{
    uint16_t high, low, result = 0;
    
    high = ADC_VREFINT_MSB & 0xFF;
    low = ADC_VREFINT_LSB & 0xFF;
    result = (high << 8) | low;
    return result;
}




//////////////////////////////////////////////
//Returns 1 if the conversion is complete
uint8_t ADC_isConversionComplete(void)
{
    return (ADC_SR & BIT_0);
}



