/*	BASIC INTERRUPT VECTOR TABLE FOR STM8 devices
 *	Copyright (c) 2007 STMicroelectronics
 */

#include "gpio.h"
#include "register.h"
#include "timer.h"
#include "uart.h"
#include "nrf24l01.h"

typedef void @far (*interrupt_handler_t)(void);


///////////////////////////////////////////////////
//Interrupt Prototypes
@far @interrupt void EXTI0_InterruptHandler (void);     //IRQ 3
@far @interrupt void EXTI1_InterruptHandler (void);     //IRQ 4
@far @interrupt void Timer2_InterruptHandler (void);    //IRQ 13 - update
@far @interrupt void Timer4_InterruptHandler (void);    //IRQ 23
@svlreg @far @interrupt void UART_InterruptHandler (void);      //IRQ 18 - RXNE



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






////////////////////////////////////////////////
//Port A EXTI Interrupt
@far @interrupt void EXTI0_InterruptHandler (void)
{
    nrf24_ISR();
    return;
}

/////////////////////////////////////////////////
//Port B EXTI Interrupt
@far @interrupt void EXTI1_InterruptHandler (void)
{
    EXTI1_ISR();        
    return;
}



//////////////////////////////////////////////////
//Timer 2 Update Interrupt
@far @interrupt void Timer2_InterruptHandler (void)
{
    Timer2_ISR();
    TIM2_SR1 &=~ UIF_BIT;	//Clear the interrupt flag	
    return;
}

/////////////////////////////////////////////////
//Timer 4 Update Interrupt
@far @interrupt void Timer4_InterruptHandler (void)
{
    Timer4_ISR();
    TIM4_SR &=~ UIF_BIT;	//Clear the interrupt flag	
    return;
}

////////////////////////////////////////////////////
//UART RXNE Interrupt Handler
//For some reason, when the ISR gets long, an 
//error message comes up during the link stage about
//missing @svlreg for UART_InterruptHandler
//
@svlreg @far @interrupt void UART_InterruptHandler (void)
{
    UART_ISR();    
    return;
}




extern void _stext();     /* startup routine */

struct interrupt_vector const _vectab[] = {
	{0x82, (interrupt_handler_t)_stext}, /* reset */
	{0x82, NonHandledInterrupt}, /* trap  */
	{0x82, NonHandledInterrupt}, /* irq0  */
	{0x82, NonHandledInterrupt}, /* irq1  */
	{0x82, NonHandledInterrupt}, /* irq2  */
	{0x82, (interrupt_handler_t)EXTI0_InterruptHandler}, /* irq3  */
	{0x82, (interrupt_handler_t)EXTI1_InterruptHandler}, /* irq4  */
	{0x82, NonHandledInterrupt}, /* irq5  */
	{0x82, NonHandledInterrupt}, /* irq6  */
	{0x82, NonHandledInterrupt}, /* irq7  */
	{0x82, NonHandledInterrupt}, /* irq8  */
	{0x82, NonHandledInterrupt}, /* irq9  */
	{0x82, NonHandledInterrupt}, /* irq10 */
	{0x82, NonHandledInterrupt}, /* irq11 */
	{0x82, NonHandledInterrupt}, /* irq12 */
	{0x82, (interrupt_handler_t)Timer2_InterruptHandler}, /* irq13 */
	{0x82, NonHandledInterrupt}, /* irq14 */
	{0x82, NonHandledInterrupt}, /* irq15 */
	{0x82, NonHandledInterrupt}, /* irq16 */
	{0x82, NonHandledInterrupt}, /* irq17 */
	{0x82, (interrupt_handler_t)UART_InterruptHandler}, /* irq18 */
	{0x82, NonHandledInterrupt}, /* irq19 */
	{0x82, NonHandledInterrupt}, /* irq20 */
	{0x82, NonHandledInterrupt}, /* irq21 */
	{0x82, NonHandledInterrupt}, /* irq22 */
	{0x82, (interrupt_handler_t)Timer4_InterruptHandler}, /* irq23 */
	{0x82, NonHandledInterrupt}, /* irq24 */
	{0x82, NonHandledInterrupt}, /* irq25 */
	{0x82, NonHandledInterrupt}, /* irq26 */
	{0x82, NonHandledInterrupt}, /* irq27 */
	{0x82, NonHandledInterrupt}, /* irq28 */
	{0x82, NonHandledInterrupt}, /* irq29 */
};
