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
The purpose of this project is to enable the SPI interface
and display a message on the LCD.  It's a continuation of the
project "blink".  This project uses the STM standard library
for initialize and control the peripherals.

*/


#include "stm8l15x.h"
#include "main.h"
#include "gpio.h"
#include "timer.h"
#include "spi.h"
#include "lcd.h"


/** @addtogroup STM8L15x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/


void clock_init(void);
void system_init(void);



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
    clock_init();       //high speed internal, peripheral clocks
    system_init();      //routing
    gpio_init();
    tim2_init();
    spi_init();
    lcd_init();
    
    //enable global interrupts
    interrupt_enable();
    
    lcd_clear(0x00);
    
    lcd_drawString(0, 0, "SPI Enabled");
    lcd_drawString(1, 0, "LCD Display");
    
    
    
    /* Infinite loop */
    while (1)
    {
        GPIO_ToggleBits(GPIOB, GPIO_Pin_5);
        delay_ms(500);
    }
}





////////////////////////////////////////////
//Configure main clocks and enable the
//peripheral clocks
void clock_init(void)
{
    //system clock source
    CLK_DeInit();
    CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);
    CLK_HSICmd(ENABLE);
    
    //peripheral clocks
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, ENABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_SPI1, ENABLE);
}

void system_init(void)
{
    //remap the SPI pins
    SYSCFG_REMAPDeInit();
    SYSCFG_REMAPPinConfig(REMAP_Pin_SPI1Full, ENABLE);
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
