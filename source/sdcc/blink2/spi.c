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
#include "registers.h"


//////////////////////////////////
//Configure PC4 - PC7 as SPI
//Configure the SPI interface
//idle clock low, data on a leading edge
//MSB first
void SPI_init(void)
{
	//Configure PC5 - PC7 as SPI
	//Configure PC4 - as output
	PC_DDR |= BIT_4;		//output
	PC_CR1 |= BIT_4;
	PC_CR2 |= BIT_4;
	PC_ODR |= BIT_4;		//CS high

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
	//SPI_CR1 = 0 1 111 1 0 0
	SPI_CR1 = 0x7C;

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
	PC_ODR &=~ BIT_4;
}

//////////////////////////////////////
//CS High
void SPI_deselect(void)
{
	PC_ODR |= BIT_4;
}


/////////////////////////////////////
//Put byte into the SPI_DR and wait
void SPI_tx(uint8_t data)
{
	SPI_DR = data
	while (!(SPI_SR & SPI_TXE_FLAG)){};		//wait
}


//////////////////////////////////////////
//Send dummy and wait for rx complete
//return the contents of the data register
uint8_t SPI_rx(void)
{
	//send 0xFF
	SPI_DR = 0xFF;
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
		SPI_rx(rxBuffer[i]);
	}

	SPI_deselect();	
}





