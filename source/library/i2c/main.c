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
The purpose of this project is to enable the i2c peripheral
to measure temperature and pressure using the BMP280 breakout board.
This project uses the STM8 standard peripheral library.  It is a 
continuation of previous projects, also enableing the ADC, DAC, etc...

The project reads the chip id at register 0xD0 and returns the value.
If the value == 0x58, it turns the green LED on.  The i2c address
for the BMP280 is 0x78.  Note that the address has to be up-shifted
by 1 in order to work properly.


*/


#include "stm8l15x.h"
#include "main.h"
#include "gpio.h"
#include "timer.h"
#include "spi.h"
#include "lcd.h"
#include "adc.h"
#include "dac.h"
#include "i2c.h"
#include "bmp280.h"



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

uint16_t adcResult = 0x00;
uint8_t length = 0x00;
uint8_t printBuffer[16];
uint16_t counter = 0x00;
uint8_t toggle = 0x00;
int value = 0x00;
uint8_t result = 0x00;
uint8_t rx[16];
BMP280_Data bmpData;

uint16_t pInt = 0;
uint16_t pFrac = 0;
int temperature = 0x00;



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
    adc_init(ADC_CHANNEL_16);
    dac_init();
    I2C_init();
    BMP280_init();
    
    
    //enable global interrupts
    interrupt_enable();
    
    lcd_clear(0x00);    
    lcd_drawString(0, 0, "I2C Project");
                
    /* Infinite loop */
    while (1)
    {
        lcd_clear(0x00);
        
        //read the pressure and temp from BMP280
        BMP280_read(&bmpData);
        temperature = adc_readTempF();
        
        //temperature
        lcd_drawString(0, 0, "T:");
        length = lcd_decimalToBuffer((uint16_t)bmpData.cTemperatureF, printBuffer, 16);
        lcd_drawStringLength(0, 16, printBuffer, 16);
        
        //pressure
        lcd_drawString(1, 0, "P:");
        pInt = ((uint16_t)(bmpData.cPressurePa / 1000));
        pFrac = ((uint16_t)(bmpData.cPressurePa % 1000));
                
        length = lcd_decimalToBuffer(pInt, printBuffer, 16);
        lcd_drawStringLength(1, 16, printBuffer, length);
        
        lcd_drawString(1, 40, ".");
        length = lcd_decimalToBuffer(pFrac, printBuffer, 16);
        lcd_drawStringLength(1, 48, printBuffer, length);
        
        
        lcd_drawString(3, 0, "MCP9700:");
        length = lcd_decimalToBuffer((uint16_t)temperature, printBuffer, 16);
        lcd_drawStringLength(4, 0, printBuffer, 16);

        
        GPIO_ToggleBits(GPIOB, GPIO_Pin_6);
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
    CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
    
    CLK_HSICmd(ENABLE);
    
    //peripheral clocks
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, ENABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_SPI1, ENABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_COMP, ENABLE); //routing
    CLK_PeripheralClockConfig(CLK_Peripheral_DAC, ENABLE);  //dac    
    CLK_PeripheralClockConfig(CLK_Peripheral_I2C1, ENABLE); //dac
}

void system_init(void)
{
    //remap the SPI pins
    SYSCFG_REMAPDeInit();
    SYSCFG_REMAPPinConfig(REMAP_Pin_SPI1Full, ENABLE);
    
    //route dac output to pin PB4 - CH15E
    SYSCFG_RIIOSwitchConfig(RI_IOSwitch_15,ENABLE);

    //enable the vref output on PD6 - RI_IOSR3 - CH9E
    SYSCFG_RIIOSwitchConfig(RI_IOSwitch_9,ENABLE);    
    COMP_VrefintToCOMP1Connect(ENABLE);
    COMP_VrefintOutputCmd(ENABLE);


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
