/*
6/30/20
I2C Controller File
Configure I2C
*/


#ifndef __I2C_H
#define __I2C_H

#include <stddef.h>
#include <stdint.h>

#define I2C_ADDRESS			0x76

void I2C_init(void);
void I2C_sendStart(void);
void I2C_sendStop(void);
void I2C_sendAddressWrite(uint8_t address);
void I2C_sendAddressRead(uint8_t address);

void I2C_write(uint8_t data);
uint8_t I2C_read(uint8_t ackEnable, uint8_t generateStop);

void I2C_writeReg(uint8_t reg, uint8_t data);
uint8_t I2C_readReg(uint8_t reg);
void I2C_readArray(uint8_t startAddress, uint8_t* data, uint8_t length);


uint8_t I2C_isTXEmpty(void);
uint8_t I2C_isByteTransferComplete(void);
uint8_t I2C_isDataRegisterNotEmpty(void);
uint8_t I2C_isAddressSent(void);
uint8_t I2C_isStartConditionGenerated(void);
uint8_t I2C_isBusBusy(void);
uint8_t I2C_isMasterMode(void);
void I2C_AckEnable(void);
void I2C_AckDisable(void);


#endif

