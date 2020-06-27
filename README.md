# stm8
Projects Built on STMicro STM8 Processors
------------------------------------------
This repository contains several projects using the STM8 processor by STMicro.  Two varients of the STM8 including STM8S103F3P6 and  STM8L151K6 are used for these projects.  The general intent was to implement various peripherals and compile into some type of a game or other useful device.  A development board for the STM8S103F3P6 is commonly found on Ebay for about $1.  The development board for the STM8L151K6 is from DanasBoatshop.com.  The The development board was built in KiCad and made by OSHPark.  The schematics are provided in the repository.

The following is a list of projects:
------------------------------------

sensor
------
A simple transmitter project using the STM8S103F3P6 and nrf24l01 transceiver.  Reads the AD conververter connected to a temperature sensor and sends the data over the nrf24l01.  This project implements the SPI, UART, and TIMER.

stm8dev_asteroids
-----------------
A simple implementation of the 80's game Asteroids.  The project is built using the STM8L151K6 and the game development board from DanasBoatshop.com.  The project implements the SPI, TIMER, EEPROM, DAC, and interrupts.

stm8devboard
------------
A simple implementation of the 80's game Space Invaders.  The project is built using the STM8L151K6 and the game development board from DanasBoatshop.com.  The project implements the SPI, TIMER, EEPROM, DAC, and interrupts.  Note:  The project started out as trying to get the GPIO to blink the LED, but expanded into something much bigger.

stm8_breakout
-------------
A simple implementation of the 80's game Breakout.  Bounce ball off player to hit tiles.  Toggle between auto / manual mode using center button.  Toggle sound on/off using the user button.  The project is built using the STM8L151K6 processor and game development board from DanasBoatshop.com.  The project implements the SPI, TIMER, EEPROM, DAC, and interrupts.

stm8_qpn
--------
Using QP Nano on the STM8 development board.  The project was originally written for the MSP430 processor and modified to work with the STM8 processor.  It contains two objects, blinky LED and a button.  Press the button to send message to blinky object to toggle between 2 states.  One state also plays a tone on the speaker.  The port of QPN is at least 1 year old, so Im' sure there is a newer QPN library, and will likely get used in subsequent projects.


![alt text](https://raw.githubusercontent.com/danaolcott/stm8/master/source/stvd/stm8devboard_schematics/stm8_dev2.jpg)



