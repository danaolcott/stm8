/*
UART Controller File for the STM8SF103 Processor.  
12/20/18 Dana Olcott


*/


#ifndef __UART__H
#define __UART__H

#include <stdint.h>
#include <stddef.h>

#define UART_RX_BUFFER_SIZE			32


typedef enum
{
	BAUD_RATE_9600,
	BAUD_RATE_19200,
	BAUD_RATE_57600,
	BAUD_RATE_115200,
	BAUD_RATE_230400,
	BAUD_RATE_921600
}UART_BaudRate_t;

void UART_init(UART_BaudRate_t rate);
void UART_sendByte(uint8_t data);
void UART_sendString(uint8_t* msg);
void UART_sendStringLength(uint8_t* buffer, uint16_t length);
void UART_processCommand(uint8_t* buffer, uint8_t length);
void UART_parseArgs(uint8_t *orig, int* argc, char** argv);
void UART_executeCommand(int argc, char** argv);
int UART_parseCommand(char* cmd);


void UART_ISR(void);



#endif

