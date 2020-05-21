/*
4/25/20
Button Controller File
For use with interrupted GPIO button pins,
flags set in the ISR and polled in the main game loop

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

