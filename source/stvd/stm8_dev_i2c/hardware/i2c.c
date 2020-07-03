/*
6/30/20
I2C Controller File
PC1 - I2C1 - CLK
PC0 - I2C1 - Data


*/

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#include "register.h"
#include "i2c.h"


///////////////////////////////////////////
//Configure I2C Master mode - i2c clocks
//have to be enabled.  Enabling the peripheral
//automatically configures the pins as needed.
//Assume 100khz, 16mhz input clock, polling mode
//7 bit addressing, master mode
void I2C_init(void)
{
    I2C1_FREQR = 0x10;              //input clock 16mhz
    I2C1_CCRL = 0x50;               //clock control reg, 100khz for 16mhz input    
    I2C1_CR1 |= I2C1_CR1_PE_BIT;    //PE enable bit - enable the i2c    
}

//////////////////////////////////////////////////
//Generate the start condition
//Also used to send a restart (ie, a write / read)
void I2C_sendStart(void)
{
    I2C1_CR2 |= I2C1_CR2_START_BIT;
    while (!I2C_isStartConditionGenerated());
}

/////////////////////////////////////////////////
//generate the stop condition after byte is transfered
//returns to slave mode after stop is detected
void I2C_sendStop(void)
{
    I2C1_CR2 |= I2C1_CR2_STOP_BIT;
    while (I2C_isMasterMode() == 1);    
}

////////////////////////////////////////////////
//Send address in write mode
void I2C_sendAddressWrite(uint8_t address)
{
    uint8_t value = 0x00;
    I2C1_DR = (address << 1);           //write the address
    while (!I2C_isAddressSent());       //wait
    value = I2C1_SR3;                   //clear flags
    I2C_AckEnable();
}

////////////////////////////////////////////////
//Send address in read mode
void I2C_sendAddressRead(uint8_t address)
{
    uint8_t value = 0x00;
    I2C1_DR = (address << 1) + 0x01;        //read bit high
    while (!I2C_isAddressSent());           //wait
    value = I2C1_SR3;                       //clear flags
    I2C_AckEnable();    
}



////////////////////////////////////////////////
//Write 1 byte
void I2C_write(uint8_t data)
{
    I2C1_DR = data;
    while(!I2C_isTXEmpty());
}


/////////////////////////////////////////////
//read byyte with ack disable / enable
//Generates the stop if the ack is disabled
uint8_t I2C_read(uint8_t ackEnable, uint8_t generateStop)
{
    if (!ackEnable)
        I2C_AckDisable();
    else
        I2C_AckEnable();
    
    if (generateStop == 1)
        I2C_sendStop();
        
    while (!I2C_isDataRegisterNotEmpty());
    
    return I2C1_DR;
}


///////////////////////////////////////////////
//write data to register
void I2C_writeReg(uint8_t reg, uint8_t data)
{
    I2C_sendStart();
    I2C_sendAddressWrite(I2C_ADDRESS);
    I2C_write(reg);
    I2C_write(data);
    I2C_sendStop();
}

////////////////////////////////////////
//Read register
//Generate a start condition for a write
//write address
//generate a start condition
//send address as a read - no ack
//read the DR
//generate the stop
//return value
uint8_t I2C_readReg(uint8_t reg)
{
    uint8_t result = 0x00;
    
    //Generate a start condition for a write
    I2C_sendStart();
    I2C_sendAddressWrite(I2C_ADDRESS);
    I2C_write(reg);
    
    //generate a start condition for a read
    I2C_sendStart();
    I2C_sendAddressRead(I2C_ADDRESS);  //single byte read, no ack    
    result = I2C_read(0, 1);            //nack and generate stop
        
    return result;
}


//////////////////////////////////////////////////
//Read length bytes into data starting at 
//address startAddress
void I2C_readArray(uint8_t startAddress, uint8_t* data, uint8_t length)
{
    uint8_t i = 0;
    uint8_t counter = 0;
    
    //Generate a start condition for a write
    I2C_sendStart();
    I2C_sendAddressWrite(I2C_ADDRESS);
    I2C_write(startAddress);
    
    I2C_sendStart();
    I2C_sendAddressRead(I2C_ADDRESS);  //single byte read, no ack

    //read with no ack and stop
    if (length == 1)
    {
        data[0] = I2C_read(0, 1);             //read with nack and generate stop
    }
    
    //read all but last with ack, last with no ack and stop
    else
    {
        for (i = 0 ; i < length - 1 ; i++)
            data[counter++] = I2C_read(1, 0);         //ack, no stop

        data[counter] = I2C_read(0, 1);
    }
}





//returns 1 if the Tx register is empty
uint8_t I2C_isTXEmpty(void)
{
    return ((I2C1_SR1 & BIT_7) >> 7);
}

//returns 1 if the byte transfer is complete
uint8_t I2C_isByteTransferComplete(void)
{
    return ((I2C1_SR1 & BIT_2) >> 2);
}

//returns 1 if the data register is not empty (rx mode)
uint8_t I2C_isDataRegisterNotEmpty(void)
{
    return ((I2C1_SR1 & BIT_6) >> 6);
}

//returns 1 indicating end of transmission of
//of the address bit, it's set when the ACK bit is cleared
uint8_t I2C_isAddressSent(void)
{
    return ((I2C1_SR1 & BIT_1) >> 1);
}

//returns 1 if the start condition is generated
uint8_t I2C_isStartConditionGenerated(void)
{
    return ((I2C1_SR1 & BIT_0) >> 0);
}

uint8_t I2C_isBusBusy(void)
{
    return ((I2C1_SR3 & BIT_1) >> 1);
}
uint8_t I2C_isMasterMode(void)
{
    return ((I2C1_SR3 & BIT_0) >> 0);
}

//ack is returned after the byte is received
void I2C_AckEnable(void)
{
    I2C1_CR2 |= BIT_2;
}

void I2C_AckDisable(void)
{
    I2C1_CR2 &=~ BIT_2;
}


