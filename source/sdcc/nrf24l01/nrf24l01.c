/*
NRF24L01 Controller File
12/25/18
Dana Olcott

SPI interface - idle clock low, leading edge, MSB first

PA1 - PA2 - Control lines for NRF24L01
PA1 - output - CE pin
PA2 - input / interrupt / falling edge trigger

NOTES:
Assume all data pipe addresses are 3 byte - use LSB only




*/

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "nrf24l01.h"
#include "register.h"
#include "spi.h"
#include "uart.h"           //retransmitting out serial port


///////////////////////////////////////////////
//NRF24 Global Variables
static NRF24_Mode_t mNRF24_Mode = NRF24_MODE_POWER_DOWN;
static volatile uint8_t mTransmitCompleteFlag = 0;


//////////////////////////////////////////////
//Dummy delay
void nrf24_dummyDelay(uint32_t delay)
{
    volatile uint32_t temp = delay;
    while (temp > 0)
        temp--;
}


////////////////////////////////////////////////
//Write data to register.
//Combine write reg command with 5 bit reg value.
//Send as write followed by 1 data byte
//
void nrf24_writeReg(uint8_t reg, uint8_t data)
{
    uint8_t i = 0x00;
    uint8_t regValue = NRF24_CMD_W_REGISTER | (reg & (0x1F));

    SPI_select();                   //CS low
    SPI_tx(regValue);               //Set write reg
    SPI_tx(data);                   //send the data
    SPI_deselect();                 //CS high
}



////////////////////////////////////////////////
//Write data to register.
//Combine write reg command with 5 bit reg value.
//Send as write followed by length data bytes
//
void nrf24_writeRegArray(uint8_t reg, uint8_t* data, uint8_t length)
{
    uint8_t i = 0x00;
    uint8_t regValue = NRF24_CMD_W_REGISTER | (reg & (0x1F));

    SPI_select();                   //CS low
    SPI_tx(regValue);               //Set write reg

    //send the data bytes
    if (length > 0)
    {
        for (i = 0 ; i < length ; i++)
            SPI_tx(data[i]);
    }

    SPI_deselect();
}


//////////////////////////////////////////////////
//Write command followed by length data bytes
void nrf24_writeCmd(uint8_t command, uint8_t* data, uint8_t length)
{
    uint8_t i = 0x00;

    SPI_select();           //CS low
    SPI_tx(command);

    if (length > 0)
    {
        for (i = 0 ; i < length ; i++)
            SPI_tx(data[i]);
    }

    SPI_deselect();
}


////////////////////////////////////////////////////
//Read register value.  Combine read reg command
//with register bits.  Read one byte
uint8_t nrf24_readReg(uint8_t reg)
{
    uint8_t data = 0x00;
    uint8_t regValue = NRF24_CMD_R_REGISTER | (reg & (0x1F));

    SPI_select();                   //CS low
    SPI_tx(regValue);               //Set read reg
    data = SPI_rx();                //read the data
    SPI_deselect();                 //CS high

    return data;
}




/////////////////////////////////////////////////

//Read register data into data buffer.  Combine
//read reg command with 5 bit register value, send
//as read, followed by reading data bytes into data
void nrf24_readRegArray(uint8_t reg, uint8_t* data, uint8_t length)
{
    uint8_t i = 0x00;
    uint8_t regValue = NRF24_CMD_R_REGISTER | (reg & (0x1F));

    SPI_select();                   //CS low
    SPI_tx(regValue);               //Set read reg

    //send the data bytes
    if (length > 0)
    {
        for (i = 0 ; i < length ; i++)
            data[i] = SPI_rx();
    }

    SPI_deselect();
}





void nrf24_ce_high(void)
{
    PA_ODR |= BIT_1;
}

void nrf24_ce_low(void)
{
    PA_ODR &=~ BIT_1;
}


/////////////////////////////////////////////////
//Pulse the CE pin - for use with transmit mode
//pulse for at least 10us
//NRF24_CE_PULSE_LENGTH = 10000
void nrf24_ce_pulse(void)
{
    volatile uint32_t time = NRF24_CE_PULSE_LENGTH;

    PA_ODR |= BIT_1;

    while (time > 0)
        time--;

    PA_ODR &=~ BIT_1;
}


