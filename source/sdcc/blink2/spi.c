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
/*
#define SPI_CR1         *((volatile uint8_t*)0x5200)
#define SPI_CR2         *((volatile uint8_t*)0x5201)
#define SPI_ICR         *((volatile uint8_t*)0x5202)
#define SPI_SR          *((volatile uint8_t*)0x5203)
#define SPI_DR          *((volatile uint8_t*)0x5204)
#define SPI_CRCPR       *((volatile uint8_t*)0x5205)
#define SPI_RXCRCR      *((volatile uint8_t*)0x5206)
#define SPI_TXCRCR      *((volatile uint8_t*)0x5207)
*/
void SPI_init(void)
{

}


void SPI_select(void)
{
	PC_ODR &=~ BIT_4;
}

void SPI_deselect(void)
{
	PC_ODR |= BIT_4;
}


void SPI_tx(uint8_t data)
{
}


uint8_t SPI_rx(void)
{
}









