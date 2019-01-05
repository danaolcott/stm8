/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 */
 
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
PC5 - PC7, PC3 - SPI - idle clock low, leading edge for data
PC3 - CS pin for SPI

PD5 - PD6 - UART1 (TX/RX)

PA1 - PA2 - Control lines for NRF24L01
PA1 - output - CE pin
PA2 - input / interrupt / falling edge trigger

Timers - TIM4 and TIM2, each configured at 1khz
with interrupt on update


 */
#include <stddef.h>
#include <stdint.h>
#include "register.h"
#include "gpio.h"
#include "timer.h"
#include "spi.h"
#include "uart.h"
#include "nrf24l01.h"

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
    CLOCK_SOURCE_FCPU
}ClockSource_t;

/////////////////////////////////////////////////
//Prototypes
void Clock_init(ClockSpeed_t speed);
void Clock_outputConfig(ClockSource_t source);

void EnableInterrupts(void);

main()
{ 
    //vars
//    uint8_t status, pipe2, pipe3, pipe4, pipe5, status_alt = 0x00;
    uint8_t pw0, pw1, pw2, pw3, pw4, pw5 = 0x00;
    volatile uint8_t fifo, status, config = 0x00;
    uint8_t txData[8] = "Hello123";
    uint8_t buffer[50] = {0x00};
    int n;
//    uint8_t result = 0x00;
    
    //enable global interrupts
    EnableInterrupts();
    
    Clock_init(CLOCK_SPEED_16MHZ);
    Clock_outputConfig(CLOCK_SOURCE_FMASTER);

    
    
    GPIO_init();
    Timer4_init();
    Timer2_init();
    Timer2_stop();
    
    SPI_init();
    UART_init(BAUD_RATE_57600);
    
    nrf24_init(NRF24_MODE_RX);
    
    while (1)
    {
        LED_Toggle();
//        nrf24_transmitData(txData, 8);
//        status = nrf24_getStatus();
//        fifo = nrf24_getFifoStatus();
//        config = nrf24_readReg(NRF24_REG_CONFIG);
        
        delay_ms(2000);
    }
}


/////////////////////////////////////
//Sets the I1 I0 bits to 10 in the 
//CPU CCR register
void EnableInterrupts(void)
{
    _asm("RIM");
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

