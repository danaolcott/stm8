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


////////////////////////////////////////////////////////
//Convert input buffer to equivalent array of hex values
//len = size of input buffer, returns numbytes in output buffer
//Assumes output buffer is large enough.  Each hex output in 
//form of "0x## ".
uint8_t utility_data2HexBuffer(uint8_t* input, uint8_t len, uint8_t* output)
{
    uint8_t i = 0x00;
    int n = 0x00;
    uint8_t hexValue[6] = {0x00};
    uint8_t offset = 0x00;          //output buffer offset

    for (i = 0 ; i < len ; i++)
    {
        n = sprintf(output + offset, "0x%02x ", input[i]);
        offset += n;
    }

    output[offset] = 0x00;      //null terminate

    return offset;
}






