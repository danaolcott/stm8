/*
3/5/20
System configuration File.  Set up the clocks, pin
mapping, etc
*/



//////////////////////////////////////////////////
//Configure the pin maps to allow spi, etc 
//on the correct pins
//see page 125
//see page 126 
void system_init(void)
{    
    //#define SYSCFG_RMPCR1		*((volatile uint8_t*)0x509E)    
    //SPI1_remap - SPI on PC5, PC6, PA2, PA3
    SYSCFG_RMPCR1 |= BIT_7;

    //usart - PC2 and PC3, bits 5 and 4 = 00
    SYSCFG_RMPCR1 &=~ BIT_5;
    SYSCFG_RMPCR1 &=~ BIT_4;

    //#define SYSCFG_RMPCR2		*((volatile uint8_t*)0x509F)
    //dont need this one


    //route the VREFINT to output at pin ...
    //see section 11.2.7
    //set the bit VREFOUTEN in COMP_CSR3

    //close the IO switch in any IO group3 by setting
    //CH7E in RI_IOSR1....CH8E..CH9
    //PC2 - CH7E bit - ADC1_IN6
    //PD7 - CH8E bit - ADC1_IN7
    //PD6 - CH9E bit - ADC1_IN8

    //analog switch control bit AD2 bit, 
    //register RI_ASCR1




    //route dac to output to pin....


}

////////////////////////////////////////////////////
//Configure the internal clock to run at 16mhz
void system_clock_config(void)
{
    
}