///////////////////////////////////////
//Set or Clear the PRIM_RX bit in the
//CONFIG REG
void nrf24_prime_rx_bit(uint8_t value)
{
    uint8_t result = nrf24_readReg(NRF24_REG_CONFIG);

    if (value == 1)
        result |= NRF24_BIT_PRIM_RX;
    else
        result &=~ NRF24_BIT_PRIM_RX;

    nrf24_writeReg(NRF24_REG_CONFIG, result);
}

void nrf24_power_up_bit(uint8_t value)
{
    uint8_t result = nrf24_readReg(NRF24_REG_CONFIG);

    if (value == 1)
        result |= NRF24_BIT_PWR_UP;
    else
        result &=~ NRF24_BIT_PWR_UP;

    nrf24_writeReg(NRF24_REG_CONFIG, result);
}


////////////////////////////////////
//nrf24_init()
//Configure the GPIO pins, radio mode, registers
//
//PA1 - output - CE pin
//PA2 - input / interrupt / falling edge trigger
//
//Pass the final mode
void nrf24_init(NRF24_Mode_t initialMode)
{
	//Configure PA1 - CE Pin as output
    PA_DDR = BIT_1;
    PA_CR1 = BIT_1;
    PA_CR2 = BIT_1;

    //configure PA2 as input, pullup, interrupt, falling edge trigger
    PA_DDR &=~ BIT_2;       //input
    PA_CR1 |= BIT_2;        //pullup
    PA_CR2 |= BIT_2;        //interrupt enabled

	//Interrupt Config
	//CCR reg = 0x28 - default, set I1 and I0 to 1.
	//required to enable external interrupts
    //PortA - falling edge to all of PortA, 
    //bit 1 high, bit 0 low
    EXTI_CR1 = BIT_1;    
	EXTI_CR1 &=~ BIT_0;

    //init all outputs low
    PA_ODR &=~ BIT_1;


    /////////////////////////////////////////////////
    //Configure the mode, registers, etc
    //Going through the registers.....
    //CONFIG - 0001 1000
    //enable rx and tx fifo interrupts only, power down, 
    //tx mode
    nrf24_writeReg(NRF24_REG_CONFIG, 0x18);

    //EN_AA - auto ack - default is all pipes enabled
    //EN_RXADDR - default is all rx pipes off...
    //SETUP_AW - default is 5 byte address widths
    //SETUP_RETR - delay between packet retry - default = 250us
    //disable retry - 0000 0000
    nrf24_writeReg(NRF24_REG_SETUP_RETR, 0x00);

    //RF_CH - Channel the radio opperates on - default

    //RF_SETUP - 1Mbps, default power output, 
    //0000 0 11 0
    nrf24_writeReg(NRF24_REG_RF_SETUP, 0x06);

    //STATUS - no change - clear all pending??

    //pipe addresses - leave these alone
    //RX_ADDR_P0 - Default 0xE7E7E7E7E7
    //RX_ADDR_P1 - Default 0xC2C2C2C2C2
    //RX_ADDR_P2 - Default 0xC3 (tx: C2C2C2C2 C3)
    //RX_ADDR_P3 - Default 0xC4 (tx: C2C2C2C2 C4)
    //RX_ADDR_P4 - Default 0xC5 (tx: C2C2C2C2 C5)
    //RX_ADDR_P5 - Default 0xC6 (tx: C2C2C2C2 C6)

    //pipe widths - set later    
    //RX_PW_P0 - payload width pipe0 - default 0
    //RX_PW_P1 - payload width pipe1 - default 0
    //RX_PW_P2 - payload width pipe2 - default 0
    //RX_PW_P3 - payload width pipe3 - default 0
    //RX_PW_P4 - payload width pipe4 - default 0
    //RX_PW_P5 - payload width pipe5 - default 0

    //dynamic payload lengths - leave this alone for now
    //FEATURE - leave this alone for now.

    nrf24_setMode(NRF24_MODE_POWER_DOWN);

    //config P0 - disable auto ack, enable the pipe, set width to 8 bytes
    nrf24_setPipeAutoAck(NRF24_DATA_PIPE_0, NRF24_PIPE_STATE_DISABLE);
    nrf24_setPipeRxEnable(NRF24_DATA_PIPE_0, NRF24_PIPE_STATE_ENABLE);

    //payload with rx = 8 bytes to trigger interrupt
    nrf24_setRxPayLoadSize(NRF24_DATA_PIPE_0, 8);

    //set mode to transmitter / receiver
    nrf24_dummyDelay(50000);
    nrf24_setMode(NRF24_MODE_STANDBY);
    nrf24_dummyDelay(50000);

    //which mode to go to from here...
    switch(initialMode)
    {
        case NRF24_MODE_POWER_DOWN:     nrf24_setMode(NRF24_MODE_POWER_DOWN);   break;
        case NRF24_MODE_STANDBY:        nrf24_setMode(NRF24_MODE_STANDBY);      break;
        case NRF24_MODE_RX:             nrf24_setMode(NRF24_MODE_RX);           break;
        case NRF24_MODE_TX:             nrf24_setMode(NRF24_MODE_TX);           break;
        default:                        nrf24_setMode(NRF24_MODE_STANDBY);      break;
    }

    nrf24_dummyDelay(50000);
}


