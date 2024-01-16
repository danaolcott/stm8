/*
5/11/20
EEPROM Controller File

*/

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#include "register.h"
#include "eeprom.h"
#include "gpio.h"


//////////////////////////////////////////
void EEPROM_delay(uint16_t delay)
{
    volatile uint16_t time = delay;
    while (time > 0)
        time--;
}


/////////////////////////////////////////
//Configure EEPROM for write/read ability.
//Unlock the write protection using the MASS
//key sequence.
void EEPROM_init(void)
{
    unsigned char status = 0x00;
    unsigned char counter = 0x00;

    //returns 1 if unlocked    
    status = EEPROM_unlock();
    
    //if bad - wait for a while, then try to
    //unlock the memory chip again
    while(!status)
    {
    	counter = 4;
        while (counter > 0)
        {
            GPIO_led_red_toggle();
            EEPROM_delay(10000);
            counter--;
        }

        status = EEPROM_unlock();
    }
}


/////////////////////////////////////////
//Disable write protection.  return 1 if
//ok
uint8_t EEPROM_unlock(void)
{
    uint8_t result = 0x00;
    uint8_t counter = 0x00;

    //write key values to FLASH_DUKR register
    FLASH_DUKR = 0xAE;
    FLASH_DUKR = 0x56;

    //test the DUL bit on the FLASH_IAPSR
    //if high, unlock was successfull
    if (FLASH_IAPSR & FLASH_DUL_BIT)
    {
        result = 1;
        return result;
    }

    else
    {
        //try again, this time with a delay
        //repeat up to 10 times
        while ((!(FLASH_IAPSR & FLASH_DUL_BIT)) && (counter < 10))
        {
            EEPROM_delay(1000);
            FLASH_DUKR = 0xAE;
            EEPROM_delay(1000);
            FLASH_DUKR = 0x56;
            EEPROM_delay(1000);

            if (FLASH_IAPSR & FLASH_DUL_BIT)
            {
                result = 1;
                return result;
            }

            counter++;
        }
    }
    return 0x00;        //bad result
}


//////////////////////////////////////////
//Enable write protection
uint8_t EEPROM_lock(void)
{
    //clear the DUL bit on the FLASH_IAPSR register
    FLASH_IAPSR &=~ FLASH_DUL_BIT;

    if (FLASH_IAPSR & FLASH_DUL_BIT)
        return 1;
    else
        return 0;
}


/////////////////////////////////////////
//Write byte to address
uint8_t EEPROM_write(uint16_t address, uint8_t data)
{
    uint8_t status = 0x00;
    uint8_t readValue = 0x00;
    uint16_t targetAddress = (uint16_t)EEPROM_START_ADDR + address;
    
    //check to see if the DUL bit is set
    if (FLASH_IAPSR & FLASH_DUL_BIT)
    {
        //cast to an address and set data
        *((volatile uint8_t*)targetAddress) = data;
        
        //test EOP bit - high when complete
        while (!(FLASH_IAPSR & FLASH_EOP_BIT)){};
        
        //read it back - also clears the EOP flag
        readValue = EEPROM_read(address);
        
        if (readValue == data)
            return 1;
        else
            return 0;
    }
        
    return 0;
}


/////////////////////////////////////////
//Read byte from address
uint8_t EEPROM_read(uint16_t address)
{
    uint8_t result = 0x00;
    uint16_t targetAddress = (uint16_t)EEPROM_START_ADDR + address;
    result = *((volatile uint8_t*)targetAddress);
    return result;
}


///////////////////////////////////////////////////
//Clear the entire eeprom memory space from
//EEPROM start addr to stop addr
//clears the first 256 address bytes
void EEPROM_clearEEPROM(uint8_t value)
{
    uint16_t i = 0;
    
    for (i = 0 ; i < 0xFF ; i++)
    {
        EEPROM_write(i, value);
    }
}

///////////////////////////////////////////////
//Update / increment current game cycle count
uint16_t EEPROM_updateCycleCount(void)
{
	uint16_t result = 0x00;
	uint8_t high = 0x00;
	uint8_t low = 0x00;

	result = EEPROM_readCycleCount();
	result += 1;
	
	//else, do nothing, and result = 0	
	high = (uint8_t)((result >> 8) & 0xFF);
	low = (uint8_t)(result & 0xFF);
	
	EEPROM_write(EEPROM_ADDRESS_CYCLE_COUNT_MSB, high);
	EEPROM_write(EEPROM_ADDRESS_CYCLE_COUNT_LSB, low);
	
	return result;
}

uint16_t EEPROM_readCycleCount(void)
{
	uint16_t low = 0x00;
	uint16_t high = 0x00;
	uint16_t result = 0x00;
	
	low = EEPROM_read(EEPROM_ADDRESS_CYCLE_COUNT_LSB);
	high = EEPROM_read(EEPROM_ADDRESS_CYCLE_COUNT_MSB);

	result = ((high & 0xFF) << 8);
	result |= (low & 0xFF);
	
	return result;
}


uint16_t EEPROM_getHighScore(void)
{
	uint16_t low = 0x00;
	uint16_t high = 0x00;
	uint16_t result = 0x00;
	
	low = EEPROM_read(EEPROM_ADDRESS_HIGH_SCORE_LSB);
	high = EEPROM_read(EEPROM_ADDRESS_HIGH_SCORE_MSB);

	result = ((high & 0xFF) << 8);
	result |= (low & 0xFF);
	
	return result;
}

void EEPROM_updateHighScore(uint16_t score)
{
	uint8_t high = 0x00;
	uint8_t low = 0x00;
	
	high = (uint8_t)((score >> 8) & 0xFF);
	low = (uint8_t)(score & 0xFF);
	
	EEPROM_write(EEPROM_ADDRESS_HIGH_SCORE_MSB, high);
	EEPROM_write(EEPROM_ADDRESS_HIGH_SCORE_LSB, low);
}






