/*
 * cliTable.c
 *
 *  Created on: Aug 22, 2018
 *      Author: danao
 *
typedef struct
{
	int cmdNumber;						//command number
	char* cmdString;					//command search string
	void* functionPtr(int, char**);		//pointer to function to run
	char* helpString;					//help string for print help
}CommandItem;
 *
 */

#include <stdio.h>          //NULL definition
#include "cliTable.h"
#include "cliFunctions.h"


///////////////////////////////////////////////////////
//CommandTable definitions
//NOTE: Last command num has to be 0xFF - terminates
//the search when getting function to run, cmdString,
//help string, etc.
//
CommandItem CommandTable[] =
{
	{0, 	"?"     , cmdHelp, 	"Print Help"},
	{1, 	"test"  , cmdTest, 	"Test Command"},

	{0xFF, 	NULL, 	NULL, 		NULL}
};


