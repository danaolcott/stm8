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

PB4 - User Button - Interrupt / Falling
PB5 - User LED
PC5 - PC7, PA3 - SPI
PD5 - PD6 - UART1 (TX/RX)
PA1 - PA2 - General Output

Timer - TIM4 - Timebase at 1khz

ADC Channels PD2, PD3 - AN3, AN4


*/

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include "register.h"
#include "gpio.h"
#include "timer.h"
#include "spi.h"
#include "eeprom.h"
#include "uart.h"
#include "adc.h"

//////////////////////////////////////////////////
//Interrupt Handler Functions - Timer4_ISR
void Timer4_InterruptHandler(void) __interrupt(TIM4_IRQ)
{
    Timer4_ISR();			//Timer4 ISR
    TIM4_SR &=~ UIF_BIT;	//Clear the interrupt flag	
}


void Timer2_InterruptHandler(void) __interrupt(TIM2_UPDATE_IRQ)
{
    Timer2_ISR();			//Timer2 ISR
    TIM2_SR1 &=~ UIF_BIT;	//Clear the interrupt flag	
}


//EXTI1 - PortB
void EXTI1_InterruptHandler(void) __interrupt(EXTI1_IRQ)
{
	EXTI1_ISR();			//GPIO ISR
}


//UART
void UART_InterruptHandler(void) __interrupt(UART1_RXNE_IRQ)
{
    UART_ISR();            //GPIO ISR
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


typedef enum
{
    CLOCK_SOURCE_FMASTER,
    CLOCK_SOURCE_FCPU,
    
}ClockSource_t;

/////////////////////////////////////////////////
//Prototypes
void Clock_init(ClockSpeed_t speed);
void Clock_outputConfig(ClockSource_t source);

////////////////////////////////////
uint16_t i, result = 0x00;
int n = 0x00;
uint8_t buffer[50] = {0x00};

int main()
{
    Clock_init(CLOCK_SPEED_16MHZ);
    Clock_outputConfig(CLOCK_SOURCE_FMASTER);

    GPIO_init();
    Timer4_init();          //timebase
    Timer2_init();          //extra timer
    Timer2_stop();

    SPI_init();
    EEPROM_init();
    UART_init(BAUD_RATE_57600);
    ADC_init();


    while (1)
    {
        LED_Toggle();
        //        PA_ODR ^= BIT_2;
        //        SPI_SendByte(0xAA);

        UART_sendString("Hello\r\n");

        n = sprintf(buffer, "ADC3: %d\n", ADC_readMilliVolts(ADC_CHANNEL_3));
        UART_sendStringLength(buffer, n);
        n = sprintf(buffer, "ADC4: %d\n\n", ADC_readMilliVolts(ADC_CHANNEL_4));
        UART_sendStringLength(buffer, n);

        delay_ms(1000);

    }



    return 0;
}






////////////////////////////////////////////
//Clock_init
//Configure HSI to run at 16MHZ
//Default = Fcpu = Fmaster, Fmaster = 2mhz
//Configure GPIO PC4 to output Fmaster
//on that pin.  Section 9.7 - config as input
//with pullup (PC_CR1 = 1)
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

}


///////////////////////////////////////////////
//Configure clock source output to pin PC4
//
void Clock_outputConfig(ClockSource_t source)
{
    //Configurable clock output register
    //outputs master / cpu clock to PC4 / CLK_CCO pin
    //CLK_CCOR - leave this alone for now

    PC_DDR &=~ BIT_4;        //input
    PC_CR1 |= BIT_4;         //pullup
    PC_CR2 &=~ BIT_4;        //no interrupt

    //source select bits are write protected until the 
    //clock source is stable.

    while (CLK_CCOR & CLK_CCOBSY_BIT){};       //wait until not busy

    //bits 4-1 - set to 1100 for Fmaster as output source
    //1110 0001
    CLK_CCOR = CLK_CCOR & 0xE1;       //clear bits

    //set the clock source bits
    switch(source)
    {
        case CLOCK_SOURCE_FMASTER:      CLK_CCOR |= (0x0C << 1);        break;
        case CLOCK_SOURCE_FCPU:         CLK_CCOR |= (0x0D << 1);        break;
        default:                        CLK_CCOR |= (0x0C << 1);        break;    
    }

    //enable the output
    CLK_CCOR |= CLK_CCOEN_BIT;

    //wait
    while (!(CLK_CCOR & CLK_CCORDY_BIT)){};
}










