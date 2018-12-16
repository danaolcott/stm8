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
#ifndef __GPIO_H_
#define __GPIO_H_

#include <stddef.h>
#include <stdint.h>


#define BIT0    (1 << 0)
#define BIT1    (1 << 1)
#define BIT2    (1 << 2)
#define BIT3    (1 << 3)
#define BIT4    (1 << 4)
#define BIT5    (1 << 5)
#define BIT6    (1 << 6)
#define BIT7    (1 << 7)

typedef enum
{
    GPIO_PIN_0 = 0,
    GPIO_PIN_1 = 1,
    GPIO_PIN_2 = 2,
    GPIO_PIN_3 = 3,
    GPIO_PIN_4 = 4,
    GPIO_PIN_5 = 5,
    GPIO_PIN_6 = 6,
    GPIO_PIN_7 = 7
}GPIO_Pin_t;

typedef enum
{
    GPIO_PORT_A,
    GPIO_PORT_B,
    GPIO_PORT_C,
    GPIO_PORT_D
}GPIO_Port_t;

typedef enum
{
    GPIO_STATE_LOW = 0,
    GPIO_STATE_HIGH = 1,
    GPIO_STATE_TOGGLE = 2
}GPIO_State_t;



void GPIO_Write(GPIO_Port_t port, GPIO_Pin_t pin, GPIO_State_t state);
GPIO_State_t GPIO_Read(GPIO_Port_t port, GPIO_Pin_t pin);

#endif
