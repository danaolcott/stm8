/*
 * cliFunctions.c
 *
 *  Created on: Aug 22, 2018
 *      Author: danao
 */

#include <stdio.h>
#include <string.h>

#include "cliFunctions.h"
#include "cliTable.h"
#include "uart.h"



///////////////////////////////////////////////////////
//Function Defintions for Command Table
//Print Help
void cmdHelp(int argc, char** argv)
{
	char buffer[64];
    int n;
	int counter = 0x00;

    UART_sendString("(CMD #\tCMD\t\tHelp String\n");
    UART_sendString("----------------------------------\n");

    if ((argc == 1) && (argv[0][0] == '?'))
    {
	    while (CommandTable[counter].cmdNumber != 0xFF)
	    {
            n = sprintf(buffer, "%d\t%s\t\t%s\n", 
                CommandTable[counter].cmdNumber,
                CommandTable[counter].cmdString,
                CommandTable[counter].helpString);

            UART_sendStringLength(buffer, n);

            counter++;
	    }
    }
}





///////////////////////////////////////////////////////
//Test Command
//Echo Args
void cmdTest(int argc, char** argv)
{
	char buffer[50];
    int i, n;

	//print the args
	UART_sendString("CMD Test Function\n");

	//print out the args
	for (i = 0 ; i < argc ; i++)
	{
		n = sprintf(buffer, "Arg %d: %s\r\n", i, argv[i]);
		UART_sendStringLength((uint8_t*)buffer, n);
	}
}



