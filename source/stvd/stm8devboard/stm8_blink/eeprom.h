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

//game addresses
#define EEPROM_ADDRESS_CYCLE_COUNT_MSB			((uint16_t)0x02)
#define EEPROM_ADDRESS_CYCLE_COUNT_LSB			((uint16_t)0x03)

#define EEPROM_ADDRESS_HIGH_SCORE_MSB           ((uint16_t)0x04)
#define EEPROM_ADDRESS_HIGH_SCORE_LSB           ((uint16_t)0x05)


void EEPROM_delay(uint16_t delay);
void EEPROM_init(void);
uint8_t EEPROM_unlock(void);
uint8_t EEPROM_lock(void);
uint8_t EEPROM_write(uint16_t address, uint8_t data);
uint8_t EEPROM_read(uint16_t address);

void EEPROM_clearEEPROM(uint8_t value);
uint16_t EEPROM_updateCycleCount(void);
uint16_t EEPROM_readCycleCount(void);

uint16_t EEPROM_getHighScore(void);
void EEPROM_updateHighScore(uint16_t score);



#endif

