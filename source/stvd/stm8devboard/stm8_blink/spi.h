/*
4/5/20
SPI Controller File
*/


#ifndef __SPI_H
#define __SPI_H

#include <stddef.h>
#include <stdint.h>



void SPI_init(void);
void SPI_select(void);
void SPI_deselect(void);
uint8_t SPI_tx(uint8_t data);
uint8_t SPI_writeByte(uint8_t data);
void SPI_writeArray(uint8_t *data, uint16_t length);




#endif

