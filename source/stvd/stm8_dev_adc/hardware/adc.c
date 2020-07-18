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
    //wakeup
    //ADC_CR1 |= ADC_CR1_ADON_BIT;

   // ADC_CR1 |= BIT_2;               //continuous conversion
    
    //set the channel - ch16
    ADC_CR3 = 0x10;
    
    //enable the ADC
    ADC_CR1 |= ADC_CR1_ADON_BIT;
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
    
    
    //wake up
    ADC_CR1 |= ADC_CR1_ADON_BIT;
    ADC_CR1 |= ADC_CR1_START_BIT;       //set the start bit

    while (!ADC_isConversionComplete());    //wait
    
    //read result - MSB then LSB
    high = (ADC_DRH & 0x0F);
    low = ADC_DRL;

    result = (high << 8) | low;



    return result;
}










/*

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
    //trigger, 16 adc cycles
//    ADC_CR2 = 0x02;     //16 cycles
    ADC_CR2 = 0x07;     //384 cycles
    
    //sample times - vrefint and ts 16adc cycles, default CH7
    ADC_CR3 = 0x40;
//    ADC_CR3 |= 0x07;        //default CH7
    ADC_CR3 |= 0x10;        //default CH16 - temp sensor
    
    //Disable DMA for single conversion
    ADC_SQR1 |= BIT_7;

    //SQR# - set either ch7 or 16 - default ch7
    //ADC_SQR4 = 0x80;        //ch7
    ADC_SQR2 = 0x01;       //ch16
        
//    ADC_TRIGR1 |= BIT_4;    //enable the internal vref
//    ADC_TRIGR1 |= BIT_5;    //enable the internal ref for temp sensor
    
    //Disable the schmit triggers
    ADC_TRIGR2 = 0x01;                //disable ch16 trigger
    ADC_TRIGR4 = 0x80;                //disable ch7 trigger

    //enable the ADC
    ADC_CR1 |= ADC_CR1_ADON_BIT;
}

*/


/*

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
    
    //wake up
    ADC_CR1 |= ADC_CR1_ADON_BIT;

    //dummy delay
    ADC_dummyDelay(10000);

    ADC_SR &=~ ADC_SR_EOC_BIT;          //clear the EOC flag
    ADC_CR1 |= ADC_CR1_START_BIT;       //set the start bit

    while (!ADC_isConversionComplete());    //wait
    
    //read result - MSB then LSB
    low = ADC_DRL;
    high = (ADC_DRH & 0x0F);

    result = (high << 8) | low;



    return result;
}


*/




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



