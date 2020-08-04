/*
7/5/20
ADC Controller File
Configures the ADC peripheral to read single
conversion mode, 16 adc cycles for 2 ADC channels
used to read the temperature sensor on the dev board


PB2 - AS5

VREF out - It looks like there's a hardware error in 
reading the VREF supplied voltage divider since it's
in the same group of IOs as the VREF output.  ie, the output
is in group PC2, PD7, PD6.  We are trying to read the divider
on PD7, but the Vref output is in PD6.  So if both switches
are closed, the voltage divider reading will short to the
vreoutput pin.

either way, switch AS2 looks like it needs to be closed 
to get to ADC1

also, AS5 looks like it needs to be closed to get PB2

Fix: cut trace at PD7 and keep PD7 switch open.  Close switch
at PD6 to supply VRefint to voltage divider, about 1.2v.  Run jumper
from PB7 to voltage divider read point and close switch at 
PB7 (CHXXXE) and AS3

temp sensor ~ 0.762V


*/

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#include "register.h"
#include "adc.h"


void ADC_dummyDelay(uint16_t delay)
{
    volatile uint16_t temp = delay;    
    while (temp > 0)
        temp--;
}



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



