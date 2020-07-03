/*
4/25/20
Button Controller File
//order: left, center, up, right, down, user button - bits 0-5

*/


#ifndef __BUTTON_H
#define __BUTTON_H

#include "register.h"		//bit defs

typedef enum {
	BUTTON_LEFT = BIT_0,
	BUTTON_CENTER = BIT_1,
	BUTTON_UP = BIT_2,
	BUTTON_RIGHT = BIT_3,
	BUTTON_DOWN = BIT_4,
	BUTTON_USER = BIT_5
}ButtonType_t;

void Button_init(void);
ButtonType_t Button_getFlag(void);
void Button_clearFlag(void);
void Button_setFlag(ButtonType_t button);



#endif

