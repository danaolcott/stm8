/*	BASIC INTERRUPT VECTOR TABLE FOR STM8 devices
 *	Copyright (c) 2007 STMicroelectronics
 */


////////////////////////////////////////////////////
//Headers that contain the isr functions
#include "gpio.h"       //isr functions
#include "timer.h"      //timer isr


typedef void @far (*interrupt_handler_t)(void);

////////////////////////////////////////////////////////
//Interrupt Prototypes
//GPIO
@far @interrupt void EXTI0_InterruptHandler (void);     //IRQ8
@far @interrupt void EXTI1_InterruptHandler (void);     //IRQ9
@far @interrupt void EXTI2_InterruptHandler (void);     //IRQ10
@far @interrupt void EXTI3_InterruptHandler (void);     //IRQ11
@far @interrupt void EXTI4_InterruptHandler (void);     //IRQ12
@far @interrupt void EXTI5_InterruptHandler (void);     //IRQ13

//Timer
@far @interrupt void TIM2_InterruptHandler (void);     //IRQ19
@far @interrupt void TIM4_InterruptHandler (void);     //IRQ13



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




////////////////////////////////////////////////////////
//Interrupt Routine Function Definitions

/////////////////////////////////////////////////////
//EXTI0 Interrupt Handler
@far @interrupt void EXTI0_InterruptHandler (void)
{
    //call the handler function
    GPIO_EXTI0_ISR();
    return;
}

/////////////////////////////////////////////////////
//EXTI1 Interrupt Handler
@far @interrupt void EXTI1_InterruptHandler (void)
{
    //call the handler function
    GPIO_EXTI1_ISR();
    return;
}

/////////////////////////////////////////////////////
//EXTI2 Interrupt Handler
@far @interrupt void EXTI2_InterruptHandler (void)
{
    //call the handler function
    GPIO_EXTI2_ISR();
    return;
}

/////////////////////////////////////////////////////
//EXTI3 Interrupt Handler
@far @interrupt void EXTI3_InterruptHandler (void)
{
    //call the handler function
    GPIO_EXTI3_ISR();
    return;
}

/////////////////////////////////////////////////////
//EXTI4 Interrupt Handler
@far @interrupt void EXTI4_InterruptHandler (void)
{
    //call the handler function
    GPIO_EXTI4_ISR();
    return;
}

/////////////////////////////////////////////////////
//EXTI5 Interrupt Handler
@far @interrupt void EXTI5_InterruptHandler (void)
{
    //call the handler function
    GPIO_EXTI5_ISR();
    return;
}


/////////////////////////////////////////////////////
//Timer 2 TIM2 - update interrupt handler
@far @interrupt void TIM2_InterruptHandler (void)
{
    TIM2_ISR();
    return;
}


/////////////////////////////////////////////////////
//Timer 4 TIM4 - update interrupt handler
@far @interrupt void TIM4_InterruptHandler (void)
{
    TIM4_ISR();
    return;
}






extern void _stext();     /* startup routine */

struct interrupt_vector const _vectab[] = {
	{0x82, (interrupt_handler_t)_stext}, /* reset */
	{0x82, NonHandledInterrupt}, /* trap  */
	{0x82, NonHandledInterrupt}, /* irq0  */
	{0x82, NonHandledInterrupt}, /* irq1  */
	{0x82, NonHandledInterrupt}, /* irq2  */
	{0x82, NonHandledInterrupt}, /* irq3  */
	{0x82, NonHandledInterrupt}, /* irq4  */
	{0x82, NonHandledInterrupt}, /* irq5  */
	{0x82, NonHandledInterrupt}, /* irq6  */
	{0x82, NonHandledInterrupt}, /* irq7  */
	{0x82, (interrupt_handler_t)EXTI0_InterruptHandler}, /* irq8  */
	{0x82, (interrupt_handler_t)EXTI1_InterruptHandler}, /* irq9  */
	{0x82, (interrupt_handler_t)EXTI2_InterruptHandler}, /* irq10 */
	{0x82, (interrupt_handler_t)EXTI3_InterruptHandler}, /* irq11 */
	{0x82, (interrupt_handler_t)EXTI4_InterruptHandler}, /* irq12 */
	{0x82, (interrupt_handler_t)EXTI5_InterruptHandler}, /* irq13 */
	{0x82, NonHandledInterrupt}, /* irq14 */
	{0x82, NonHandledInterrupt}, /* irq15 */
	{0x82, NonHandledInterrupt}, /* irq16 */
	{0x82, NonHandledInterrupt}, /* irq17 */
	{0x82, NonHandledInterrupt}, /* irq18 */
	{0x82, (interrupt_handler_t)TIM2_InterruptHandler}, /* irq19 */
	{0x82, NonHandledInterrupt}, /* irq20 */
	{0x82, NonHandledInterrupt}, /* irq21 */
	{0x82, NonHandledInterrupt}, /* irq22 */
	{0x82, NonHandledInterrupt}, /* irq23 */
	{0x82, NonHandledInterrupt}, /* irq24 */
	{0x82, (interrupt_handler_t)TIM4_InterruptHandler}, /* irq25 */
	{0x82, NonHandledInterrupt}, /* irq26 */
	{0x82, NonHandledInterrupt}, /* irq27 */
	{0x82, NonHandledInterrupt}, /* irq28 */
	{0x82, NonHandledInterrupt}, /* irq29 */
};
