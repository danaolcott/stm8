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
    //SYSCFG_RMPCR1 - remap
    //SPI on PC5, PC6, PA2, PA3
    SYSCFG_RMPCR1 |= BIT_7;

    //UART - PC2 and PC3, bits 5 and 4 = 00
    SYSCFG_RMPCR1 &=~ BIT_5;
    SYSCFG_RMPCR1 &=~ BIT_4;

    //VREFINT - Section 11.2.7
    COMP_CSR3 |= BIT_0;     //set VREFOUTEN bit 0 in COMP_CSR3
    
    //Close the IO switch group3 - PD6 - Section 11.4.11
    //CH9E bit in RI_IOSR3, ADC1_IN8  - bit 2
    RI_IOSR3 |= BIT_2;
    
    //analog switch control bit AD2 bit, 
    //register RI_ASCR1 - not sure about this one

    //DAC output - PB4 route dac to output to pin....
    //Table 24 - Group 5
    //CH15E bit, ADC1_IN14.  AS4 bit
    RI_IOSR3 |= BIT_4;      //Section 11.4.11



}

////////////////////////////////////////////////////
//Configure the internal clock to run at 16mhz
//HSI clock source - Section 9.1
//Defaulit is the HSI RC clock at 16mhz
//divide by 8.
void system_clock_config(void)
{

}


//////////////////////////////////////////////////////
//Configure the peripheral clocks, SPI, etc.
//Section 9.7 - After reset, all peripheral
//clocks are disabled
//#define CLK_PCKENR1		    *((volatile uint8_t*)0x50C3)    //peripheral gating
//#define CLK_PCKENR2		    *((volatile uint8_t*)0x50C4)    //peripheral gating

void system_peripheral_clock_config(void)
{
    //PCKENR2 - clock enable
    CLK_PCKENR2 |= BIT_7;           //enable the perphieral clocks as a whole
    CLK_PCKENR2 |= BIT_0;           //ADC1
        
    //PCKENR1 - Bits 17-10
    CLK_PCKENR1 |= BIT_7;           //DAC
    CLK_PCKENR1 |= BIT_4;           //SPI
    
}




////////////////////////////////////////////////////
//Enable global interrupts - Section 12.7
//Configure the I1 and I0 bits to 10 in CPU Code 
//Condition registesr.
//"RIM" - enable global interrupts
void system_enableInterrupts(void)
{
    _asm("RIM");
}

/////////////////////////////////////////////////////
//Disable Global interrupts - Section 12.7
//Configure the I1 and I0 bits to 11 in CPU Code
//Condition register.
//"SIM" - disable global interrupts
void system_disableInterrupts(void)
{
    _asm("SIM");
}


