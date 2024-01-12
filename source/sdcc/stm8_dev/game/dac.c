/*
4/20/20
DAC Controller File
Output DAC on PB4
For use with the sound output
*/

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#include "register.h"
#include "dac.h"



/////////////////////////////////////////
//Configure the dac to output, assumes the 
//peripheral clock enabled and the switch
//routes to the appropriate pin.  system.h
//Leave GPIO pin config alone.  If you set to 
//output, it wont' work.
//Also, have to enable the COMP1 and COMP2
//clock before calling this function to allow RI to work.
void DAC_init(void)
{    
    //DAC_CR1 Configure
    DAC_CR1 |= DAC_BIT_CR1_TSEL0;       //software trigger
    DAC_CR1 |= DAC_BIT_CR1_TSEL1;
    DAC_CR1 |= DAC_BIT_CR1_TSEL2;
    
    DAC_CR1 |= DAC_BIT_CR1_TEN;         //enable trigger
    DAC_CR1 &=~ DAC_BIT_CR1_BOFF;       //disable output buffer
    DAC_CR1 |= DAC_BIT_CR1_EN;          //enable the dac output
}

/////////////////////////////////////////
//Dac Output - 12 bit value outptu to PB4
//Write as left of right aligned, does not matter
void DAC_set(unsigned int value)
{
    //Write data as left aligned
    DAC_LDHRH = ((value >> 4) & 0xFF);
    DAC_LDHRL = ((value << 4) & 0xF0);

    //odd - this bit does not go high....
    //set the software trigger bit, cleared by
    //hardware once the data is written
    DAC_SWTRIGR |= BIT_0;

    //wait, BIT_0 is cleared by hardware
    while (DAC_SWTRIGR & BIT_0){};
    
}


////////////////////////////////////////////
//Set the dac output using the dac 8 bit register
void DAC_set8Bit(unsigned char value)
{
    //Write data - assuming it's 8 bit
    DAC_DHR8 = value;

    //odd - this bit does not go high....
    //set the software trigger bit, cleared by
    //hardware once the data is written
    DAC_SWTRIGR |= BIT_0;

    //wait, BIT_0 is cleared by hardware
    while (DAC_SWTRIGR & BIT_0){};
}




