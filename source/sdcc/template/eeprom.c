/*
EEPROM Controller File for Use with 
the STM8SF103 Processor.  
Accesses the ICs internal EEPROM

Process for read / write to onchip EEPROM:
640 bytes of data eeprom.  Data is write protected 
until the user unlocks it.
Write protection is unlocked by writing 2 sets of 
MASS keys to a specific register.  A bit is set 
indicating the data memory is unlocked.  EEPROM
address starts at 0x4000 for all ICs.  Writes / reads
are performed directly to the address after the 
EEPROM is unlocked.  There is a bit that can be
polled to determine if the write / read is complete.

FLASH_IAPSR - EOP flag is set after programming data memory
WR_PG_DIS bit is set when tried to write to a protected section

Enable write access to data section:
write two key values to the FLASH_DUKR register:
0xAE
0x56

If the writes are sucessfull and it's unlocked, the
DUL bit of the FLASH_IAPSR is set.  ie, if not set, 
try again...

NOTE: you can disable write access to the DATA 
area by clearing the DUL bit

Register addresses:
FLASH_IAPSR		0x505F
FLASH_DUKR		0x5064

FLASH_DUL_BIT		 	BIT3
FLASH_EOP_BIT		 	BIT2
FLASH_WR_PG_DIS_BIT	 	BIT0


*/

#include <stdint.h>
#include <stddef.h>
#include "eeprom.h"


/////////////////////////////////////
//Dummy delay
void EEPROM_delay(uint32_t temp)
{
    volatile uint32_t time = temp;

    while (time > 0)
        time--;
}

///////////////////////////////////
//Unlocks the EEPROM data 
void EEPROM_init(void)
{
    uint8_t result = EEPROM_unlock();

    if (!result)
    {
        while (1);
    }

}


//////////////////////////////////////////
//unlock the data section in program memory
//tries up to 10 times to unlock, if successful
//returns 1, else, returns 0
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
            EEPROM_Delay(1000);
            FLASH_DUKR = 0xAE;
            EEPROM_Delay(1000);
            FLASH_DUKR = 0x56;
            EEPROM_Delay(1000);

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
//Lock the data section in program memory
uint8_t EEPROM_lock(void)
{
    //clear the DUL bit on the FLASH_IAPSR register
    FLASH_IAPSR &=~ FLASH_DUL_BIT;

    if (FLASH_IAPSR & FLASH_DUL_BIT)
        return 1;
    else
        return 0;
}


//////////////////////////////////////////////////
//write 8bit data to address.   Address is assumed
//to start at base address = 0x4000.  ie, pass 0x00
//for 0x4000, 0x01 for 0x4001...etc
uint8_t EEPROM_writeByte(uint16_t address, uint8_t data)
{
    uint16_t result = EEPROM_BASE_ADDRESS + address;
    uint8_t readback = 0x00;
    
    //test the FLASH_DUL_BIT, if set:
    if (FLASH_IAPSR & FLASH_DUL_BIT)
    {
        *((volatile uint8_t*)result) = data;
    
        //wait
        while (!(FLASH_IAPSR & FLASH_EOP_BIT)){};

        //read it back...
        readback = EEPROM_readByte(address);

        if (readback == data)
            return 1;
        else
            return 0;
    }

    return 0;
}

uint8_t EEPROM_readByte(uint16_t address)
{
    uint16_t target = EEPROM_BASE_ADDRESS + address;
    uint8_t result = 0x00;
    
    result = *((volatile uint8_t*)target);

    return result;
}





