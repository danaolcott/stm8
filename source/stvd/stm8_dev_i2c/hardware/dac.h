/*
4/20/20
DAC Controller File
For use with the sound output
*/


#ifndef __DAC_H
#define __DAC_H


void DAC_init(void);
void DAC_set(unsigned int value);
void DAC_set8Bit(unsigned char value);

#endif

