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

//Clock control registers
#define CLK_DIVR		    *((volatile uint8_t*)0x50C0)    //master divide register
#define CLK_CRTCR		    *((volatile uint8_t*)0x50C1)    //RTC register
#define CLK_ICKR		    *((volatile uint8_t*)0x50C2)    //internal clock control
#define CLK_PCKENR1		    *((volatile uint8_t*)0x50C3)    //peripheral gating
#define CLK_PCKENR2		    *((volatile uint8_t*)0x50C4)    //peripheral gating
#define CLK_CCOR		    *((volatile uint8_t*)0x50C5)    //configurable clock control
#define CLK_ECKR		    *((volatile uint8_t*)0x50C6)    //external clock control
#define CLK_SCSR		    *((volatile uint8_t*)0x50C7)    //system clock status
#define CLK_SWR		        *((volatile uint8_t*)0x50C8)    //system clock switch
#define CLK_SWCR		    *((volatile uint8_t*)0x50C9)    //clock switch control
#define CLK_CSSR		    *((volatile uint8_t*)0x50CA)    //clock security system
#define CLK_CBEEPR		    *((volatile uint8_t*)0x50CB)    //clock beep reg
#define CLK_HSICALR		    *((volatile uint8_t*)0x50CC)    //HSI cal
#define CLK_HSITRIMR		*((volatile uint8_t*)0x50CD)    //HSI cal
#define CLK_HSIUNLCKR		*((volatile uint8_t*)0x50CE)    //HSI unlock
#define CLK_REGCSR		    *((volatile uint8_t*)0x50CF)    //main regulator control




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

//SPI
#define SPI1_BSY_FLAG	(1u << 7)	//busy = high
#define SPI1_TXE_FLAG	(1u << 1)	//tx empty = high
#define SPI1_RXNE_FLAG	(1u << 0)	//rx not empty = high

#define SPI1_CR1         *((volatile uint8_t*)0x5200)
#define SPI1_CR2         *((volatile uint8_t*)0x5201)
#define SPI1_ICR         *((volatile uint8_t*)0x5202)
#define SPI1_SR          *((volatile uint8_t*)0x5203)
#define SPI1_DR          *((volatile uint8_t*)0x5204)
#define SPI1_CRCPR       *((volatile uint8_t*)0x5205)
#define SPI1_RXCRCR      *((volatile uint8_t*)0x5206)
#define SPI1_TXCRCR      *((volatile uint8_t*)0x5207)



//Comparator Control and Status - use for VREF
#define COMP_CSR1       *((volatile uint8_t*)0x5440)
#define COMP_CSR2       *((volatile uint8_t*)0x5441)
#define COMP_CSR3       *((volatile uint8_t*)0x5442)
#define COMP_CSR4       *((volatile uint8_t*)0x5443)
#define COMP_CSR5       *((volatile uint8_t*)0x5444)

//IO Switch and Analog Switch Registers
#define RI_ICR1        *((volatile uint8_t*)0x5431)    //timer input capture routing 1
#define RI_ICR2        *((volatile uint8_t*)0x5432)    //timer input capture routing 2
#define RI_IOIR1       *((volatile uint8_t*)0x5433)    //IO input register 1
#define RI_IOIR2       *((volatile uint8_t*)0x5434)    //IO input register 2
#define RI_IOIR3       *((volatile uint8_t*)0x5435)    //IO input register 3
#define RI_IOCMR1      *((volatile uint8_t*)0x5436)    //IO control mode register 1
#define RI_IOCMR2      *((volatile uint8_t*)0x5437)    //IO control mode register 2
#define RI_IOCMR3      *((volatile uint8_t*)0x5438)    //IO control mode register 3
#define RI_IOSR1       *((volatile uint8_t*)0x5439)    //IO switch registser 1
#define RI_IOSR2       *((volatile uint8_t*)0x543A)    //IO switch registser 2
#define RI_IOSR3       *((volatile uint8_t*)0x543B)    //IO switch registser 3
#define RI_IOGCR       *((volatile uint8_t*)0x543C)    //IO group control register
#define RI_ASCR1       *((volatile uint8_t*)0x543D)    //analog switch register 1
#define RI_ASCR2       *((volatile uint8_t*)0x543E)    //analog switch register 2
#define RI_RCR         *((volatile uint8_t*)0x543F)    //resistor control register 1