//////////////////////////////////////
//Set the mode of the nrf24 radio, 
//powerdown, standby, rx, tx mode
//
void nrf24_setMode(NRF24_Mode_t mode)
{
    switch(mode)
    {
        case NRF24_MODE_POWER_DOWN:
        {
            mNRF24_Mode = NRF24_MODE_POWER_DOWN;
            nrf24_prime_rx_bit(0);      //set the prim-rx bit low
            nrf24_ce_low();         //set the ce pin low
            nrf24_power_up_bit(0);      //clear the powerup bit
            break;
        }
        case NRF24_MODE_STANDBY:
        {
            mNRF24_Mode = NRF24_MODE_STANDBY;
            nrf24_prime_rx_bit(0);      //set the prim-rx bit low
            nrf24_ce_low();             //set the ce pin low
            nrf24_power_up_bit(1);      //set power up bit to 1
            break;
        }
        case NRF24_MODE_TX:
        {
            //set the powerup bit high, PRIM_RX bit low
            //payload in the  tx fifo, and pulse CE pin high
            //for at least 10us.  ie, steadystate CE pin is low.
            //ie, this does not seem to be a steadystate mode.
            //when transmitting, set the mode to tx, write a payload,
            //then pulse the CE pin
            mNRF24_Mode = NRF24_MODE_TX;
            nrf24_prime_rx_bit(0);      //set the prim-rx bit low
            nrf24_ce_low();             //set the ce pin low
            nrf24_power_up_bit(1);      //set power up bit to 1
            break;
        }
        case NRF24_MODE_RX:
        {
            //set the power up bit high, set the PRIM_RX bit high
            //set the CE pin high
            mNRF24_Mode = NRF24_MODE_RX;
            nrf24_prime_rx_bit(1);      //set the prim-rx bit high
            nrf24_ce_high();            //set the ce pin high
            nrf24_power_up_bit(1);      //set power up bit to 1
            break;
        }

        default:
        {
            //equivalent of power down mode
            mNRF24_Mode = NRF24_MODE_POWER_DOWN;
            nrf24_prime_rx_bit(0);      //set the prim-rx bit low
            nrf24_ce_low();             //set the ce pin low
            nrf24_power_up_bit(0);      //clear the powerup bit
            break;
        }
    }
}



/////////////////////////////////////
//Get the current opperating mode
NRF24_Mode_t nrf24_getMode(void)
{
    return mNRF24_Mode;
}


////////////////////////////////////////
//Read the status register by sending dummy
//command and reading result on the MISO line
//Try reading as normal register
uint8_t nrf24_getStatus(void)
{
    uint8_t status = nrf24_readReg(NRF24_REG_STATUS);
    return status;

}


uint8_t nrf24_getFifoStatus(void)
{
    uint8_t status = nrf24_readReg(NRF24_REG_FIFO_STATUS);
    return status;

}



