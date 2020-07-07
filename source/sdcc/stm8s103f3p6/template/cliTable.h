/*
 * cliTable.h
 *
 *  Created on: Aug 22, 2018
 *      Author: danao
 *
 *  Table of Commands to be executed via the CLI
 *  CommandTable contains an array of CommandItems
 *
 */

#ifndef CLITABLE_H_
#define CLITABLE_H_


typedef struct
{
	int cmdNumber;									//command number
	char* cmdString;								//command search string
	void (*functionPtr)(int argc, char** argv);		//pointer to function to run
	char* helpString;								//help string for print help
}CommandItem;

extern CommandItem CommandTable[];

#endif