#define RI_IOSR1        *((volatile uint8_t*)0x5439)    //io switch control
#define RI_IOSR2        *((volatile uint8_t*)0x543A)
#define RI_IOSR3        *((volatile uint8_t*)0x543B)
#define RI_IOGCR        *((volatile uint8_t*)0x543C)    //io group control
#define RI_ASCR1        *((volatile uint8_t*)0x543D)    //analog switch control
#define RI_ASCR2        *((volatile uint8_t*)0x543E)

//DAC
#define DAC_CR1         *((volatile uint8_t*)0x5380)
#define DAC_CR2         *((volatile uint8_t*)0x5381)
#define DAC_SWTRIGR     *((volatile uint8_t*)0x5384)
#define DAC_SR          *((volatile uint8_t*)0x5385)

#define DAC_RDHRH       *((volatile uint8_t*)0x5388)
#define DAC_RDHRL       *((volatile uint8_t*)0x5389)

#define DAC_LDHRH       *((volatile uint8_t*)0x538C)
#define DAC_LDHRL       *((volatile uint8_t*)0x538D)

#define DAC_DHR8        *((volatile uint8_t*)0x5390)

#define DAC_DORH        *((volatile uint8_t*)0x53AC)
#define DAC_DORL        *((volatile uint8_t*)0x53AD)

#define DAC_BIT_CR1_TSEL0       BIT_3
#define DAC_BIT_CR1_TSEL1       BIT_4
#define DAC_BIT_CR1_TSEL2       BIT_5

#define DAC_BIT_CR1_TEN         BIT_2
#define DAC_BIT_CR1_BOFF        BIT_1
#define DAC_BIT_CR1_EN          BIT_0

#define DAC_BIT_SWTRIGR_SWTRIG1     BIT_0







//timers - TIM2 - not all registers included
#define TIM2_CR1        *((volatile uint8_t*)0x5250)
#define TIM2_CR2        *((volatile uint8_t*)0x5251)
#define TIM2_SMCR       *((volatile uint8_t*)0x5252)
#define TIM2_ETR        *((volatile uint8_t*)0x5253)
#define TIM2_DER        *((volatile uint8_t*)0x5254)
#define TIM2_IER        *((volatile uint8_t*)0x5255)
#define TIM2_SR1        *((volatile uint8_t*)0x5256)
#define TIM2_SR2        *((volatile uint8_t*)0x5257)
#define TIM2_EGR        *((volatile uint8_t*)0x5258)
#define TIM2_CCMR1      *((volatile uint8_t*)0x5259)
#define TIM2_CCMR2      *((volatile uint8_t*)0x525A)
#define TIM2_CCER1      *((volatile uint8_t*)0x525B)
#define TIM2_CNTRH      *((volatile uint8_t*)0x525C)
#define TIM2_CNTRL      *((volatile uint8_t*)0x525D)
#define TIM2_PSCR       *((volatile uint8_t*)0x525E)
#define TIM2_ARRH       *((volatile uint8_t*)0x525F)
#define TIM2_ARRL       *((volatile uint8_t*)0x5260)
#define TIM2_CCR1H      *((volatile uint8_t*)0x5261)
#define TIM2_CCR1L      *((volatile uint8_t*)0x5262)
#define TIM2_CCR2H      *((volatile uint8_t*)0x5263)
#define TIM2_CCR2L      *((volatile uint8_t*)0x5264)
#define TIM2_BKR        *((volatile uint8_t*)0x5265)
#define TIM2_OISR       *((volatile uint8_t*)0x5266)

//timers - TIM4
#define TIM4_CR1        *((volatile uint8_t*)0x52E0)
#define TIM4_CR2        *((volatile uint8_t*)0x52E1)
#define TIM4_SMCR       *((volatile uint8_t*)0x52E2)
#define TIM4_DER        *((volatile uint8_t*)0x52E3)
#define TIM4_IER        *((volatile uint8_t*)0x52E4)
#define TIM4_SR1        *((volatile uint8_t*)0x52E5)
#define TIM4_EGR        *((volatile uint8_t*)0x52E6)
#define TIM4_CNTR       *((volatile uint8_t*)0x52E7)
#define TIM4_PSCR       *((volatile uint8_t*)0x52E8)
#define TIM4_ARR        *((volatile uint8_t*)0x52E9)


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

