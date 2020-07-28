/*
6/30/20
I2C Controller File
Configure I2C
*/


#ifndef __I2C_H
#define __I2C_H

#include "main.h"


#define I2C_ADDRESS			(0x76 << 1)

void I2C_init(void);

void I2C_writeReg(uint8_t reg, uint8_t data);
uint8_t I2C_readReg(uint8_t reg);
void I2C_readArray(uint8_t startAddress, uint8_t* data, uint8_t length);

#endif

