/*
Utility Functions

*/

#ifndef __UTILITY__H
#define __UTILITY__H

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

uint8_t utility_decimal2Buffer(uint16_t value, uint8_t* output);
uint8_t utility_data2HexBuffer(uint8_t* input, uint8_t len, uint8_t* output);


#endif

