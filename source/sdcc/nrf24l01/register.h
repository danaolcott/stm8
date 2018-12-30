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

//External Interrupts - PortA - PortE
#define EXTI_CR1		*((volatile uint8_t*)0x50A0)
#define EXTI_CR2		*((volatile uint8_t*)0x50A1)

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


//Timer2 - General 16 bit timer
#define TIM2_CR1        *((volatile uint8_t*)0x5300)
#define TIM2_IER        *((volatile uint8_t*)0x5303)
#define TIM2_SR1        *((volatile uint8_t*)0x5304)
#define TIM2_SR2        *((volatile uint8_t*)0x5305)
#define TIM2_EGR        *((volatile uint8_t*)0x5306)

#define TIM2_CNTRH      *((volatile uint8_t*)0x530C)
#define TIM2_CNTRL      *((volatile uint8_t*)0x530D)
#define TIM2_PSCR       *((volatile uint8_t*)0x530E)
#define TIM2_ARRH       *((volatile uint8_t*)0x530F)
#define TIM2_ARRL       *((volatile uint8_t*)0x5310)


//SPI
#define SPI_BSY_FLAG	(1u << 7)	//busy = high
#define SPI_TXE_FLAG	(1u << 1)	//tx empty = high
#define SPI_RXNE_FLAG	(1u << 0)	//rx not empty = high

#define SPI_CR1         *((volatile uint8_t*)0x5200)
#define SPI_CR2         *((volatile uint8_t*)0x5201)
#define SPI_ICR         *((volatile uint8_t*)0x5202)
#define SPI_SR          *((volatile uint8_t*)0x5203)
#define SPI_DR          *((volatile uint8_t*)0x5204)
#define SPI_CRCPR       *((volatile uint8_t*)0x5205)
#define SPI_RXCRCR      *((volatile uint8_t*)0x5206)
#define SPI_TXCRCR      *((volatile uint8_t*)0x5207)

//EEPROM
#define EEPROM_BASE_ADDRESS     ((uint16_t)0x4000)
#define FLASH_DUL_BIT           (1u << 3)
#define FLASH_EOP_BIT           (1u << 2)
#define FLASH_WR_PG_DIS_BIT     (1u << 0)

#define FLASH_IAPSR             *((volatile uint8_t*)0x505F)
#define FLASH_DUKR              *((volatile uint8_t*)0x5064)


//Clock Configuration
#define CLK_CCOEN_BIT       (1u << 0)
#define CLK_CCORDY_BIT      (1u << 5)
#define CLK_CCOBSY_BIT      (1u << 6)

#define CLK_ICKR        *((volatile uint8_t*)0x50C0)
#define CLK_ECKR        *((volatile uint8_t*)0x50C1)
#define CLK_CMSR        *((volatile uint8_t*)0x50C3)
#define CLK_SWR         *((volatile uint8_t*)0x50C4)
#define CLK_SWCR        *((volatile uint8_t*)0x50C5)
#define CLK_CKDIVR      *((volatile uint8_t*)0x50C6)
#define CLK_PCKENR1     *((volatile uint8_t*)0x50C7)
#define CLK_CSSR        *((volatile uint8_t*)0x50C8)
#define CLK_CCOR        *((volatile uint8_t*)0x50C9)
#define CLK_PCKENR2     *((volatile uint8_t*)0x50CA)
#define CLK_HSITRIMR    *((volatile uint8_t*)0x50CC)
#define CLK_SWIMCCR     *((volatile uint8_t*)0x50CD)


//UART1 Configuration
#define UART1_TXE_BIT	(1u << 7)		//use with the UART1_SR
#define UART1_TC_BIT	(1u << 6)
#define UART1_RXNE_BIT	(1u << 5)

#define UART1_CLKEN_BIT (1u << 3)
#define UART1_CPOL_BIT  (1u << 2)
#define UART1_CPHA_BIT  (1u << 1)
#define UART1_LBCL_BIT  (1u << 0)

//UART1_CR2
#define UART1_RIEN_BIT	(1u << 5)		//rx interrupt enable
#define UART1_TEN_BIT	(1u << 3)		//tx enable
#define UART1_REN_BIT 	(1u << 2)		//rx enable

#define USART_CR2_TEN       (1 << 3)
#define USART_CR3_STOP2     (1 << 5)
#define USART_CR3_STOP1     (1 << 4)
#define USART_SR_TXE        (1 << 7)

#define UART1_SR 		*((volatile uint8_t*)0x5230)
#define UART1_DR 		*((volatile uint8_t*)0x5231)
#define UART1_BRR1 		*((volatile uint8_t*)0x5232)
#define UART1_BRR2 		*((volatile uint8_t*)0x5233)
#define UART1_CR1 		*((volatile uint8_t*)0x5234)
#define UART1_CR2 		*((volatile uint8_t*)0x5235)
#define UART1_CR3 		*((volatile uint8_t*)0x5236)
#define UART1_CR4 		*((volatile uint8_t*)0x5237)
#define UART1_CR5 		*((volatile uint8_t*)0x5238)
#define UART1_GTR 		*((volatile uint8_t*)0x5239)
#define UART1_PSCR 		*((volatile uint8_t*)0x523A)


//ADC Registers
#define ADC_EOC_BIT     (1u << 7)
#define ADC_ADON_BIT    (1u << 0)


#define ADC_CSR 		*((volatile uint8_t*)0x5400)
#define ADC_CR1 		*((volatile uint8_t*)0x5401)
#define ADC_CR2 		*((volatile uint8_t*)0x5402)
#define ADC_CR3 		*((volatile uint8_t*)0x5403)
#define ADC_DRH 		*((volatile uint8_t*)0x5404)
#define ADC_DRL 		*((volatile uint8_t*)0x5405)








//interrupt IRQ - See Table 10 in the datasheet
#define EXTI0_IRQ       3
#define EXTI1_IRQ       4
#define EXTI2_IRQ       5
#define EXTI3_IRQ       6

#define PORTA_IRQ       3
#define PORTB_IRQ       4
#define PORTC_IRQ       5
#define PORTD_IRQ       6

#define TIM1_UPDATE_IRQ     11
#define TIM1_COMPARE_IRQ    12
#define TIM2_UPDATE_IRQ     13
#define TIM2_COMPARE_IRQ    14

#define UART1_TXE_IRQ		17
#define UART1_RXNE_IRQ		18

#define TIM4_IRQ        23



#endif

