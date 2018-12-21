/*
UART Controller File for the STM8SF103 Processor.  
12/20/18 Dana Olcott

Pinout - Configure as alternate function
PD5 - UART1_TX
PD6 - UART1_RX

*/

#include <stdint.h>
#include <stddef.h>
#include "register.h"
#include "usart.h"


////////////////////////////////////////////
//UART1 Private Variables
static uint8_t rxBuffer1[UART_RX_BUFFER_SIZE];
static uint8_t rxBuffer2[UART_RX_BUFFER_SIZE];
static uint8_t rxActiveBuffer = 0x01;
static uint8_t rxIndex = 0x00;



//////////////////////////////////////////
//Configure UART
//default rate = 115200
void UART_init(UART_BaudRate_t rate)
{
	//configure pins


	//reset vars, buffers, etc
	memset(rxBuffer1, 0x00, UART_RX_BUFFER_SIZE);
	memset(rxBuffer2, 0x00, UART_RX_BUFFER_SIZE);
	rxActiveBuffer = 1;
	rxIndex = 0x00;


	uint8_t brr1 = 0x00;
	uint8_t brr2 = 0x00;

	//set the baud rate registers, defaults to 115200
	//these assume fmaster = 16mhz
	//UART1_BRR1 	- baud rate registers
	//UART1_BRR2

	switch(rate)
	{
		case BAUD_RATE_9600:	brr1 = 0x68;		brr2 = 0x03;		break;
		case BAUD_RATE_19200:	brr1 = 0x34;		brr2 = 0x01;		break;
		case BAUD_RATE_57600:	brr1 = 0x11;		brr2 = 0x06;		break;
		case BAUD_RATE_115200:	brr1 = 0x08;		brr2 = 0x0B;		break;
		case BAUD_RATE_230400:	brr1 = 0x04;		brr2 = 0x05;		break;
		case BAUD_RATE_921600:	brr1 = 0x01;		brr2 = 0x01;		break;
		case default:			brr1 = 0x08;		brr2 = 0x0B;		break;		
	}

	//UART1_SR 		- status register
	//UART1_DR 		- data register
	//UART1_CR1 	- control reg1 - do nothing
	//UART1_CR2 	- control reg2 - interrupts / enable
	//enable rx interrupt, rx and tx turned on
	UART1_CR2 = (UART1_RIEN_BIT | UART1_TEN_BIT | UART1_REN_BIT);

	//UART1_CR3 	- control reg3 - do nothing - SCLK pin optional
	//UART1_CR4 	- control reg4 - do nothing
	//UART1_CR5 	- control reg5 - do nothing
	//UART1_GTR 	- guard time register - do nothing
	//UART1_PSCR 	- prescale register - do nothing
	
}


//////////////////////////////////////////////
//UART_sendByte
//put data into the UART1_DR, poll the TC bit
//in the SR
void UART_sendByte(uint8_t data)
{
	UART1_DR = data;
	while (!(UART1_SR & UART1_TC_BIT)){};
	while (!(UART1_SR & UART1_TXE_BIT)){};
}

void UART_sendString(uint8_t* msg)
{
	uint16_t i = 0x00;
	uint16_t len = strlen(msg);

	for (i = 0 ; i < len ; i++)
		UART_sendByte(msg[i]);
}

void UART_sendStringLength(uint8_t* buffer, uint16_t length)
{
	uint16_t i = 0x00;

	for (i = 0 ; i < length ; i++)
		UART_sendByte(buffer[i]);
}


//////////////////////////////////////////
//UART1_ISR - Called when incoming byte
//is received.  Process: put byte into buffer
//if line ending, swap buffers, post buffer
//to processing function
void UART_ISR(void)
{
	uint8_t data = 0x00;

	//receiver is not empty - data in the UART1_DR
	if (UART1_SR & UART1_RXNE_BIT)
	{
		data = UART1_DR;

		if ((data != 0x00) && (rxIndex < (UART_RX_BUFFER_SIZE - 1)))
		{
			if (rxActiveBuffer == 1)
				rxBuffer1[rxIndex] = data;
			else
				rxBuffer2[rxIndex] = data;

			rxIndex++;

			//test the char
			if (data == '\n')
			{
				if (rxActiveBuffer == 1)
				{
					rxBuffer1[rxIndex] = 0x00;

					//process command
					UART1_processCommand(rxBuffer1, rxIndex);

					//flip the buffers
					rxActiveBuffer = 2;
					memset(rxBuffer2, 0x00, UART_RX_BUFFER_SIZE);
					rxIndex = 0x00;
				}

				else
				{
					rxBuffer2[rxIndex] = 0x00;

					//process command
					UART1_processCommand(rxBuffer2, rxIndex);

					//flip the buffers
					rxActiveBuffer = 1;
					memset(rxBuffer1, 0x00, UART_RX_BUFFER_SIZE);
					rxIndex = 0x00;
				}
			}

		//bad data or buffer overrun, reset everything
		rxActiveBuffer = 1;
		memset(rxBuffer1, 0x00, UART_RX_BUFFER_SIZE);
		memset(rxBuffer2, 0x00, UART_RX_BUFFER_SIZE);
		rxIndex = 0x00;

		}
	}
}



///////////////////////////////////////////////
//Process the command
void UART1_processCommand(uint8_t* buffer, uint8_t length)
{

}


