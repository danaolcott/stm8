/*	BASIC INTERRUPT VECTOR TABLE FOR STM8 devices
 *	Copyright (c) 2007 STMicroelectronics
 */
 
#include "stm8l15x_it.h"
#include "main.h"
#include "gpio.h"
#include "timer.h"



typedef void @far (*interrupt_handler_t)(void);


//interrupt prototypes
@far @interrupt void TRAP_IRQHandler(void)
{
    return;
}


@far @interrupt void FLASH_IRQHandler(void)
{
    return;
}

@far @interrupt void DMA1_CHANNEL0_1_IRQHandler(void){return;} /* DMA1 Channel0/1*/
@far @interrupt void DMA1_CHANNEL2_3_IRQHandler(void){return;} /*DMA1 Channel2/3*/
@far @interrupt void RTC_CSSLSE_IRQHandler(void){return;} /* RTC /CSS_LSE */
@far @interrupt void EXTIE_F_PVD_IRQHandler(void){return;} /*EXTI PORTE/EXTI PORTF/PVD*/
@far @interrupt void EXTIB_G_IRQHandler(void){return;} /* EXTI PORTB / EXTI PORTG */
@far @interrupt void EXTID_H_IRQHandler(void){return;} /* EXTI PORTD / EXTI PORTH*/
@far @interrupt void EXTI0_IRQHandler(void){return;} /* EXTI PIN0 */

/////////////////////////////////////////////////////////////
//EXTI 1
@far @interrupt void EXTI1_IRQHandler(void)
{
    exti1_isr();    
    return;
}

@far @interrupt void EXTI2_IRQHandler(void){return;} /* EXTI PIN2 */
@far @interrupt void EXTI3_IRQHandler(void){return;} /* EXTI PIN3 */
@far @interrupt void EXTI4_IRQHandler(void){return;} /* EXTI PIN4 */

//////////////////////////////////////////////////////////
//User button interrupt
@far @interrupt void EXTI5_IRQHandler(void)
{
    exti5_isr();    
    return;
}



@far @interrupt void EXTI6_IRQHandler(void){return;} /* EXTI PIN6 */
@far @interrupt void EXTI7_IRQHandler(void){return;} /* EXTI PIN7 */
@far @interrupt void LCD_AES_IRQHandler(void){return;} /* LCD /AES */
@far @interrupt void SWITCH_CSS_BREAK_DAC_IRQHandler(void){return;} /* Switch CLK/CSS/TIM1 Break/DAC */
@far @interrupt void ADC1_COMP_IRQHandler(void){return;} /*ADC1/COMP*/

/* TIM2 UPD/OVF/TRG/BRK / USART2 TX */
//Timer 2 Interrupt
@far @interrupt void TIM2_UPD_OVF_TRG_BRK_USART2_TX_IRQHandler(void)
{    
    tim2_isr();
    return;
}


@far @interrupt void TIM2_CC_USART2_RX_IRQHandler(void){return;} /* TIM2 CAP / USART2 RX */
@far @interrupt void TIM3_UPD_OVF_TRG_BRK_USART3_TX_IRQHandler(void){return;} /* TIM3 UPD/OVF/TRG/BRK /USART3 TX*/
@far @interrupt void TIM3_CC_USART3_RX_IRQHandler(void){return;} /* TIM3 CAP/ USART3 RX */
@far @interrupt void TIM1_UPD_OVF_TRG_COM_IRQHandler(void){return;}/* TIM1 UPD/OVF/TRG/COM */
@far @interrupt void TIM1_CC_IRQHandler(void){return;}/* TIM1 CAP*/
@far @interrupt void TIM4_UPD_OVF_TRG_IRQHandler(void){return;} /* TIM4 UPD/OVF/TRI */
@far @interrupt void SPI1_IRQHandler(void){return;} /* SPI1 */
@far @interrupt void USART1_TX_TIM5_UPD_OVF_TRG_BRK_IRQHandler(void){return;} /* USART1 TX / TIM5 UPD/OVF/TRG/BRK */
@far @interrupt void USART1_RX_TIM5_CC_IRQHandler(void){return;} /* USART1 RX / TIM5 CAP */
@far @interrupt void I2C1_SPI2_IRQHandler(void){return;} /* I2C1 / SPI2 */


struct interrupt_vector {
	unsigned char interrupt_instruction;
	interrupt_handler_t interrupt_handler;
};

@far @interrupt void NonHandledInterrupt (void)
{
	/* in order to detect unexpected events during development, 
	   it is recommended to set a breakpoint on the following instruction
	*/
	return;
}

extern void _stext();     /* startup routine */

