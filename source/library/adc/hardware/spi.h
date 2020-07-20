/*
7/18/20
SPI Controller File
Pinout: 
PA6 - SPI1_CS
PC6 - SPI1_SCK
PA3 - SPI1_MOSI
Note: SPI1 Miso pin not needed or defined.

*/


#ifndef __SPI_H
#define __SPI_H

#include "main.h"           //typedefs

void spi_init(void);
void spi_select(void);
void spi_deselect(void);
uint8_t spi_tx(uint8_t data);
uint8_t spi_writeByte(uint8_t data);
void spi_writeArray(uint8_t *data, uint16_t length);




#endif

