/*
7/20/20
DAC Controller file
Initialize dac to output on PB4
Requires enable comparator clock to allow routing
the output to PB4

*/

#include "main.h"
#include "dac.h"




////////////////////////////////////////////
//Configure dac to output as 12 bit
void dac_init(void)
{
    DAC_DeInit();
    DAC_Init(DAC_Channel_1, DAC_Trigger_None, DAC_OutputBuffer_Disable);
    DAC_Cmd(DAC_Channel_1, ENABLE);
    DAC_SetChannel1Data(DAC_Align_12b_R,0x00);
}

//////////////////////////////////////////////
//Write value to dac output
void dac_write(uint16_t value)
{
    DAC_SetChannel1Data(DAC_Align_12b_R,(value & 0xFFF));
}


