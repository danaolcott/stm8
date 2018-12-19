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


///////////////////////////////////
//Unlocks the EEPROM data 
void EEPROM_init(void)
{
}

void EEPROM_writeByte(uint8_t data)
{
}

uint8_t EEPROM_readByte(void)
{
}

