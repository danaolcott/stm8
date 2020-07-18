/**
  ******************************************************************************
  * @file    Project/STM8L15x_StdPeriph_Template/main.c
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    13-May-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */  
	
/* Includes ------------------------------------------------------------------*/
/*
The purpose of this project is to enable various
peripherals on the board using the STM standard library.
The project is called blink, but it will likely enable
other parts of the board too, including flashing leds,
user button, polling and interrupt, spi output, ...etc




*/


#include "stm8l15x.h"
#include "main.h"

/** @addtogroup STM8L15x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

void delay(uint32_t time);
void gpio_init(void);
void tim2_init(void);
void clock_init(void);
void interrupt_enable(void);
void interrupt_disable(void);


volatile uint32_t gTimerTick = 0x00;

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
    //disable global interrupts
    interrupt_disable();
    
    //configure peripherals
    clock_init();
    gpio_init();
    tim2_init();
    
    //enable global interrupts
    interrupt_enable();
    
    /* Infinite loop */
    while (1)
    {
        GPIO_ToggleBits(GPIOB, GPIO_Pin_5);
        delay(500);
    }
}





////////////////////////////////////////////
//Delay ms
void delay(uint32_t time)
{
    volatile uint32_t temp = gTimerTick + time;
    while (gTimerTick < temp);
}


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



/////////////////////////////////////////////
//Configure timer2 to timeout at 1ms and 
//generate an interrupt
//Assuming base clock = 1mhz, prescale = 2, period of 
//(clock / 1000) - 1 gives a timeout at 1ms. 
void tim2_init(void)
{
    uint16_t period = 999;
    TIM2_DeInit();
    
    //enable internal clock
    TIM2_InternalClockConfig();
    TIM2_TimeBaseInit(TIM2_Prescaler_2, TIM2_CounterMode_Up, period);
    
    //configure interrupt - see stm8_interrupt_vector.c
    TIM2_ITConfig(TIM2_IT_Update, ENABLE);

    //start the timer
    TIM2_Cmd(ENABLE);
}


////////////////////////////////////////////
//Configure main clocks and enable the
//peripheral clocks
void clock_init(void)
{
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, ENABLE);
}

////////////////////////////////////////////
//Enable global interrupts
void interrupt_enable(void)
{
    _asm("RIM");
}

///////////////////////////////////////////
//Disable global interrupts
void interrupt_disable(void)
{
    _asm("SIM");
}


void tim2_isr(void)
{
    //clear the interrupt
    TIM2_ClearITPendingBit(TIM2_IT_Update);

    gTimerTick++;
}





#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
