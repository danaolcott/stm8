/*
 *
 * A simple tasker using a linked list and a timer
 *
 * list.h
 *
 *  Created on: Feb 15, 2020
 *      Author: riley
 *
 *  Linked list
 *
 *
 */

#ifndef LIST_H_
#define LIST_H_

#define LIST_SIZE 		10
#define LIST_NODE_NAME_SIZE		8

#include <stdint.h>
#include <stddef.h>

//List node
struct node
{
	struct node *next;						//pointer to node
	uint8_t name[LIST_NODE_NAME_SIZE];			//name of the node
	uint8_t alive;								//active node
	uint8_t index;
	uint16_t timeout;
	uint16_t timetick;
	uint8_t runFlag;
	void (*functionPtr)(void);
};

typedef struct node ListNode;

//functions
void list_timeout(void);
void list_run(void);

ListNode* list_init(uint8_t* name, uint16_t timeout, void (*functionPtr)(void));
ListNode* list_clear(uint8_t *name, uint16_t timeout, void (*functionPtr)(void));
void list_print(void);
uint8_t list_getLength(void);
ListNode* list_getFreeNode(void);

ListNode* list_appendNode(uint8_t* name, uint16_t timeout, void (*functionPtr)(void));
ListNode* list_stackNode(uint8_t* name, uint16_t timeout, void (*functionPtr)(void));


#endif /* LIST_H_ */
