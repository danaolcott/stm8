/*
SPI Header File for use with STM8S103 processor
12/12/18
Dana Olcott

*/

#ifndef __SPI__H
#define __SPI__H

#include <stddef.h>
#include <stdint.h>

void SPI_init(void);
void SPI_select(void);
void SPI_deselect(void);
void SPI_tx(uint8_t data);
uint8_t SPI_rx(void);
void SPI_SendByte(uint8_t data);
void SPI_SendMultiByte(uint8_t* buffer, uint16_t length);
void SPI_Send_Receive(uint8_t* txBuffer, uint16_t txLength, uint8_t* rxBuffer, uint16_t rxLength);

#endif
