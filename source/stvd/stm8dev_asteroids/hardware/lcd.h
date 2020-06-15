/*
4/6/20
LCD Controller File for the EA Display DOGS102N-6 LCD
102x64 Display with SPI Interface.
*/


#ifndef __LCD_H
#define __LCD_H

#include "register.h"
#include "bitmap.h"


#define LCD_CMD_PIN			BIT_4
#define LCD_RST_PIN			BIT_5

#define LCD_HEIGHT		64
#define LCD_WIDTH		102
#define LCD_NUM_PAGES	8

//////////////////////////////////////////////
//Note:  Frame buffer is not the full size of the
//LCD due to memory constraints.
#define FRAME_BUFFER_SIZE		714


#define FRAME_BUFFER_WIDTH		102
#define FRAME_BUFFER_HEIGHT		56
#define FRAME_BUFFER_OFFSET_X	0
#define FRAME_BUFFER_OFFSET_Y	0
#define FRAME_BUFFER_START_PAGE	1
#define FRAME_BUFFER_STOP_PAGE	7
#define FRAME_BUFFER_NUM_PAGES	7

#define LCD_SCORE_PAGE				0


void lcd_init(void);
void lcd_dummyDelay(unsigned long delay);
void lcd_reset(void);
void lcd_writeCommand(uint8_t cmd);
void lcd_writeData(uint8_t data);
void lcd_writeDataBurst(uint8_t *data, uint16_t length);

void lcd_setPage(uint8_t page);
void lcd_setColumn(uint8_t column);
void lcd_clear(uint8_t value);

void lcd_clearPage(uint8_t page, uint8_t value);
void lcd_clearFrameBuffer(uint8_t value, uint8_t update);
void lcd_clearBackground(uint8_t value);
void lcd_updateFrameBuffer(void);

void lcd_drawString(uint8_t row, uint8_t col, char *myString);
void lcd_drawStringLength(uint8_t row, uint8_t col, char *mystring, uint8_t length);

uint8_t lcd_decimalToBuffer(unsigned int val, char *buffer, uint8_t size);


//functions that manipulate the framebuffer
void lcd_putPixelRam(uint16_t x, uint16_t y, uint8_t color, uint8_t update);


//carryover draw image functions
void lcd_drawIcon(uint32_t xPosition, uint32_t yPosition, const ImageData *pImage, uint8_t update);
void lcd_drawIconWrap(uint32_t xPosition, uint32_t yPosition, const ImageData *pImage, uint8_t update);






#endif

