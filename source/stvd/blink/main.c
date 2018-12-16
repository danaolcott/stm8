/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 */
 
/*
Blink Project Using the STM8S103F3P6 Processor
Cheap board from ebay.
Pins Available:
PA1 - PA3
PB4 - PB5
PC3 - PC7
PD1 - PD6

Register Descriptions:
PB_ODR - output data register
PB_IDR - input data register
PB_DDR - data direction
PB_CR1 - control reg 1
PB_CR2 - control reg 2

See Table 21 - Page 107 in the ref manual
For output PP, fast mode, config ODR = 1, CR1 = 1, CR2 = 1


PB5 - LED
PB4 - Button with pullup
 
 */
#include <stddef.h>
#include <stdint.h>
#include <iostm8s103.h>

#include "gpio.h"


#define BIT0    (1 << 0)
#define BIT1    (1 << 1)
#define BIT2    (1 << 2)
#define BIT3    (1 << 3)
#define BIT4    (1 << 4)
#define BIT5    (1 << 5)
#define BIT6    (1 << 6)
#define BIT7    (1 << 7)



void Delay(uint32_t temp);
void GPIO_Init(void);


main()
{
    GPIO_Init();
    
    while (1)
    {
        //read the button - low
        //if low, toggle, else, hold high
        if (!GPIO_Read(GPIO_PORT_B, GPIO_PIN_4))
            GPIO_Write(GPIO_PORT_B, GPIO_PIN_5, GPIO_STATE_TOGGLE);
        else
            GPIO_Write(GPIO_PORT_B, GPIO_PIN_5, GPIO_STATE_HIGH);
        Delay(5000);
    }

}


void Delay(uint32_t temp)
{
    volatile uint32_t delay = temp;
    while (delay > 0)
        delay--;
}

//Configure PA1, PA2, PA3 as output
//See Table 21, Page 107 ref manual
void GPIO_Init(void)
{
    PA_DDR = BIT1 | BIT2 | BIT3;		//high output
    PA_CR1 = BIT1 | BIT2 | BIT3;		//CR1 high output fast PP
    PA_CR2 = BIT1 | BIT2 | BIT3;		//CR2 high output fast PP

    //PB5 - output
    PB_DDR = BIT5;                      //led
    PB_CR1 = BIT5;
    PB_CR2 = BIT5;
    
    //PB4 - input, pullup - PB_CR1 = 1 for pullup, PB_DDR = 0 input
    PB_DDR &=~ BIT4;        //input
    PB_CR1 |= BIT4;         //pullup
    PB_CR2 &=~ BIT4;        //interrupt disabled
    
    
}


