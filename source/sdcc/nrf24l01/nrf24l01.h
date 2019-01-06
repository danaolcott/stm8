/*
NRF24L01 Controller File
12/25/18
Dana Olcott

*/

#ifndef __NRF24L01__H
#define __NRF24L01__H


#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

//pulse length of the CE pin, for use with transmitting
//data out of tx payload.  at cpu = 16mhz, this is about 10ms
#define NRF24_CE_PULSE_LENGTH           ((uint16_t)5000)
#define NRF24_TX_TIMEOUT                ((uint16_t)50000)

#define NRF24_PIPE_WIDTH                ((uint8_t)8)
#define NRF24_PIPE_WIDTH_MAX            ((uint8_t)32)

#define NRF24_CHANNEL                   ((uint8_t)2)


///////////////////////////////////////////////
//Register Definitions - Commands
//first byte in any SPI transmission
#define NRF24_CMD_R_REGISTER            0x00
#define NRF24_CMD_W_REGISTER            0x20

#define NRF24_CMD_R_RX_PAYLOAD          0x61
#define NRF24_CMD_W_TX_PAYLOAD          0xA0

#define NRF24_CMD_FLUSH_TX              0xE1
#define NRF24_CMD_FLUSH_RX              0xE2

#define NRF24_CMD_REUSE_TX_PL           0xE3
#define NRF24_CMD_R_RX_PL_WID           0x60

#define NRF24_CMD_W_ACK_PAYLOAD         0xA8

#define NRF24_CMD_W_TX_PAYLOAD_NOACK    0xB0
#define NRF24_CMD_NOP                   0xFF


////////////////////////////////////////////////
//Bit Definitions
//Reg: CONFIG
#define NRF24_BIT_MASK_RX_DR            (1u << 6)
#define NRF24_BIT_MASK_TX_DS            (1u << 5)
#define NRF24_BIT_MAX_RT                (1u << 4)
#define NRF24_BIT_EN_CRC                (1u << 3)
#define NRF24_BIT_CRCO                  (1u << 2)
#define NRF24_BIT_PWR_UP                (1u << 1)
#define NRF24_BIT_PRIM_RX               (1u << 0)


//reg: STATUS
#define NRF24_BIT_RX_DR                 (1u << 6)      //data received in rx fifo - write 1 to clear
#define NRF24_BIT_TX_DS                 (1u << 5)      //data sent from tx fifo - write 1 to clear
#define NRF24_BIT_MAX_RT                (1u << 4)      //hit max # re-transmissions, write 1 to reenable tx
#define NRF24_BIT_TX_FULL               (1u << 0)      //1 = tx full, 0 = tx empty

//reg: FIFO_STATUS
#define NRF24_BIT_RX_EMPTY              (1u << 0)       //1 = empty







///////////////////////////////////////////////
//Register Definitions - Registers
#define NRF24_REG_CONFIG              0x00
#define NRF24_REG_EN_AA               0x01
#define NRF24_REG_EN_RXADDR           0x02
#define NRF24_REG_SETUP_AW            0x03
#define NRF24_REG_SETUP_RETR          0x04
#define NRF24_REG_RF_CH               0x05
#define NRF24_REG_RF_SETUP            0x06
#define NRF24_REG_STATUS              0x07
#define NRF24_REG_OBSERVE_TX          0x08
#define NRF24_REG_RPD                 0x09    //receive power detector
#define NRF24_REG_RX_ADDR_P0          0x0A
#define NRF24_REG_RX_ADDR_P1          0x0B
#define NRF24_REG_RX_ADDR_P2          0x0C
#define NRF24_REG_RX_ADDR_P3          0x0D
#define NRF24_REG_RX_ADDR_P4          0x0E
#define NRF24_REG_RX_ADDR_P5          0x0F

#define NRF24_REG_TX_ADDR             0x10

//num bytes in rx payload, pipe 0-5
#define NRF24_REG_RX_PW_P0            0x11
#define NRF24_REG_RX_PW_P1            0x12
#define NRF24_REG_RX_PW_P2            0x13
#define NRF24_REG_RX_PW_P3            0x14
#define NRF24_REG_RX_PW_P4            0x15
#define NRF24_REG_RX_PW_P5            0x16

#define NRF24_REG_FIFO_STATUS         0x17

#define NRF24_REG_DYNPD               0x1C
#define NRF24_REG_FEATURE             0x1D



/////////////////////////////////////////////////
//enums
typedef enum
{
    NRF24_MODE_TX,
    NRF24_MODE_RX
}NRF24_Mode_t;




/////////////////////////////////////////////////
//Message ID's - 
//Shared list of message ids that define various
//sensor inputs, locations, etc.
typedef enum
{
    MID_ADC_TEMP1 = 0x00      //Millivolts read by temp sensor
}NRF24_MID_t;


////////////////////////////////////////////////
//NRF24 Packet Definition
//8 bytes
//Byte 0        0xFE
//Byte 1        NRF24_MID_t
//Bytes 2 - 7   Data Bytes
//      ADC - Bytes 2 and 3 - remaining 0x00


////////////////////////////////////////////////
//Prototypes
void nrf24_dummyDelay(uint32_t delay);

void nrf24_writeReg(uint8_t reg, uint8_t data);
void nrf24_writeRegArray(uint8_t reg, uint8_t* data, uint8_t length);
void nrf24_writeCmd(uint8_t command, uint8_t* data, uint8_t length);
uint8_t nrf24_readReg(uint8_t reg);

void nrf24_ce_high(void);
void nrf24_ce_low(void);
void nrf24_ce_pulse(void);

void nrf24_prime_rx_bit(uint8_t value);

void nrf24_power_up(void);
void nrf24_power_down(void);


void nrf24_init(NRF24_Mode_t initialMode);


uint8_t nrf24_getStatus(void);
uint8_t nrf24_getFifoStatus(void);
uint8_t nrf24_RxFifoHasData(void);

uint8_t nrf24_TxFifoHasSpace(void);

void nrf24_flushRx(void);
void nrf24_flushTx(void);



//transmit
void nrf24_setTxPipe(uint8_t pipe);
void nrf24_writeTXPayLoad(uint8_t* buffer, uint8_t length);
void nrf24_transmitData(uint8_t pipe, uint8_t* buffer, uint8_t length);

//receive
uint8_t nrf24_getRxPayLoadSize(uint8_t pipe);                           //width of the pipe
void nrf24_setRxPayLoadSize(uint8_t pipe, uint8_t numBytes);            //set width of pipe
uint8_t nrf24_getRxPipeToRead(void);                                    //which pipe to read next

void nrf24_readRxPayLoad(uint8_t* data, uint8_t length);        //read the top payload in the rx fifo
uint8_t nrf24_readRxData(uint8_t* data, uint8_t* pipe);         //read data in rx pipe, returns len bytes



////////////////////////////////////////////////
//called from the interrupt IRQ pin handler.
void nrf24_ISR(void);





#endif

