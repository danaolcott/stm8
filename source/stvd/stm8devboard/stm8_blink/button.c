/*
4/25/20
Button Controller File

*/

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#include "register.h"
#include "button.h"


ButtonType_t mButtonFlag = 0x00;



void Button_init(void)
{
    mButtonFlag = 0x00;
}

ButtonType_t Button_getFlag(void)
{
    return mButtonFlag;
}

void Button_clearFlag(void)
{
    mButtonFlag = 0x00;
}

void Button_setFlag(ButtonType_t button)
{
    mButtonFlag = button;
}

