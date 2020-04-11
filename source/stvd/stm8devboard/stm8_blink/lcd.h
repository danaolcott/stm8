/*
4/6/20
LCD Controller File
*/


#ifndef __LCD_H
#define __LCD_H


#define LCD_CMD_PIN			BIT_4
#define LCD_RST_PIN			BIT_5

#define LCD_HEIGHT		64
#define LCD_WIDTH		102
#define LCD_NUM_PAGES	8


void lcd_init(void);
void lcd_dummyDelay(unsigned long delay);
void lcd_reset(void);
void lcd_writeCommand(uint8_t cmd);
void lcd_writeData(uint8_t data);
void lcd_writeDataBurst(uint8_t *data, uint16_t length);

void lcd_setPage(uint8_t page);
void lcd_setColumn(uint8_t column);
void lcd_clear(uint8_t value);




#endif

