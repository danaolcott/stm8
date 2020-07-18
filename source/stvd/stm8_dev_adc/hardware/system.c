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
    //VrefInt is about 1.22V as shown in Figure 21.
    //Section 11.3.5 - route output to PD6 - Figure 43.
    //configure PD6 as output - see gpio.c configure
//    COMP_CSR3 |= BIT_0;     //set VREFOUTEN bit 0 in COMP_CSR3
//    RI_IOSR3 |= BIT_2;      //CH9E in RI_IOSR3 - Bit 2
    //RI_ASCR1 |= BIT_2;      //AS2 switch to read and output vref

    //Route DAC output to PB4 - See Section 11.4.11
    RI_IOSR3 |= BIT_4;      //close CH15E bit
    RI_ASCR1 |= BIT_4;      //close analog AS4 bit

    //Route ADC........ - Do nothing for GPIO used as 
    //ADC inputs.  The routing is controlled by the ADC
    //close switches as2 and as5
    //
    
    //I think this is the hardware error - vref int output
    //is shorted to the voltage divider read, showing the 
    //divider read = vrefint output
   // RI_ASCR1 |= BIT_2;      //AS2 switch to read and output vref
   // RI_IOSR2 |= BIT_2;      //CH8E to close the switch to read
    
    
    //temp sensor on PB2 - should be ok to close these since
    //PB1 and PB3 are not populated
    RI_ASCR1 |= BIT_5;      //AS5 switch - PB2 - need to close PB2 switch at CHxE...CH17E bit
    RI_IOSR2 |= BIT_5;      //CH17E bit to close PB2
    RI_IOCMR2 |= BIT_5;     //mode control - GPIO settings no effect.
    
   
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
    CLK_PCKENR1 |= BIT_2;           //TIM4
    CLK_PCKENR1 |= BIT_0;           //TIM2    
    CLK_PCKENR1 |= BIT_3;           //I2C1
    
    //PCKENR2 - PCKEN25 bit - enable routing interface
    CLK_PCKENR2 |= BIT_5;           //RI
    
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


