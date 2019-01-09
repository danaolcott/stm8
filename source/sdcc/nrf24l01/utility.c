/*
Utility Functions

*/


#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#include "utility.h"

/////////////////////////////////////////////
//Convert unsigned int value to an array
//of uint8_t.  Assumes output buffer is large enough
//
uint8_t utility_decimal2Buffer(uint16_t value, uint8_t* output)
{
    uint8_t temp[16] = {0x00};
    uint8_t index = 0x00;
    uint8_t i = 0x00;

    //output ascii equivalent - 0x30  = "0"
    if (value < 10)
    {    
        output[0] = 0x30 + (value);
        return 1;
    }

    //convert each digit to ascii equivalent
    while (value > 0)
    {
        temp[index++] = 0x30 + (value % 10);
        value = value / 10;
    }

    //reverse it into output buffer and null terminate
    for (i = 0 ; i < index ; i++)
        output[i] = temp[index - i - 1];

    output[i] = 0x00;

    return index;
}





