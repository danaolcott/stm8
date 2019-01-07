/*
Dana Olcott 12/14/18
SPI Controller File

Pinout:
PC4 - SPI_CS
PC5 - SPI_SCK
PC6 - SPI_MOSI
PC7 - SPI_MISO



*/

#include <stddef.h>
#include <stdint.h>
#include "spi.h"
#include "register.h"


//////////////////////////////////
//Configure PC5 - PC7 as SPI
//PC3 as CS pin.
//Configure the SPI interface
//idle clock low, data on a leading edge
//MSB first
void SPI_init(void)
{
	//Configure PC5 - PC7 as SPI

	//Configure PC3 as output - CS PIN
	PC_DDR |= BIT_3;		//output
	PC_CR1 |= BIT_3;
	PC_CR2 |= BIT_3;
	PC_ODR |= BIT_3;		//CS high

	//SPI output pins MOSI, SCK - output, push pull, fast
	PC_DDR |= BIT_5;		//output
	PC_CR1 |= BIT_5;
	PC_CR2 |= BIT_5;
	PC_ODR &=~ BIT_5;		

	PC_DDR |= BIT_6;		//output
	PC_CR1 |= BIT_6;
	PC_CR2 |= BIT_6;
	PC_ODR &=~ BIT_6;		

	//SPI input - MISO - input, pullup, no interrupt
	PC_DDR &=~ BIT_7;		//input
	PC_CR1 |= BIT_7;		//pullup
	PC_CR2 &=~ BIT_7;		//no interrupt

	//SPI_CR1 - 20.4.1, Page 282
	//MSB, SPI enable, 256 prescale, master mode,
	//low polarity, leading edge 

    //prescale - 256 - SPI 7.7khz
	//SPI_CR1 = 0 1 111 1 0 0
	//SPI_CR1 = 0x7C;
    
    //prescale - 32
    //SPI_CR1 = 0 1 100 1 0 0
    //SPI_CR1 = 0x64;
    
    //prescale - 16
    //SPI_CR1 = 0 1 011 1 0 0
    SPI_CR1 = 0x5C;
    
    //prescale - 8
	//SPI_CR1 = 0 1 010 1 0 0
//	SPI_CR1 = 0x54;

    //prescale - 4
	//SPI_CR1 = 0 1 001 1 0 0
	//SPI_CR1 = 0x4C;


	//SPI_CR2 - no need to change anything, default 0x00
	//2 line, na, no crc, send tx, reserved, full duplex, 

	//SPI_ICR - interrupt control reg - no need to change anything

	//remaining is SPI_SR - TXE, RXNE, BSY flags
	//and SPI_DR

}


//////////////////////////////////////
//CS Low
void SPI_select(void)
{
	PC_ODR &=~ BIT_3;
}

//////////////////////////////////////
//CS High
void SPI_deselect(void)
{
	PC_ODR |= BIT_3;
}


/////////////////////////////////////
//Put byte into the SPI_DR and wait
uint8_t SPI_tx(uint8_t data)
{
	SPI_DR = data;
    while (SPI_SR & SPI_BSY_FLAG){};        //wait
    while (!(SPI_SR & SPI_RXNE_FLAG)){};	//wait
    
    return SPI_DR;
}


//////////////////////////////////////////
//Send dummy and wait for rx complete
//return the contents of the data register
uint8_t SPI_rx(void)
{
	//send 0xFF
	SPI_DR = 0xFF;
    while (SPI_SR & SPI_BSY_FLAG){};        //wait
	while (!(SPI_SR & SPI_TXE_FLAG)){};		//wait
	while (!(SPI_SR & SPI_RXNE_FLAG)){};	//wait
	return SPI_DR;
}

/////////////////////////////////////////////
//Send one byte
void SPI_SendByte(uint8_t data)
{
	SPI_select();
	SPI_tx(data);
	SPI_deselect();	
}


//////////////////////////////////////////////////
//Send Array
void SPI_SendMultiByte(uint8_t* buffer, uint16_t length)
{
	uint16_t i;
	SPI_select();
	for (i = 0 ; i < length ; i++)
	{
		SPI_tx(buffer[i]);
	}
	SPI_deselect();	
}


////////////////////////////////////////////
//Send txLength from txBuffer, followed by
//reading rxLength bytes into rxBuffer
void SPI_Send_Receive(uint8_t* txBuffer, uint16_t txLength, uint8_t* rxBuffer, uint16_t rxLength)
{
	uint16_t i;
	SPI_select();

	//write
	for (i = 0 ; i < txLength ; i++)
	{
		SPI_tx(txBuffer[i]);
	}

	//read
	for (i = 0 ; i < rxLength ; i++)
	{
		rxBuffer[i] = SPI_rx();
	}

	SPI_deselect();	
}





