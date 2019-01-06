/*
UART Controller File for the STM8SF103 Processor.  
12/20/18 Dana Olcott

Pinout - Configure as alternate function
PD5 - UART1_TX
PD6 - UART1_RX

*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <stddef.h>
#include "register.h"
#include "uart.h"
#include "cliTable.h"

////////////////////////////////////////////
//UART1 Private Variables
static volatile uint8_t rxBuffer1[UART_RX_BUFFER_SIZE];
static volatile uint8_t rxBuffer2[UART_RX_BUFFER_SIZE];
static volatile uint8_t rxActiveBuffer = 0x01;
static volatile uint8_t rxIndex = 0x00;


//////////////////////////////////////////
//Configure UART
//default rate = 57600
void UART_init(UART_BaudRate_t rate)
{
	uint8_t brr1 = 0x00;
	uint8_t brr2 = 0x00;

    //PD5 - UART1_TX - output, PP, fast
    PD_DDR |= BIT_5;		//output
    PD_CR1 |= BIT_5;
    PD_CR2 |= BIT_5;

    //PD6 - UART1_RX - input, floating, no interrupt
    PD_DDR &=~ BIT_6;
    PD_CR1 &=~ BIT_6;
    PD_CR2 &=~ BIT_6;

	//reset vars, buffers, etc
	memset(rxBuffer1, 0x00, UART_RX_BUFFER_SIZE);
	memset(rxBuffer2, 0x00, UART_RX_BUFFER_SIZE);
	rxActiveBuffer = 1;
	rxIndex = 0x00;

	//set the baud rate registers, assuming Fmaster = 16mhz
    //see programming manual for brr1 and brr2
	switch(rate)
	{
		case BAUD_RATE_9600:	brr1 = 0x68;		brr2 = 0x03;		break;
		case BAUD_RATE_57600:	brr1 = 0x11;		brr2 = 0x06;		break;
		default:			    brr1 = 0x11;		brr2 = 0x06;		break;		
	}

    UART1_BRR1 = brr1;      //set the baud rate
    UART1_BRR2 = brr2;      //set the baud rate

	//UART1_CR2 	- control reg2 - interrupts / enable
	UART1_CR2 = (UART1_TEN_BIT | UART1_REN_BIT);        //turn on tx and rx
    UART1_CR2 |= UART1_RIEN_BIT;                        //enable receive interrupts
}


//////////////////////////////////////////////
//UART_sendByte
//put data into the UART1_DR, poll the TC bit
//in the SR
void UART_sendByte(uint8_t data)
{
	while (!(UART1_SR & UART1_TXE_BIT)){};
	UART1_DR = data;
    while (!(UART1_SR & UART1_TXE_BIT)){};
}



void UART_sendString(uint8_t* msg)
{
    uint8_t counter = 0;
    while ((msg[counter] != 0x00) && (counter < 128))
        UART_sendByte(msg[counter++]);
}


void UART_sendStringLength(uint8_t* buffer, uint16_t length)
{
	uint16_t i = 0x00;

	for (i = 0 ; i < length ; i++)
		UART_sendByte(buffer[i]);
}



///////////////////////////////////////////////
//Process the command
void UART_processCommand(uint8_t* buffer, uint8_t length)
{
	int numArgs = 0x00;						        //destination numArgs
    int i = 0x00;
	char* argv[UART_RX_BUFFER_SIZE];                //array of char*
	uint8_t input[UART_RX_BUFFER_SIZE] = {0x00};	//modified original buffer

    //remove line endings
	for (i = 0x00 ; i < length ; i++)
	{
        input[i] = buffer[i];
		if ((input[i] == '\r') || (input[i] == '\n'))
			input[i] = 0x00;
	}

	//populate numargs and load argv with char*
	UART_parseArgs(input, &numArgs, argv);

	//run the command
	UART_executeCommand(numArgs, argv);
}



/////////////////////////////////////////////////////////
//UART_parseArgs
//orig - initial buffer that is modified.
//argc is the number of args
//argv - array of pointers. - point to the first 
//char after a null char
//orig is assumed to be null terminated
void UART_parseArgs(uint8_t *orig, int* argc, char** argv)
{
	int numArgs = 0x00;
    int i = 0x00;
    int counter = 0x00;
        uint8_t c = 0x00;
        
    while (orig[counter] != 0x00)
    {
        c = orig[counter];
        if ((c == ' ') || (c == ','))
            orig[counter] = 0x00;
            
        counter++;
    }

    //populate the array of char* by placing a 
    //pointer at the first char of each token

    //first arg
    argv[numArgs++] = &orig[0];
    
    //compare current and one byte ahead
    for (i = 0 ; i < (counter - 1) ; i++)
    {
        //if the current == null and the next one is not null
        if ((orig[i] == 0x00) && (orig[i + 1] > 0x00))
            argv[numArgs++] = &orig[i + 1];
    }
    
	*argc = numArgs;
}



/////////////////////////////////////////////////
//Execute the command
//Get the command entry from CommandTable, 
//run the cooresponding function pointer.
void UART_executeCommand(int argc, char** argv)
{
	//get the command table entry from argv[0]
	int cmd = UART_parseCommand(argv[0]);

	if (cmd >= 0)
	{
		//execute the function associated with cmd##
		CommandTable[cmd].functionPtr(argc, argv);
	}
	else
	{
        UART_sendString("CMD NOT FOUND (? - CMD List): ");
        UART_sendString((char*)argv[0]);
        UART_sendString("\n");
	}
}


//////////////////////////////////////////////////////
//Look up command based on arg0 string compare.
//Return -1 if command is wrong
int UART_parseCommand(char* cmd)
{
	int counter = 0x00;

	while (CommandTable[counter].cmdNumber != 0xFF)
	{
		//compare cmd with .cmdString
		if (!strcmp(CommandTable[counter].cmdString, cmd))
		{
			return CommandTable[counter].cmdNumber;
		}

		counter++;
	}

	return -1;
}


/////////////////////////////////////////////
//UART1_ISR - Called when incoming byte
//is received.  Process: put byte into buffer
//if line ending, pass buffer to processing
//function, swap buffers
//
void UART_ISR(void)
{
    uint8_t data;

	//receiver is not empty - data in the UART1_DR
	if (UART1_SR & UART1_RXNE_BIT)
	{
		data = UART1_DR;
        if (rxIndex < (UART_RX_BUFFER_SIZE - 1))
        {
            if (rxActiveBuffer == 1)
                rxBuffer1[rxIndex++] = data;
            else
                rxBuffer2[rxIndex++] = data;

            if (data == '\n')
            {
                if (rxActiveBuffer == 1)
                {
                    //end the string and process
                    rxBuffer1[rxIndex] = 0x00;
                    
                    //set data ready flat - rxBuffer1
					UART_processCommand(rxBuffer1, rxIndex);

                    rxIndex = 0x00;
                    rxActiveBuffer = 2;
                    memset(rxBuffer2, 0x00, UART_RX_BUFFER_SIZE);
                }

                else
                {
                    //end the string and process
                    rxBuffer2[rxIndex] = 0x00;
					UART_processCommand(rxBuffer2, rxIndex);

                    rxIndex = 0x00;
                    rxActiveBuffer = 1;
                    memset(rxBuffer1, 0x00, UART_RX_BUFFER_SIZE);
                }
            }
        }

        else
        {           
            rxIndex = 0x00;
            memset(rxBuffer1, 0x00, UART_RX_BUFFER_SIZE);
            memset(rxBuffer2, 0x00, UART_RX_BUFFER_SIZE);
            rxActiveBuffer = 1;
        }
    }
}



