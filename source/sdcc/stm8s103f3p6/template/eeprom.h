/*
EEPROM Controller File for Use with 
the STM8SF103 Processor.  

*/


#ifndef __EEPROM__H
#define __EEPROM__H

#include <stdint.h>
#include <stddef.h>

void EEPROM_delay(uint32_t temp);
void EEPROM_init(void);
uint8_t EEPROM_unlock(void);
uint8_t EEPROM_lock(void);
uint8_t EEPROM_writeByte(uint16_t address, uint8_t data);
uint8_t EEPROM_readByte(uint16_t address);

#endif