struct interrupt_vector const _vectab[] = {
  {0x82, (interrupt_handler_t)_stext},		/* Reset */
  {0x82, (interrupt_handler_t)TRAP_IRQHandler},			/* TRAP - Software interrupt */
  {0x82, (interrupt_handler_t)NonHandledInterrupt},		/* Reserved */
  {0x82, (interrupt_handler_t)FLASH_IRQHandler},			/* irq1 - FLASH interrupt */
  {0x82, (interrupt_handler_t)DMA1_CHANNEL0_1_IRQHandler},			/* irq2 - DMA1 channel0 and channel1 interrupt */
	{0x82, (interrupt_handler_t)DMA1_CHANNEL2_3_IRQHandler},			/* irq3 - DMA1 channel2 and channel3 interrupt */
  {0x82, (interrupt_handler_t)RTC_CSSLSE_IRQHandler},			/* irq4 - RTC/ CSS on LSE interrupt */
  {0x82, (interrupt_handler_t)EXTIE_F_PVD_IRQHandler},    /* irq5 - External IT PORTE/F interrupt /PVD interrupt*/
  {0x82, (interrupt_handler_t)EXTIB_G_IRQHandler},      /* irq6 - External IT PORTB / PORTG interrupt */
  {0x82, (interrupt_handler_t)EXTID_H_IRQHandler},			/* irq7 - External IT PORTD / PORTH interrupt */
  {0x82, (interrupt_handler_t)EXTI0_IRQHandler},			/* irq8 - External IT PIN0 interrupt */
  {0x82, (interrupt_handler_t)EXTI1_IRQHandler},			/* irq9 - External IT PIN1 interrupt */
  {0x82, (interrupt_handler_t)EXTI2_IRQHandler},			/* irq10 - External IT PIN2 interrupt */
  {0x82, (interrupt_handler_t)EXTI3_IRQHandler},		  /* irq11 - External IT PIN3 interrupt */
  {0x82, (interrupt_handler_t)EXTI4_IRQHandler},      /* irq12 - External IT PIN4 interrupt */
  {0x82, (interrupt_handler_t)EXTI5_IRQHandler},      /* irq13 - External IT PIN5 interrupt */
  {0x82, (interrupt_handler_t)EXTI6_IRQHandler},      /* irq14 - External IT PIN6 interrupt */
  {0x82, (interrupt_handler_t)EXTI7_IRQHandler},      /* irq15 - External IT PIN7 interrupt */
  {0x82, (interrupt_handler_t)LCD_AES_IRQHandler},      /* irq16 - LCD / AES interrupt */
  {0x82, (interrupt_handler_t)SWITCH_CSS_BREAK_DAC_IRQHandler},      /* irq17 - CLK switch/CSS interrupt/ TIM1 Break interrupt / DAC */
  {0x82, (interrupt_handler_t)ADC1_COMP_IRQHandler},      /* irq18 - ADC1 and Comparator interrupt */
  {0x82, (interrupt_handler_t)TIM2_UPD_OVF_TRG_BRK_USART2_TX_IRQHandler},      /* irq19 - TIM2 Update/Overflow/Trigger/Break / USART2 TX interrupt */
  {0x82, (interrupt_handler_t)TIM2_CC_USART2_RX_IRQHandler},      /* irq20 - TIM2 Capture/Compare / USART2 RX interrupt */
  {0x82, (interrupt_handler_t)TIM3_UPD_OVF_TRG_BRK_USART3_TX_IRQHandler},      /* irq21 - TIM3 Update/Overflow/Trigger/Break / USART3 TX interrupt */
  {0x82, (interrupt_handler_t)TIM3_CC_USART3_RX_IRQHandler},			/* irq22 - TIM3 Capture/Compare /USART3 RX interrupt */
  {0x82, (interrupt_handler_t)TIM1_UPD_OVF_TRG_COM_IRQHandler},      /* irq23 - TIM1 Update/Overflow/Trigger/Commutation interrupt */
  {0x82, (interrupt_handler_t)TIM1_CC_IRQHandler},      /* irq24 - TIM1 Capture/Compare interrupt */
  {0x82, (interrupt_handler_t)TIM4_UPD_OVF_TRG_IRQHandler},     /* irq25 - TIM4 Update/Overflow/Trigger interrupt */
  {0x82, (interrupt_handler_t)SPI1_IRQHandler},      /* irq26 - SPI1 interrupt */
  {0x82, (interrupt_handler_t)USART1_TX_TIM5_UPD_OVF_TRG_BRK_IRQHandler},      /* irq27 - USART1 TX / TIM5 Update/Overflow/Trigger/Break interrupt */
  {0x82, (interrupt_handler_t)USART1_RX_TIM5_CC_IRQHandler},      /* irq28 - USART1 RX / TIM1 Capture/Compare interrupt */
  {0x82, (interrupt_handler_t)I2C1_SPI2_IRQHandler},      /* irq29 - I2C1 / SPI2 interrupt */

};

