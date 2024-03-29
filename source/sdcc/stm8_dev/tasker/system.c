/*
3/5/20
System configuration File.  Set up the clocks, pin
mapping, etc
*/

#include "register.h"


//////////////////////////////////////////////////
//Configure the pin maps to allow spi, etc 
//on the correct pins
//see page 125
//see page 126 
void system_init(void)
{    
    /////////////////////////////////////////
    //System remap for SPI, UART, etc
    //SYSCFG_RMPCR1 - remap
    //SPI on PC5, PC6, PA2, PA3
    SYSCFG_RMPCR1 |= BIT_7;

    //UART - PC2 and PC3, bits 5 and 4 = 00
    SYSCFG_RMPCR1 &=~ BIT_5;
    SYSCFG_RMPCR1 &=~ BIT_4;

    ///////////////////////////////////////////////
    //Switch control - Routing Interface
    //section 11.3 - RI registers accessed after setting 
    //comparator clock enabled, PCKEN25 bit in CLK_PCKENR2
    
    //Route VREFINT - Section 11.2.7 - output to .....
    COMP_CSR3 |= BIT_0;     //set VREFOUTEN bit 0 in COMP_CSR3

    //Route DAC output to PB4 - See Section 11.4.11
    RI_IOSR3 |= BIT_4;      //close CH15E bit
    RI_ASCR1 |= BIT_4;      //close analog AS4 bit

    //Route ADC........

}

////////////////////////////////////////////////////
//Configure the internal clock to run at 16mhz
//HSI clock source - Section 9.1
//Defaulit is the HSI RC clock at 16mhz
//divide by 8.  Update to run at full speed
//
void system_clock_config(void)
{
    CLK_DIVR = 0x00;        //set the divider to 1

}


//////////////////////////////////////////////////////
//Configure the peripheral clocks, SPI, etc.
//Section 9.7 - After reset, all peripheral
//clocks are disabled

void system_peripheral_clock_config(void)
{
    //PCKENR2 - clock enable
    CLK_PCKENR2 |= BIT_7;           //enable the perphieral clocks as a whole
    CLK_PCKENR2 |= BIT_0;           //ADC1
        
    //PCKENR1 - Bits 17-10
    CLK_PCKENR1 |= BIT_7;           //DAC
    CLK_PCKENR1 |= BIT_4;           //SPI
    CLK_PCKENR1 |= BIT_2;           //TIM4
    CLK_PCKENR1 |= BIT_0;           //TIM2
    
    //PCKENR2 - PCKEN25 bit - enable routing interface
    CLK_PCKENR2 |= BIT_5;           //RI
    
}




////////////////////////////////////////////////////
//Enable global interrupts - Section 12.7
//Configure the I1 and I0 bits to 10 in CPU Code 
//Condition registesr.
//"RIM" - enable global interrupts
//Note:  use __asm__ for sdcc, use _asm() for stvd
void system_enableInterrupts(void)
{
//    _asm("RIM");
    __asm__("RIM");
}

/////////////////////////////////////////////////////
//Disable Global interrupts - Section 12.7
//Configure the I1 and I0 bits to 11 in CPU Code
//Condition register.
//"SIM" - disable global interrupts
void system_disableInterrupts(void)
{
    __asm__("SIM");
}


