/*
7/26/20
I2C Controller File using the STM8 Standard Library.

PC1 - I2C1 - CLK
PC0 - I2C1 - Data

Looking at the standard lib, the main functions for 
controlling the i2c, seem to be the following:

//reset the i2c to power on state
void I2C_DeInit(I2C_TypeDef* I2Cx); - deinit the i2c

//configure the i2c
void I2C_Init(I2C_TypeDef* I2Cx, uint32_t OutputClockFrequency, uint16_t OwnAddress,
              I2C_Mode_TypeDef I2C_Mode, I2C_DutyCycle_TypeDef I2C_DutyCycle,
              I2C_Ack_TypeDef I2C_Ack, I2C_AcknowledgedAddress_TypeDef I2C_AcknowledgedAddress);


//start the i2c
void I2C_Cmd(I2C_TypeDef* I2Cx, FunctionalState NewState);


communications:

//generate start condition
void I2C_GenerateSTART(I2C_TypeDef* I2Cx, FunctionalState NewState);

//generate the stop condition
void I2C_GenerateSTOP(I2C_TypeDef* I2Cx, FunctionalState NewState);

//set / clear the ACK bit
void I2C_AcknowledgeConfig(I2C_TypeDef* I2Cx, FunctionalState NewState);

//send the target address
void I2C_Send7bitAddress(I2C_TypeDef* I2Cx, uint8_t Address, I2C_Direction_TypeDef I2C_Direction);

//read / write
void I2C_SendData(I2C_TypeDef* I2Cx, uint8_t Data);
uint8_t I2C_ReceiveData(I2C_TypeDef* I2Cx);





*/


#include "main.h"
#include "i2c.h"
#include "register.h"



///////////////////////////////////////////
//Configure I2C Master mode - i2c clocks
//have to be enabled.  Enabling the peripheral
//automatically configures the pins as needed.
//Assume 100khz, 16mhz input clock, polling mode
//7 bit addressing, master mode
void I2C_init(void)
{
    I2C_DeInit(I2C1);
    I2C_Init(I2C1, I2C_MAX_STANDARD_FREQ, 0x00, I2C_Mode_I2C, I2C_DutyCycle_2,
        I2C_Ack_Enable, I2C_AcknowledgedAddress_7bit);

    I2C_AcknowledgeConfig(I2C1, ENABLE);
    I2C_Cmd(I2C1, ENABLE);
}




///////////////////////////////////////////////
//write data to register
void I2C_writeReg(uint8_t reg, uint8_t data)
{
    //wait while busy
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
    I2C_GenerateSTART(I2C1, ENABLE);
    
    //wait until master mode started
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    
    //send 7 bit address as transmit
    I2C_Send7bitAddress(I2C1, I2C_ADDRESS, I2C_Direction_Transmitter);

    //wait
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    I2C_SendData(I2C1, reg);
    
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING));
    
    I2C_SendData(I2C1, data);
    
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING));

    I2C_GenerateSTOP(I2C1, ENABLE);
    
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
        
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
    I2C_GenerateSTART(I2C1, ENABLE);
    
    //wait until master mode started
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    
    //send 7 bit address as transmit
    I2C_Send7bitAddress(I2C1, I2C_ADDRESS, I2C_Direction_Transmitter);

    //wait
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    I2C_SendData(I2C1, reg);
    
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    
    //generate a restart
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

    //send address as receiver
    I2C_Send7bitAddress(I2C1, I2C_ADDRESS, I2C_Direction_Receiver);
    
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    
    I2C_AcknowledgeConfig(I2C1, DISABLE);
    I2C_GenerateSTOP(I2C1, ENABLE);
       
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED));
    result = I2C_ReceiveData(I2C1);
        
    return result;
}


//////////////////////////////////////////////////
//Read length bytes into data starting at 
//address startAddress
void I2C_readArray(uint8_t startAddress, uint8_t* data, uint8_t length)
{
    uint8_t counter = 0x00;
    uint8_t bytesToRead = length;
        
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
    I2C_GenerateSTART(I2C1, ENABLE);
    
    //wait until master mode started
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    
    //send 7 bit address as transmit
    I2C_Send7bitAddress(I2C1, I2C_ADDRESS, I2C_Direction_Transmitter);

    //wait
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    I2C_SendData(I2C1, startAddress);
    
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    
    //generate a restart
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

    //send address as receiver
    I2C_Send7bitAddress(I2C1, I2C_ADDRESS, I2C_Direction_Receiver);    
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    
    //read the data
    while(bytesToRead > 0)
    {        
        if (bytesToRead == 1)
        {
            I2C_AcknowledgeConfig(I2C1, DISABLE);
            I2C_GenerateSTOP(I2C1, ENABLE);
        }
        
        else
        {
            I2C_AcknowledgeConfig(I2C1, ENABLE);
        }
        
        while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED));
        data[counter] = I2C_ReceiveData(I2C1);

        bytesToRead--;
        counter++;
    }
}




