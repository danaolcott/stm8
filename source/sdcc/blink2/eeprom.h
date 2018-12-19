/*
EEPROM Controller File for Use with 
the STM8SF103 Processor.  

*/


#ifndef __EEPROM__H
#define __EEPROM__H

#include <stdint.h>
#include <stddef.h>

void EEPROM_init(void);
void EEPROM_writeByte(uint8_t data);
uint8_t EEPROM_readByte(void);


#endif

