/*
7/18/20
GPIO Controller File
Function definitions for controlling the pins
on the development board from Dana's Boathshop.com

This approach uses the standard library from STM
for initializing and controlling the gpio pins

Initializes the following:
LEDs red and green on PB5 and PB6
User button and center joystick button as interrupt, 
falling edge trigger.  

*/


#include "main.h"
#include "gpio.h"

/////////////////////////////////////////////
//Configure LEDs red and green as output
//PB5 - red
//PB6 - green
//Configure user button and center button on
//joystick as input, interrupt, falling trigger
//enable global interrupts in main program
//
void gpio_init(void)
{
    //void GPIO_Init(GPIO_TypeDef* GPIOx, uint8_t GPIO_Pin, GPIO_Mode_TypeDef GPIO_Mode);
    GPIO_Init(GPIOB, GPIO_Pin_5, GPIO_Mode_Out_PP_Low_Fast);
    GPIO_Init(GPIOB, GPIO_Pin_6, GPIO_Mode_Out_PP_Low_Fast);
    
    GPIO_ResetBits(GPIOB, GPIO_Pin_5);
    GPIO_ResetBits(GPIOB, GPIO_Pin_6);

    //enable the user button - interrupt, falling edge trigger - EXTI5
    GPIO_Init(GPIOD, GPIO_Pin_5, GPIO_Mode_In_PU_IT);
    EXTI_DeInit();
    EXTI_SetPinSensitivity(EXTI_Pin_5, EXTI_Trigger_Falling);
    EXTI_SelectPort(EXTI_Port_D);
    
    //enable the center button on joystick - interrupt, falling edge trigger - EXTI1
    GPIO_Init(GPIOD, GPIO_Pin_1, GPIO_Mode_In_PU_IT);
    EXTI_DeInit();
    EXTI_SetPinSensitivity(EXTI_Pin_1, EXTI_Trigger_Falling);
    EXTI_SelectPort(EXTI_Port_D);

}



void exti1_isr(void)
{
        //read the status
    if (EXTI_GetITStatus(EXTI_IT_PortD) == RESET)
    {
        GPIO_ToggleBits(GPIOB, GPIO_Pin_6);
    }

    EXTI_ClearITPendingBit(EXTI_IT_Pin1);
}

void exti5_isr(void)
{
    
    //read the status
    if (EXTI_GetITStatus(EXTI_IT_PortD) == RESET)
    {
        GPIO_ToggleBits(GPIOB, GPIO_Pin_5);
    }

    EXTI_ClearITPendingBit(EXTI_IT_Pin5);
}



