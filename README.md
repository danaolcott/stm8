# stm8
Projects Built on STM8 Processors
------------------------------------------
This repository contains several projects using the STM8 processor.  Two varients of the STM8 include STM8S103F3P6 and STM8L151K6 are used for these projects.  A development board for the STM8S103F3P6 is commonly found on Ebay for about $1.  The development board for the STM8L151K6 processor was layed out in KiCad, sent to OSHPark for constuction, parts from Digikey, and assembled in my laboratory.  The schematics for the board are provided in the respository.  The board is pretty useful for experimenting with the STM8L151K6 processor.  It contains a 128x64 display, user buttons, 4 position joystick, temperature sensor, eeprom memory, 2 leds, a really loud speaker, and powered from a 3.3v coil cell battery.

For development using Windows, I used STVD (ST visual Develop) with the Cosmic C compiler.  Overall, using STVD is not that difficult in terms of getting a project to compile, flashing the code on the processor, using the debugger, etc.  Development using Linux is quite a bit more difficult.  For these projects, the SDCC (Small Device C Compiler) was used.  In either Windows or Linux, the STM programmer / debugger is needed to flash the code onto the processor.  The following are a few notes regarding using Linux and the SDCC compiler:

## Linux
### Install STLink
* Install libusb-1.0-0-dev
* Get the stlink repository and build the ST Link Tools from source: https://github.com/texane/stlink

* Note: After clone the repo to local computer, the README file mentions installing the st-tools directly using the package manager.  It points you to the following link: http://ftp.us.debian.org/debian/pool/main/s/stlink/stlink-tools_1.5.1+ds-1_amd64.deb

* Add the repo to the /etc/apt/sources.list
* Update the package list using apt update
* Install the packages using the package manager: sudo apt-get install stlink-tools

When you plug in the STLink, should should see stlinkv2_1, _2, etc in /dev/directory when the device is plugged in.

### Install stm8flash
* Clone the repo for stm8flash and build from scratch.  I had to install a few things for the program to build.
* Add libusb-1.0 to the include path in the Makefile
* I got an error about pkg-config, had to install pkg-config.

Copy the stm8flash executable into appropriate location, (ie, /usr/bin... etc)


Directories
-----------
* library - Projects that were built using STVD and the STM8 Standard Peripheral Library.
* sdcc - Projects that were built using the Small Device C Compilier.
* stvd - Projects that were built using STVD environment.

See next section for a list of projects (work in progress).

The following is a list of projects:
------------------------------------

sensor
------
A simple transmitter project using the STM8S103F3P6 and nrf24l01 transceiver.  Reads the AD conververter connected to a temperature sensor and sends the data over the nrf24l01.  This project implements the SPI, UART, and TIMER.

stm8dev_asteroids
-----------------
A simple implementation of the 80's game Asteroids.  The project is built using the STM8L151K6 processor.  The project implements the SPI, TIMER, EEPROM, DAC, and interrupts.

stm8devboard
------------
A simple implementation of the 80's game Space Invaders.  The project is built using the STM8L151K6 processor.  The project implements the SPI, TIMER, EEPROM, DAC, and interrupts.  Note:  The project started out as trying to get the GPIO to blink the LED, but expanded into something much bigger.

stm8_breakout
-------------
A simple implementation of the 80's game Breakout.  Bounce ball off player to hit tiles.  Toggle between auto / manual mode using center button.  Toggle sound on/off using the user button.  The project is built using the STM8L151K6 processor.  The project implements the SPI, TIMER, EEPROM, DAC, and interrupts.

stm8_qpn
--------
Using QP Nano on the STM8 development board.  The project was originally written for the MSP430 processor and modified to work with the STM8 processor.  It contains two objects, blinky LED and a button.  Press the button to send message to blinky object to toggle between 2 states.  One state also plays a tone on the speaker.  The port of QPN is at least 1 year old, so Im' sure there is a newer QPN library, and will likely get used in subsequent projects.

stm8_dev_i2c
------------
A simple i2c project that uses the BMP280 temperature/pressure sensor.  Reads the temperature and pressure and displays on the LCD.  Changes in pressure are used to track changes in elevation using a simple coorelation of about 3pa / ft.  Press the user button and center button to update a baseline pressure and reset the current base elevation.

stm8_dev_adc
------------
A project that initializes the ADC to read channels 7, 16, and vref and output the values on the LCD.  The read values in millivolts are scaled based on the ratio of the vref and factory calibrated vref based on an input voltage of 3.00v.  The project also reads and displays the temperature from the MCP9700 temperature chip.

library / adc
-------------
A project that uses the STM8 Standard Library to read a temperature sensor and output the results to an LCD.  This project also initializes the DAC output and routes the output to a GPIO pin to generate sound on a speaker.  The sound is a simple squarewave.

library / blink
---------------
A project that uses the STM8 Standard Library to initialize GPIOs for LEDs and toggle the LEDs

library / spi
-------------
A project that inintializes the SPI peripheral and controls a SPI-enabled LCD.

library / i2c
-------------
A project that initializes the i2c to read temperature and pressure from the BMP280 and display results on the LCD.




![alt text](https://raw.githubusercontent.com/danaolcott/stm8/master/source/stvd/stm8devboard_schematics/stm8_dev2.jpg)