//////////////////////////////////////////////
//Check if the rx fifo has more data to read.
//Read the fifo status reg - bit 0 - RX_EMPTY bit
//if high, no more data to read.
//
uint8_t nrf24_RxFifoHasData(void)
{
    uint8_t bit, status = 0x00;
    status = nrf24_readReg(NRF24_REG_FIFO_STATUS);
    bit = status & NRF24_BIT_RX_EMPTY;

    //bit value = 0 - Not Empty
    if (!bit)
        return 1;
    else
        return 0;
}



/////////////////////////////////////////
//TxFifo Has Space
//returns 1 if there is an empty space
//in the fifo
//read the status reg, bit NRF24_BIT_TX_FULL
//reg returns 1 if full
uint8_t nrf24_TxFifoHasSpace(void)
{
    uint8_t status = nrf24_getStatus();

    //full = high bit
    if (status & NRF24_BIT_TX_FULL)
        return 0;
    else
        return 1;
}



//////////////////////////////////////
//Send command only - flush rx buffers
void nrf24_flushRx(void)
{
    uint8_t dummy = 0x00;
    nrf24_writeCmd(NRF24_CMD_FLUSH_RX, &dummy, 0);
}


////////////////////////////////////////
//Send command only - flush tx buffers
void nrf24_flushTx(void)
{
    uint8_t dummy = 0x00;
    nrf24_writeCmd(NRF24_CMD_FLUSH_TX, &dummy, 0);
}


////////////////////////////////////////////////////////
//Set or clear the auto ack response for a
//particular data pipe.  state can be NRF24_PIPE_STATE_ENABLE
//or NRF24_PIPE_STATE_DISABLE
//Set/clear bits in the EN_AA register
void nrf24_setPipeAutoAck(NRF24_DataPipe_t pipe, NRF24_PipeState_t state)
{
    uint8_t result = nrf24_readReg(NRF24_REG_EN_AA);   //read the current state of the EN_AA register
    uint8_t _pipe = (uint8_t)pipe;                      //uses the bit values in the enum
    
    if (state == NRF24_PIPE_STATE_ENABLE)
        result |= _pipe;
    else
        result &=~ _pipe;

    nrf24_writeReg(NRF24_REG_EN_AA, result);
}


////////////////////////////////////////////////////////
//Enable / Disable RX pipe - Pipe0 - Pipe5, or all
void nrf24_setPipeRxEnable(NRF24_DataPipe_t pipe, NRF24_PipeState_t state)
{
    uint8_t result = nrf24_readReg(NRF24_REG_EN_RXADDR);    //current state of the data pipes
    uint8_t _pipe = (uint8_t)pipe;                          //uses the bit values in the enum
    
    if (state == NRF24_PIPE_STATE_ENABLE)
        result |= _pipe;
    else
        result &=~ _pipe;

    nrf24_writeReg(NRF24_REG_EN_RXADDR, result);
}



//////////////////////////////////////////////////////
//Write data buffer to the tx payload register.
//max length = 32 bytes.  write to W_TX_PAYLOAD reg
//does not transmit, just dumps data into the tx fifo
//flush it with flush tx
void nrf24_writeTXPayLoad(uint8_t* buffer, uint8_t length)
{
    uint8_t numBytes = length;
    if (numBytes > 32)
        numBytes = 32;

    //write buffer to tx payload reg. - does not send data
    nrf24_writeRegArray(NRF24_CMD_W_TX_PAYLOAD, buffer, numBytes);
}


