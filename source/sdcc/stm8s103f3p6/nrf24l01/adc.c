/*
ADC Controller File for the STM8S103F3P6 Processor
Dana Olcott 12/22/18

Configure ADC channels 3 and 4 on PD2 and PD3

*/

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include "register.h"
#include "adc.h"


////////////////////////////////////////////
//Configure pins PD2 and PD3 as ADC
//Channels AN3 and AN4
//
void ADC_init(void)
{
    PD_DDR &=~ BIT_2;
    PD_CR1 &=~ BIT_2;
    PD_CR2 &=~ BIT_2;

    PD_DDR &=~ BIT_3;
    PD_CR1 &=~ BIT_3;
    PD_CR2 &=~ BIT_3;

    //#define ADC_CSR 		*((volatile uint8_t*)0x5400)
    ADC_CSR = 0x03;         //enable AN3, all interrupts off

    //#define ADC_CR1 		*((volatile uint8_t*)0x5401)
    ADC_CR1 = 0x70;         //single conversion, fmaster/18, ADON = 0;

    //#define ADC_CR2 		*((volatile uint8_t*)0x5402)
    ADC_CR2 = 0x08;         //right alignment, read LSB, then MSB

    //#define ADC_CR3 		*((volatile uint8_t*)0x5403)
    //no change

    //ADC_DRH - MSB, ADC_DRL - LSB

    //disable the schmidt trigger - all channels - datasheet
    ADC_TDRH = 0xFF;
    ADC_TDRL = 0xFF;


    //finally, wakeup the adc
    ADC_wakeup();

}


//////////////////////////////////
//Wakes up the ADC peripheral
//from powerdown mode
void ADC_wakeup(void)
{
    ADC_CR1 |= ADC_ADON_BIT;
}


//////////////////////////////////////
//Read raw data from ADC channel.
//Set the read channel using ADC_CSR - Bits 3:0 (AN15-AN0)
//Clear the EOC flag - ADC_CSR
//Set the ADON bit in ADC_CR1
//Poll the EOC flag - ADC_CSR until set
//Read the LSB in the ADC_DRL
//Read the MSB in the ADC_DRH
//Return the 10bit data
uint16_t ADC_readRawData(ADC_Channel_t ch)
{
    uint16_t resultLow = 0x00;
    uint16_t resultHigh = 0x00;

    //Set the read channel
    uint8_t initial = ADC_CSR & 0xF0;   //clear the bottom 4 bits
    initial |= ch;
    ADC_CSR = initial;

    ADC_CR1 |= ADC_ADON_BIT;       //wakeup

    ADC_CSR &=~ ADC_EOC_BIT;       //Clear the EOC flag - ADC_CSR
    ADC_CR1 |= ADC_ADON_BIT;       //Set the ADON bit in ADC_CR1

    //Poll the EOC flag - ADC_CSR until set
    while (!(ADC_CSR & ADC_EOC_BIT)){};

    resultLow = ADC_DRL & 0xFF;
    resultHigh = ((ADC_DRH & 0x03) << 8);

    ADC_CR1 &=~ ADC_ADON_BIT;       //Clear the ADON bit in ADC_CR1

    return (resultLow | resultHigh);
}


/////////////////////////////////////////////
//Scale raw data based on input voltage 3300
//return millivolts  - 0x3FF
uint16_t ADC_readMilliVolts(ADC_Channel_t ch)
{
    uint16_t raw = 0x00;
    uint32_t result = 0x00;
    raw = ADC_readRawData(ch);
    result = ((uint32_t)raw * 3300) / 0x3FF;

    return result;
}




