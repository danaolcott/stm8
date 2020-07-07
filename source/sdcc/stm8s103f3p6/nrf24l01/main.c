/*
Dana Olcott
12/25/18

Project using the STM8S103F3P6 Processor with
the NRF24L01 RF IC from Nordic.  Project uses the
cheap dev board from ebay and the NRF24L01 transceiver
from amazon.  

Set up the project as a simple transmitter.  Use the CLI
UART and user button to transmit message.

Pinout:

PB4 - User Button - Interrupt / Falling
PB5 - User LED
PC5 - PC7, PA3 - SPI - idle clock low, leading edge for data
PD5 - PD6 - UART1 (TX/RX)

PA1 - PA2 - Control lines for NRF24L01
PA1 - output - CE pin
PA2 - input / interrupt / falling edge trigger

Timers - TIM4 and TIM2, each configured at 1khz
with interrupt on update

*/

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include "register.h"
#include "gpio.h"
#include "timer.h"
#include "spi.h"
#include "uart.h"
#include "adc.h"
#include "nrf24l01.h"
#include "utility.h"


#define CONFIG_TRANSMITTER      1


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

//EXTI0 - PortA - PA2 as input, falling edge, pullup
void EXTI0_InterruptHandler(void) __interrupt(EXTI0_IRQ)
{
    nrf24_ISR();
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


//////////////////////////////////////
//Globals
//uint8_t txBuffer[NRF24_PIPE_WIDTH] = {0x00};
uint8_t txBuffer[30] = {0x00};
uint16_t adcMv = 0x00;
uint32_t temperature = 0x00;
uint8_t tempInt = 0x00;
uint8_t tempFrac = 0x00;
uint8_t lsb, msb = 0x00;
int n = 0x00;
int counter = 0x00;
uint8_t pipe = 0x00;

////////////////////////////////////
//
int main()
{
    Clock_init(CLOCK_SPEED_16MHZ);
    Clock_outputConfig(CLOCK_SOURCE_FMASTER);

    GPIO_init();
    Timer4_init();          //timebase
    Timer2_init();          //extra timer
    Timer2_stop();
    SPI_init();
    UART_init(BAUD_RATE_57600);
    ADC_init();

#ifdef CONFIG_TRANSMITTER
    nrf24_init(NRF24_MODE_TX);
#else
    nrf24_init(NRF24_MODE_RX);
#endif


    while (1)
    {
 
#ifdef CONFIG_TRANSMITTER

        //Transmitter - ADC Channel 3
        //uint8_t txBuffer[NRF24_PIPE_WIDTH] = {0x00};
        //uint16_t ADC_readMilliVolts(ADC_Channel_t ch);
        adcMv = ADC_readMilliVolts(ADC_CHANNEL_3);
        
        lsb = adcMv & 0xFF;
        msb = (adcMv >> 8) & 0xFF;

        temperature = (adcMv - 500) * 100;
        temperature *= 9;
        temperature = temperature / 5;
        temperature += 32000;

        //temp is deg * 100
        tempInt = temperature / 1000;
        tempFrac = (temperature / 100) % 10;

        txBuffer[0] = 0xFE;
        txBuffer[1] = STATION_1;
        txBuffer[1] = MID_TEMP_MCP9700A;

        txBuffer[3] = lsb;
        txBuffer[4] = msb;

        txBuffer[5] = tempInt;
        txBuffer[6] = tempFrac;

        txBuffer[7] = 0xFE;
        
        nrf24_transmitData(pipe, txBuffer, 8);

        if (pipe < 5)
            pipe++;
        else
            pipe = 0;
#endif

        LED_On();
        delay_ms(500);
        LED_Off();
        delay_ms(4500);

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