//////////////////////////////////////////////////////
//Transmit Data
//put in transmitter mode
//flush the tx buffer, write data to tx payload
//pulse the CE pin
//for odd tx payloads, dump 0x00 for the remainder
//
//NOTE: for now, assume the buffer length = 32 bytes
//
//Length has to be the same length as the receiver pipe
//if length is > NRF24_PIPE_WIDTH, 
//set the length to pipe width
//
void nrf24_transmitData(uint8_t* buffer, uint8_t length)
{
    uint8_t i = 0x00;
    uint8_t len = length;
    uint8_t txPacket[32] = {0x00};
    NRF24_Mode_t currentMode = NRF24_MODE_STANDBY;

    //just send the first PIPE_WIDTH bytes
    if (len != NRF24_PIPE_WIDTH)
        len = NRF24_PIPE_WIDTH;

    for (i = 0 ; i < len ; i++)
        txPacket[i] = buffer[i];
 
    currentMode = nrf24_getMode();                      //get the current mode - for return
    nrf24_setMode(NRF24_MODE_TX);                       //set the mode to transmit
    nrf24_flushTx();                                    //flush the tx fifo
    nrf24_writeTXPayLoad(txPacket, 32);                 //write the data to the payload reg
    nrf24_ce_pulse();                                   //pulse the ce pin

    //clear the transmit complete flag - this gets set
    //in the ISR when transmission is complete
    mTransmitCompleteFlag = 0;

    //wait until the transmission is complete
    while (!mTransmitCompleteFlag){};

    //return to the previous mode
    nrf24_setMode(currentMode);    
}



//////////////////////////////////////////////////
//Receiver functions

/////////////////////////////////////////////////////
//Set the size of the RX payload.  Likely the number 
//of bytes that tiggers an interrupt.  Range = 1 to 32
//0 - pipe not used
//
void nrf24_setRxPayLoadSize(NRF24_DataPipe_t pipe, uint8_t numBytes)
{
    uint8_t reg = 0x00;

    if (numBytes > 32)
        numBytes = 32;

    switch(pipe)
    {
        case NRF24_DATA_PIPE_0:         reg = NRF24_REG_RX_PW_P0;   break;
        case NRF24_DATA_PIPE_1:         reg = NRF24_REG_RX_PW_P1;   break;
        case NRF24_DATA_PIPE_2:         reg = NRF24_REG_RX_PW_P2;   break;
        case NRF24_DATA_PIPE_3:         reg = NRF24_REG_RX_PW_P3;   break;
        case NRF24_DATA_PIPE_4:         reg = NRF24_REG_RX_PW_P4;   break;
        case NRF24_DATA_PIPE_5:         reg = NRF24_REG_RX_PW_P5;   break;
        case NRF24_DATA_PIPE_ALL:       reg = NRF24_REG_RX_PW_P0;   break;      //use default P0
        default:                        reg = NRF24_REG_RX_PW_P0;   break;
    }

    nrf24_writeReg(reg, (numBytes & 0x3F));             //write num bytes to bits 0:5
}



//////////////////////////////////////////////////
//Read number of bytes to read in Rx Payload
//in a particular data pipe
uint8_t nrf24_getRxPayLoadSize(NRF24_DataPipe_t pipe)
{
    uint8_t reg = 0x00;
    uint8_t numBytes = 0x00;

    switch(pipe)
    {
        case NRF24_DATA_PIPE_0:         reg = NRF24_REG_RX_PW_P0;   break;
        case NRF24_DATA_PIPE_1:         reg = NRF24_REG_RX_PW_P1;   break;
        case NRF24_DATA_PIPE_2:         reg = NRF24_REG_RX_PW_P2;   break;
        case NRF24_DATA_PIPE_3:         reg = NRF24_REG_RX_PW_P3;   break;
        case NRF24_DATA_PIPE_4:         reg = NRF24_REG_RX_PW_P4;   break;
        case NRF24_DATA_PIPE_5:         reg = NRF24_REG_RX_PW_P5;   break;
        case NRF24_DATA_PIPE_ALL:       reg = NRF24_REG_RX_PW_P0;   break;      //use default P0
        default:                        reg = NRF24_REG_RX_PW_P0;   break;
    }

    //read the reg, capture bits 0:5, value & 0x3F
    numBytes = (nrf24_readReg(reg) & 0x3F);
    return numBytes;
}






//////////////////////////////////////////////////////////
//Read which pipe has data available to read from
//the RX Fifo.  STATUS register - bits 3:1, 111 = RX FIFO
//empty, 110 not used. ie, 0 - 5
uint8_t nrf24_getRxPipeToRead(void)
{
    uint8_t pipe = (nrf24_getStatus() >> 1) & 0x07;
    
    //test for valid, if 111, return 0xFF;
    if (pipe == 0x07)           //empty
        return 0xFF;
     else if (pipe == 0x06)     //not used, error
        return 0xFE;
    else
        return pipe;
}




