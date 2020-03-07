/*
3/5/20
Register Definitions for the STM8L151K6

*/

#ifndef __REGISTER_H
#define __REGISTER_H

#include <stdint.h>
#include <stddef.h>
#include "register.h"       //irq definitions

#define BIT_0           (1u << 0)
#define BIT_1           (1u << 1)
#define BIT_2           (1u << 2)
#define BIT_3           (1u << 3)
#define BIT_4           (1u << 4)
#define BIT_5           (1u << 5)
#define BIT_6           (1u << 6)
#define BIT_7           (1u << 7)

/*
//CPU registers
#define CPU_A           *((volatile uint8_t*)0x7F00)
#define CPU_PCE         *((volatile uint8_t*)0x7F01)
#define CPU_PCH         *((volatile uint8_t*)0x7F02)
#define CPU_PCL         *((volatile uint8_t*)0x7F03)
#define CPU_XH          *((volatile uint8_t*)0x7F04)
#define CPU_XL          *((volatile uint8_t*)0x7F05)

#define CPU_YH          *((volatile uint8_t*)0x7F06)
#define CPU_YL          *((volatile uint8_t*)0x7F07)

#define CPU_SPH         *((volatile uint8_t*)0x7F08)
#define CPU_SPL         *((volatile uint8_t*)0x7F09)

#define CPU_CCR         *((volatile uint8_t*)0x7F0A)
#define CPU_CFG_GCR     *((volatile uint8_t*)0x7F60)


#define CPU_CCR_BIT_I1      (1u << 5)
#define CPU_CCR_BIT_I0      (1u << 3)

*/


//System Config Registers
#define SYSCFG_RMPCR1		*((volatile uint8_t*)0x509E)
#define SYSCFG_RMPCR2		*((volatile uint8_t*)0x509F)




//GPIO Registers - PortA - PortF
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

#define PE_ODR          *((volatile uint8_t*)0x5014)
#define PE_IDR          *((volatile uint8_t*)0x5015)
#define PE_DDR          *((volatile uint8_t*)0x5016)
#define PE_CR1          *((volatile uint8_t*)0x5017)
#define PE_CR2          *((volatile uint8_t*)0x5018)

#define PF_ODR          *((volatile uint8_t*)0x5019)
#define PF_IDR          *((volatile uint8_t*)0x501A)
#define PF_DDR          *((volatile uint8_t*)0x501B)
#define PF_CR1          *((volatile uint8_t*)0x501C)
#define PF_CR2          *((volatile uint8_t*)0x501D)

//External Interrupts - PortA - PortE
#define EXTI_CR1		*((volatile uint8_t*)0x50A0)
#define EXTI_CR2		*((volatile uint8_t*)0x50A1)
#define EXTI_CR3		*((volatile uint8_t*)0x50A2)
#define EXTI_SR1		*((volatile uint8_t*)0x50A3)
#define EXTI_SR2		*((volatile uint8_t*)0x50A4)
#define EXTI_CONF1		*((volatile uint8_t*)0x50A5)


//interrupt IRQ - See Table 10 in the datasheet
#define EXTI0_IRQ       8
#define EXTI1_IRQ       9
#define EXTI2_IRQ       10
#define EXTI3_IRQ       11
#define EXTI4_IRQ       12
#define EXTI5_IRQ       13
#define EXTI6_IRQ       14
#define EXTI7_IRQ       15





#endif

