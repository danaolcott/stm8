/*
Dana Olcott
12/12/18

Blink Project Using the STM8S103F3P6 Processor
Cheap board from ebay.
Pins Available:
PA1 - PA3
PB4 - PB5
PC3 - PC7
PD1 - PD6

Register Descriptions:
PB_ODR - output data register
PB_IDR - input data register
PB_DDR - data direction
PB_CR1 - control reg 1
PB_CR2 - control reg 2

See Table 21 - Page 107 in the ref manual
For output PP, fast mode, config ODR = 1, CR1 = 1, CR2 = 1

PB4 - User Button
PB5 - User LED
PC4 - PC7 - SPI
*/

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include "register.h"
#include "gpio.h"
#include "timer.h"
#include "spi.h"
#include "eeprom.h"


//////////////////////////////////////////////////
//Interrupt Handler Functions - Timer4_ISR
void Timer4_InterruptHandler(void) __interrupt(TIM4_IRQ)
{
    PA_ODR ^= BIT_3;        //toggle PA3 - debug pin
    Timer4_ISR();			//Timer4 ISR
    TIM4_SR &=~ UIF_BIT;	//Clear the interrupt flag	
}


//EXTI1 - PortB
void EXTI1_InterruptHandler(void) __interrupt(EXTI1_IRQ)
{
	EXTI1_ISR();			//GPIO ISR
}


///////////////////////////////////////////
//Typedefs
typedef enum
{
    CLOCK_SPEED_2MHZ,
    CLOCK_SPEED_4MHZ,
    CLOCK_SPEED_8MHZ,
    CLOCK_SPEED_16MHZ
}ClockSpeed_t;


/////////////////////////////////////////////////
//Prototypes
void Clock_init(ClockSpeed_t speed);


////////////////////////////////////
uint8_t i, result = 0x00;


int main()
{
    Clock_init(CLOCK_SPEED_16MHZ);
    GPIO_init();
    Timer4_init();
    SPI_init();
    EEPROM_init();

    while (1)
    {
		LED_Toggle();
//        PA_ODR ^= BIT_2;
//        SPI_SendByte(0xAA);
		delay_ms(1000);

        //read them back
        for (i = 0 ; i < 0xFF ; i++)
        {            
            result = EEPROM_readByte(i);

            if (result == i)
            {
                for (i = 0 ; i < 10 ; i++)
                {
                    LED_Toggle();
                    delay_ms(50);
                }            
            } 
        }

        delay_ms(1000);

    }



    return 0;
}






////////////////////////////////////////////
//Clock_init
//Configure HSI to run at 16MHZ
//Default = Fcpu = Fmaster, Fmaster = 2mhz
//
void Clock_init(ClockSpeed_t speed)
{

    //CLK_ICKR - BIT0 - HSI enable
    CLK_ICKR |= BIT_0;

    //CLK_ECKR - clear BIT0 - HSE off (default is on)
    CLK_ECKR &=~ BIT_0;

    //CLK_CKDIVR - clock divider 
    //Fhsi = RC output - 16mhz - bits 4:3 = 00
    //CPU divider - fcpu = fmaster = 000
    switch(speed)
    {
        case CLOCK_SPEED_2MHZ:      CLK_CKDIVR = 0x18;      break;
        case CLOCK_SPEED_4MHZ:      CLK_CKDIVR = 0x10;      break;
        case CLOCK_SPEED_8MHZ:      CLK_CKDIVR = 0x08;      break;
        case CLOCK_SPEED_16MHZ:     CLK_CKDIVR = 0x00;      break;
        default:                    CLK_CKDIVR = 0x00;      break;
    }

    //CLK_PCKENR1 - peripheral clock enable/disable
    //CLK_PCKENR2 - peripherla clock enable/disable

    //Configurable clock output register
    //outputs master / cpu clock to PC4 / CLK_CCO pin
    //CLK_CCOR - leave this alone for now

}