////////////////////////////////////////////////////////
//Read the payload at the top of the Rx FIFO
//Uses command: NRF24_CMD_R_RX_PAYLOAD - reading 1 to 32 bytes
//#define NRF24_CMD_R_RX_PAYLOAD          0x61
void nrf24_readRxPayLoad(uint8_t* data, uint8_t length)
{
    uint8_t i = 0x00;
    uint8_t cmd = NRF24_CMD_R_RX_PAYLOAD;

    SPI_select();       //CS low
    SPI_tx(cmd);        //Set the read rx fifo cmd

    //read data bytes into buffer
    for (i = 0 ; i < length ; i++)
        data[i] = SPI_rx();

    SPI_deselect();
}



///////////////////////////////////////////////////
//Read next set of data available in the FIFO
//in a particular data pipe.  Gets the data pipe
//to read and the size of bytes in the pipe.  Reads
//the data in the Rx payload register, assuming it's 
//from that pipe.  Returns the number of bytes read
//and loads the data buffer.  
//At this point, no tracking of which pipe it's from
uint8_t nrf24_readRxData(uint8_t* data)
{
    uint8_t tPipe = 0x00;
    uint8_t length = 0x00;
    NRF24_DataPipe_t pipe = NRF24_DATA_PIPE_0;

    tPipe = nrf24_getRxPipeToRead();

    if (tPipe <= 0x05)      //pipe 5 max
    {
        switch(tPipe)
        {
            case 0x00:  pipe = NRF24_DATA_PIPE_0;   break;
            case 0x01:  pipe = NRF24_DATA_PIPE_1;   break;
            case 0x02:  pipe = NRF24_DATA_PIPE_2;   break;
            case 0x03:  pipe = NRF24_DATA_PIPE_3;   break;
            case 0x04:  pipe = NRF24_DATA_PIPE_4;   break;
            case 0x05:  pipe = NRF24_DATA_PIPE_5;   break;
            default:    pipe = NRF24_DATA_PIPE_0;   break;
        }

        //get the length of bytes to read for the pipe
        //uint8_t nrf24_getRxPayLoadSize(NRF24_DataPipe_t pipe)
        length = nrf24_getRxPayLoadSize(pipe);

        //read length bytes from rx payload into data buffer
        //void nrf24_readRxPayLoad(uint8_t* data, uint8_t length)
        
        nrf24_readRxPayLoad(data, length);

        return length;
    }

    return 0xFF;        //error
}



////////////////////////////////////////////////////////
//Called from the interrupt IRQ pin handler.
//Mapped to pin PA2 - input, falling edge
//Three interrupt sources: data transmitted successfully,
//max retransmissions reached, and data arrived in RX fifo.
//each it's own bit.  Write one to clear the bit.  
//
//For now, only enable the RX_DR interrupt and TX_DS interrupt
//
void nrf24_ISR(void)
{
    uint8_t len = 0x00;
    uint8_t rxBuffer[32] = {0x00};
    uint8_t status = nrf24_getStatus();

    //test for rx_dr packet available in the rx fifo
    if (status & NRF24_BIT_RX_DR)
    {
        //read the rx fifo until there is no more
        //data packets to read
        while (nrf24_RxFifoHasData())
        {
            //read the packet
            len = nrf24_readRxData(rxBuffer);

            //forward the packet out the uart
            //void UART_sendStringLength(uint8_t* buffer, uint16_t length);
            UART_sendStringLength(rxBuffer, len);
        
            //write 1 to the RX_DR 
            nrf24_writeReg(NRF24_REG_STATUS, (status |= NRF24_BIT_RX_DR));
        }
    }

    //test if the transmit - data sent interrupt triggered
    else if (status & NRF24_BIT_TX_DS)
    {
        //set the transmit complete flag - read in the transmit function
        mTransmitCompleteFlag = 1;
        //post a message over the usart about transmit complete
        UART_sendString("Transmission Complete...\n");
        //write 1 to clear the transmit complete flag
        nrf24_writeReg(NRF24_REG_STATUS, (status |= NRF24_BIT_TX_DS));

    }
}



