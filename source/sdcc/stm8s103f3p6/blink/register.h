/*
Dana Olcott
12/12/18

Register Definitions

*/

#ifndef __REGISTER_H
#define __REGISTER_H

#include <stdint.h>
#include <stddef.h>


#define BIT_0           (1u << 0)
#define BIT_1           (1u << 1)
#define BIT_2           (1u << 2)
#define BIT_3           (1u << 3)
#define BIT_4           (1u << 4)
#define BIT_5           (1u << 5)
#define BIT_6           (1u << 6)
#define BIT_7           (1u << 7)

#define PA_ODR          *((volatile uint8_t*)0x5000)
#define PA_IDR          *((volatile uint8_t*)0x5001)
#define PA_DDR          *((volatile uint8_t*)0x5002)
#define PA_CR1          *((volatile uint8_t*)0x5003)
#define PA_CR2          *((volatile uint8_t*)0x5004)

#define PB_ODR          *((volatile uint8_t*)0x5005)
#define PB_IDR          *((volatile uint8_t*)0x5006)
#define PB_DDR          *((volatile uint8_t*)0x5007)
#define PB_CR1          *((volatile uint8_t*)0x5008)
#define PB_CR2          *((volatile uint8_t*)0x5009)

#define PC_ODR          *((volatile uint8_t*)0x500A)
#define PC_IDR          *((volatile uint8_t*)0x500B)
#define PC_DDR          *((volatile uint8_t*)0x500C)
#define PC_CR1          *((volatile uint8_t*)0x500D)
#define PC_CR2          *((volatile uint8_t*)0x500E)

#define PD_ODR          *((volatile uint8_t*)0x500F)
#define PD_IDR          *((volatile uint8_t*)0x5010)
#define PD_DDR          *((volatile uint8_t*)0x5011)
#define PD_CR1          *((volatile uint8_t*)0x5012)
#define PD_CR2          *((volatile uint8_t*)0x5013)

//Timer4 - Simplest one

#define ARPE_BIT        (1u << 7)
#define OPM_BIT         (1u << 3)
#define URS_BIT         (1u << 2)
#define UDIS_BIT        (1u << 1)
#define CEN_BIT         (1u << 0)
#define TIE_BIT         (1u << 6)
#define UIE_BIT         (1u << 0)
#define TIF_BIT         (1u << 6)
#define UIF_BIT         (1u << 0)




#define TIM4_CR1        *((volatile uint8_t*)0x5340)
#define TIM4_IER        *((volatile uint8_t*)0x5343)
#define TIM4_SR         *((volatile uint8_t*)0x5344)
#define TIM4_EGR        *((volatile uint8_t*)0x5345)
#define TIM4_CNTR       *((volatile uint8_t*)0x5346)
#define TIM4_PSCR       *((volatile uint8_t*)0x5347)
#define TIM4_ARR        *((volatile uint8_t*)0x5348)


//interrupt IRQ - See Table 10 in the datasheet
#define TIM1_UPDATE_IRQ     11
#define TIM1_COMPARE_IRQ    12
#define TIM2_UPDATE_IRQ     13
#define TIM2_COMPARE_IRQ    14

#define TIM4_IRQ        23

#define EXTI0_IRQ       3
#define EXTI1_IRQ       4
#define EXTI2_IRQ       5
#define EXTI3_IRQ       6

#define PORTA_IRQ       3
#define PORTB_IRQ       4
#define PORTC_IRQ       5
#define PORTD_IRQ       6


#endif

