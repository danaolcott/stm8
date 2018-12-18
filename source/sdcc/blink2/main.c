/*
Dana Olcott
12/12/18

STM8SF103
User LED on PB5
User Button on PB4

*/

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include "register.h"
#include "gpio.h"


//prototypes
void Delay(unsigned int val);

int main()
{
    GPIO_init();
    Timer4_init();
    Timer4_start();

    while (1)
    {

        Delay(10000);
    }

    return 0;
}



////////////////////////////////////
//Generic delay function.  Not calibrated
//this was just to get the quad-core xmas 
//tree up and running
//
void Delay(unsigned int val)
{
    volatile unsigned int temp = val;
    while(temp > 0)
        temp--;
}



