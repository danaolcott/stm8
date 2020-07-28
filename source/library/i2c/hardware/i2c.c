/*
7/26/20
I2C Controller File using the STM8 Standard Library.
Functions for write and read to a register.  The test
device for this file was  the BMP280 pressure and
temperature sensor.

PC1 - I2C1 - CLK
PC0 - I2C1 - Data

Note:  There is no need to remap any pin or 
configure the GPIO pins as open drain.  Enabling 
the I2C does this automatically.  The peripheral 
clock needs to be enabled for this to work.

The I2C address also needs to be the upshifted
value that is passed into the write/read address functions.

*/


#include "main.h"
#include "i2c.h"


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




////////////////////////////////////////////////////
//Write data to register
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

//////////////////////////////////////////////////////////
//Read data from a register
//Note: The order of the check event / read
//data register is important.  If reversed, the
//scope shows the data should be accurate, but the
//return value is not accurate.  Polling the event appears
//to start the read process.
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
    
    //generate a restart as a read
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

    //send address as receiver
    I2C_Send7bitAddress(I2C1, I2C_ADDRESS, I2C_Direction_Receiver);    
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    
    I2C_AcknowledgeConfig(I2C1, DISABLE);
    I2C_GenerateSTOP(I2C1, ENABLE);

    //start the read process
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED));
    result = I2C_ReceiveData(I2C1);
        
    return result;
}


////////////////////////////////////////////////////
//Read length bytes into data starting at 
//address startAddress.  Disable the ack and generate
//the stop condition on the last byte.
//
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




