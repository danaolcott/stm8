/* 
GPIO Controller File - STM8S103F3P6 Processor
Hardware Layout:
Pins Available
PA1 - PA3
PB4 - PB5
PC3 - PC7
PD1 - PD6

User LED located on PB5

Register Descriptions:
PB_ODR - output data register
PB_IDR - input data register
PB_DDR - data direction
PB_CR1 - control reg 1
PB_CR2 - control reg 2

See Table 21 - Page 107 in the reference manual 
 
 */
#include <stddef.h>
#include <stdint.h>
#include <iostm8s103.h>
#include "gpio.h"



////////////////////////////////////////////
//Generic GPIO Write Function
//Assumes the pins are properly configured,
//no checks to verify the pins are actually available
void GPIO_Write(GPIO_Port_t port, GPIO_Pin_t pin, GPIO_State_t state)
{
    volatile char* ptr_write = &PA_ODR;
    volatile char* ptr_read = &PA_IDR;
    volatile uint8_t current = 0x00;
        
    //set the read / write pointers
    switch(port)
    {
        case GPIO_PORT_A:     ptr_write = &PA_ODR;     ptr_read = &PA_IDR;      break;
        case GPIO_PORT_B:     ptr_write = &PB_ODR;     ptr_read = &PB_IDR;      break;
        case GPIO_PORT_C:     ptr_write = &PC_ODR;     ptr_read = &PC_IDR;      break;
        case GPIO_PORT_D:     ptr_write = &PD_ODR;     ptr_read = &PD_IDR;      break;
    }
    
    if (state == GPIO_STATE_LOW)
        *ptr_write &=~ (1u << pin);
    else if (state == GPIO_STATE_HIGH)
        *ptr_write |= (1u << pin);
    else if (state == GPIO_STATE_TOGGLE)
        *ptr_write ^= (1u << pin);


/*
    current = *ptr_read;                //read current
    
    if (state == GPIO_STATE_LOW)
        current &=~ (1u << pin);        //update
    else if (state == GPIO_STATE_HIGH)
        current |= (1u << pin);         //update
    else if (state == GPIO_STATE_TOGGLE)
        current ^= (1u << pin);
        
    *ptr_write = current;               //write
  */
  
}


GPIO_State_t GPIO_Read(GPIO_Port_t port, GPIO_Pin_t pin)
{
    volatile char* ptr_read = &PA_IDR;    
    uint8_t current = 0x00;
    uint8_t bitValue = 0x00;
    uint8_t pinMask = 1u << pin;
        
    //set the read / write pointers
    switch(port)
    {
        case GPIO_PORT_A:     ptr_read = &PA_IDR;      break;
        case GPIO_PORT_B:     ptr_read = &PB_IDR;      break;
        case GPIO_PORT_C:     ptr_read = &PC_IDR;      break;
        case GPIO_PORT_D:     ptr_read = &PD_IDR;      break;
    }
    
    current = (*ptr_read & pinMask) >> pin;
    
    return current;        
}



