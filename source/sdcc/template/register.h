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

