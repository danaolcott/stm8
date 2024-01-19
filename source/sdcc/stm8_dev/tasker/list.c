/*
 * list.c
 *
 *  Created on: Feb 15, 2020
 *      Author: riley
 *
 *  Linked list module
 */


#include <stddef.h>
#include <stdlib.h>

#include "list.h"


//define array of ListNodes
ListNode List[LIST_SIZE];
ListNode *ListHead;


/////////////////////////////////////////////////
//Called from the timer handler function
//Updates the timetick for each node and sets the
//run flag as needed.  List functions are run in the
//main loop.
void list_timeout(void)
{
	ListNode *ptr = ListHead;

	//traverse the list, decrement the timeout
	//and set the runflag as needed.

	while (ptr->next != NULL)
	{
		//decrement the timetick
		if (ptr->timetick > 0)
			ptr->timetick--;
		else
		{
			ptr->runFlag = 1;					//set the run flag
			ptr->timetick = ptr->timeout;		//reset the timer
		}

		ptr = ptr->next;
	}

	//decrement the last one and reset
	if (ptr->timetick > 0)
		ptr->timetick--;
	else
	{
		ptr->runFlag = 1;					//set the run flag
		ptr->timetick = ptr->timeout;		//reset the timer
	}
}



///////////////////////////////////////////////
//list_run
//infinite loop.  put this function in the main loop
//The run flag is set in the timer callback function,
//which decrements the timer tick each time the timeout
//is called.  after the run flag is set in the timer
//callback, the run flag is set, so the function to
//run can be called here.
void list_run(void)
{
	ListNode *ptr = ListHead;

	while(1)
	{
		if (ptr->runFlag == 1)
		{
			//clear the flag
			ptr->runFlag = 0;
			//run the function
			ptr->functionPtr();
		}

		//increment the pointer or reset to head
		if (ptr->next != NULL)
			ptr = ptr->next;
		else
			ptr = ListHead;

	}
}



////////////////////////////////////////
//init the list with top node with name
//and returns the address of the head
ListNode* list_init(uint8_t* name, uint16_t timeout, void (*functionPtr)(void))
{
	ListNode *ptr;
	uint8_t i, j;

	//set the ptr to the top of the list
	ptr = List;

	//clear the list
	for (i = 0 ; i < LIST_SIZE ; i++)
	{
		List[i].alive = 0;
		List[i].next = NULL;
		List[i].index = i;
		List[i].timeout = 0;
		List[i].timetick = 0;
		List[i].runFlag = 0;
		List[i].functionPtr = NULL;

		for (j = 0 ; j < LIST_NODE_NAME_SIZE ; j++)
		    List[i].name[j] = 0x00;
	}

	//init the top node
	ptr = List;
	ptr->alive = 1;
	ptr->next = NULL;

	for (i = 0 ; i < LIST_NODE_NAME_SIZE ; i++)
	    ptr->name[i] = name[i];

	ptr->timeout = timeout;				//freq to run
	ptr->timetick = timeout;			//down counter
	ptr->runFlag = 0;
	ptr->functionPtr = functionPtr;		//function to run

	//set the head
	ListHead = ptr;

	return ListHead;
}



///////////////////////////////////////
//Clears the list and reinit the head
//with name.  Returns address of the head
ListNode* list_clear(uint8_t *name, uint16_t timeout, void (*functionPtr)(void))
{
    uint8_t i, j;
	ListNode *ptr;
	ptr = List;

	//clear the list
	for (i = 0 ; i < LIST_SIZE ; i++)
	{
		List[i].alive = 0;
		List[i].next = NULL;
		List[i].index = i;
		List[i].timeout = 0;
		List[i].timetick = 0;
		List[i].runFlag = 0;
		List[i].functionPtr = NULL;

		for (j = 0 ; j < LIST_NODE_NAME_SIZE ; j++)
		    List[i].name[j] = 0x00;
	}

	//init the top node
	ptr = List;
	ptr->alive = 1;
	ptr->next = NULL;

    for (i = 0 ; i < LIST_NODE_NAME_SIZE ; i++)
        ptr->name[i] = name[i];

	ptr->timeout = timeout;				//freq to run
	ptr->timetick = timeout;			//down counter
	ptr->runFlag = 0;
	ptr->functionPtr = functionPtr;		//function to run

	//set the head
	ListHead = ptr;

	return ListHead;
}


///////////////////////////////////////////
//Print the names of the list active nodes
void list_print(void)
{
}



///////////////////////////////////////////
//Traverse the list and return the length
uint8_t list_getLength(void)
{
	uint8_t length = 1;
	//start at the head
	ListNode *ptr = ListHead;

	while ((ptr->next != NULL) && (length < LIST_SIZE))
	{
		ptr = ptr->next;
		length++;
	}

	return length;
}

////////////////////////////////////////////
//Traverse the array and return the address
//of the first node with alive = 0
ListNode* list_getFreeNode(void)
{
	ListNode* ptr = List;

	for (int i = 0 ; i < LIST_SIZE ; i++)
	{
		if (!(List[i].alive))
		{
			ptr = &List[i];
			ptr->alive = 1;
			return ptr;
		}
	}

	//list is full
	return NULL;
}


///////////////////////////////////////////////
//Append node to the end of the list, setting
//it's property alive = 1, name = name
ListNode* list_appendNode(uint8_t* name, uint16_t timeout, void (*functionPtr)(void))
{
    uint8_t i;
	ListNode *ptr = ListHead;
	ListNode *newNode = NULL;

	//get a new node
	newNode = list_getFreeNode();

	if (newNode == NULL)
		return NULL;

	//set the properties of the node
	newNode->alive = 1;

    for (i = 0 ; i < LIST_NODE_NAME_SIZE ; i++)
        newNode->name[i] = name[i];

	newNode->timeout = timeout;				//freq to run
	newNode->timetick = timeout;			//down counter
	newNode->runFlag = 0;
	newNode->functionPtr = functionPtr;		//function to run

	//jump to the end
	while (ptr->next != NULL)
		ptr = ptr->next;

	//set the new node location
	ptr->next = newNode;
	newNode->next = NULL;

	return ListHead;
}


////////////////////////////////////////////////
//Stack node at the front of the list
//update the head, return pointer to the head
ListNode* list_stackNode(uint8_t* name, uint16_t timeout, void (*functionPtr)(void))
{
    uint8_t i;
	ListNode *ptr = ListHead;
	ListNode *newNode = NULL;

	//allocate a new node
	newNode = list_getFreeNode();

	//if none are available
	if (newNode == NULL)
		return NULL;

	//set the new node to the front
	newNode->next = ptr;

	//set the properties of the new node
    for (i = 0 ; i < LIST_NODE_NAME_SIZE ; i++)
        newNode->name[i] = name[i];

	newNode->timeout = timeout;				//freq to run
	newNode->timetick = timeout;			//down counter
	newNode->runFlag = 0;
	newNode->functionPtr = functionPtr;		//function to run

	//update the head, tail continues to point to null
	ListHead = newNode;

	return ListHead;
}


