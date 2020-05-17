/*
5/11/20
EEPROM Controller File
*/


#ifndef __EEPROM_H
#define __EEPROM_H

#include "register.h"

#define EEPROM_START_ADDR			0x1000
#define EEPROM_STOP_ADDR			0x13FF
#define EEPROM_HW_KEY1				0xAE
#define EEPROM_HW_KEY2				0x56



void EEPROM_delay(uint16_t delay);
void EEPROM_init(void);
uint8_t EEPROM_unlock(void);
uint8_t EEPROM_lock(void);
uint8_t EEPROM_write(uint16_t address, uint8_t data);
uint8_t EEPROM_read(uint16_t address);

#endif

