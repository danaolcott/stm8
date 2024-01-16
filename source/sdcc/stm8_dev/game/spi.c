/*
4/5/20
SPI Controller File
Pinout: 
PA6 - SPI1_CS
PC6 - SPI1_SCK
PA3 - SPI1_MOSI
Note: SPI1 Miso pin not needed or defined.



*/

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#include "register.h"
#include "spi.h"


///////////////////////////////////////////
//Configure SPI
//PA6 - CS, PC6 - SCK, PA3 - MOSI.
//Master mode, MSB first, idle idle clock low
//defaulit clock is 1mhz for system clock
void SPI_init(void)
{
    //PA6 - SPI1_CS - pushpull output, initially high
    PA_ODR |= BIT_6;
    PA_DDR |= BIT_6;
    PA_CR1 |= BIT_6;
    PA_CR2 &=~ BIT_6;

    //PC6 - SPI1_SCK
    //PA3 - SPI1_MOSI
    //SPI output pins MOSI, SCK - output, push pull
    //with CR2 bit kept low
    PC_DDR |= BIT_6;        //output
    PC_CR1 |= BIT_6;
    PC_CR2 &=~ BIT_6;
    PC_ODR &=~ BIT_6;       

    PA_DDR |= BIT_3;        //output
    PA_CR1 |= BIT_3;
    PA_CR2 &=~ BIT_3;
    PA_ODR &=~ BIT_3;       

    //Configure the SPI - configure SPI - MSB, SPI enable, master mode
    //SPI1_CR2 - slave select
    //Disable slave select hardware management - set to software.
    //Note:  If you dont, the slave select will be based on the value read
    //by the NSS pin.  Also, SPI1_CR1 will get reset every
    //time you try to enable SPI as master.
    //
    SPI1_CR2 |= BIT_1;      //enable software management
    SPI1_CR2 |= BIT_0;      //master mode

    //SPI_CR1 - Section 30.4 - table 30.4.1
    //MSB, SPI enable, master mode, low polarity, leading edge
    //bit 7 - 0 - msb
    //bit 6 - 1 - spi en
    //bits 5-3 - 000 - sysclk / 2
    //bit 2 - 1 = master mode
    //bit 1 - cpol - idle low = 0
    //bit 0 - edge, leading edge = 0
    //0100 0100
    
    SPI1_CR1 |= BIT_2;      //master mode
//    SPI1_CR1 |= BIT_3;      //sysclk / 4 - comment out for sysclk/2
    SPI1_CR1 |= BIT_6;      //spi en

}

void SPI_select(void)
{
    PA_ODR &=~ BIT_6;
}

void SPI_deselect(void)
{
    PA_ODR |= BIT_6;
}

///////////////////////////////////////
//Put one byte into the data register
//and wait until tx is empty
uint8_t SPI_tx(uint8_t data)
{
    SPI1_DR = data;
    while (!(SPI1_SR & SPI1_TXE_FLAG)){};    //wait
    while (!(SPI1_SR & SPI1_RXNE_FLAG)){};    //wait        
    return SPI1_DR;
}

uint8_t SPI_writeByte(uint8_t data)
{
    uint8_t result = 0x00;
    SPI_select();
    result = SPI_tx(data);
    SPI_deselect();
    return result;
}

void SPI_writeArray(uint8_t *data, uint16_t length)
{
    uint16_t i = 0;
    uint8_t result = 0;

    SPI_select();
    for (i = 0 ; i < length ; i++)
    {
        result = SPI_tx(data[i]);
    }

    SPI_deselect();
}



