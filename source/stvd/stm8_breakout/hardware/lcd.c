/*
4/6/20
LCD Controller File for the EA Display DOGS102N-6 LCD
102x64 Display with SPI Interface.

*/

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#include "register.h"
#include "lcd.h"
#include "spi.h"
#include "font_table.h"
#include "bitmap.h"


#define abs(a)  ((a)<0?-(a):a)

/////////////////////////////////////////////////////
//Framebuffer allocated to memory location 0x100
//0x100u is the start of .bss
//Note: define the address when using the short stack
//memory mode, leave alone if using the tall stack memory
//model.  
volatile uint8_t frameBuffer[FRAME_BUFFER_SIZE];// @ 0x100u;

/////////////////////////////////////////////////////
//Configure the CD and RST pins, and register settings
//PA4 - LCD_CD
//PA5 - LCD_RST
void lcd_init(void)
{
    PA_ODR &=~ BIT_4;
    PA_ODR &=~ BIT_5;
    
    PA_DDR |= BIT_4;
    PA_CR1 |= BIT_4;
    PA_CR2 &=~ BIT_4;

    PA_DDR |= BIT_5;
    PA_CR1 |= BIT_5;
    PA_CR2 &=~ BIT_5;

    lcd_reset();

    //Configure the registers - See datasheet
	lcd_writeCommand(0x40);		//Display start line 0
	lcd_writeCommand(0xA1);		//SEG reverse
	lcd_writeCommand(0xC0);		//Normal COM0-COM63
	lcd_writeCommand(0xA4);		//Set all pixels to ON
	lcd_writeCommand(0xA6);		//Display inverse off
	lcd_writeCommand(0xA2);		//Set bias 1/9 (Duty 1/65)
	lcd_writeCommand(0x2F);		//Booster, Regulator and Follower on
	lcd_writeCommand(0x27);		//Set Contrast - internal resistor ratio- max, base = 0x20
	lcd_writeCommand(0x81);		//Set Contrast - contrast, cmd 1
	lcd_writeCommand(0x0E);		//Set Contrast - value, 6 bits	
	lcd_writeCommand(0xFA);		//Set temperature compensation
	lcd_writeCommand(0x90);		//Set temperature compensation	
	lcd_writeCommand(0xAF);		//Display on

    lcd_clear(0x00);
    lcd_clearFrameBuffer(0x00, 0);
}



void lcd_dummyDelay(unsigned long delay)
{
    volatile unsigned long temp = delay;
    while (temp > 0)
    {
        temp--;     
    }
}

//////////////////////////////////////////////
//LCD Reset - toggle the RST pin
void lcd_reset(void)
{
    PA_ODR |= LCD_RST_PIN;      //high
    PA_ODR &=~ LCD_RST_PIN;     //low
    lcd_dummyDelay(5000);       //about 100ms
    PA_ODR |= LCD_RST_PIN;      //high
}

void lcd_writeCommand(uint8_t cmd)
{
    PA_ODR &=~ LCD_CMD_PIN;       //CMD = low
    SPI_writeByte(cmd);
}

void lcd_writeData(uint8_t data)
{
    PA_ODR |= LCD_CMD_PIN;       //data = high
    SPI_writeByte(data);
}

void lcd_writeDataBurst(uint8_t *data, uint16_t length)
{
    PA_ODR |= LCD_CMD_PIN;       //data = high
    SPI_writeArray(data, length);
}



/////////////////////////////////////////////
//Set Page - 0 to 7, top to bottom.
//Base address = 0xB0, page address is
//the lower 4 bits
void lcd_setPage(uint8_t page)
{
    uint8_t result = 0xB0 | (page & 0x0F);
    lcd_writeCommand(result);
}


////////////////////////////////////////////
//Set Column - x offset
void lcd_setColumn(uint8_t column)
{
    uint8_t low = 0x00 | (column & 0x0F);       //lower 4 bits
    uint8_t high = 0x10 | (column >> 4);        //upper 4 bits
    lcd_writeCommand(low);                      //set the low bits
    lcd_writeCommand(high);                     //set the high bits 
}


///////////////////////////////////////////
//LCD Clear - clear the display with a value
void lcd_clear(uint8_t value)
{
    uint8_t i, j;

    for (i = 0 ; i < LCD_NUM_PAGES ; i++)
    {
        lcd_setPage(i);         //increment the page
        lcd_setColumn(0);       //reset the x
        
        for (j = 0 ; j < LCD_WIDTH ; j++)
            lcd_writeData(value);
    }
}


void lcd_clearPage(uint8_t page, uint8_t value)
{
	uint8_t i = 0x00;
	lcd_setColumn(0);
	lcd_setPage(page);
	
	for (i = 0 ; i < LCD_WIDTH ; i++)
			lcd_writeData(value);	
}

//////////////////////////////////////////////
//Clears the framebuffer memory and updates
//the contents of the display within the 
//framebuffer region only
//if update == 1, update the frame buffer with
//contents of the display
void lcd_clearFrameBuffer(uint8_t value, uint8_t update)
{
	uint8_t i, j;
	uint16_t index = 0x00;
	
	for (index = 0 ; index < FRAME_BUFFER_SIZE ; index++)
		frameBuffer[index] = value;
	
	//update the contents of the display
	if (update == 1)
	{
		index = 0x00;
		
		lcd_setColumn(FRAME_BUFFER_OFFSET_X);
		lcd_setPage(FRAME_BUFFER_START_PAGE);
		
		for (i = FRAME_BUFFER_START_PAGE ; i < FRAME_BUFFER_STOP_PAGE + 1 ; i++)
		{
			lcd_setColumn(FRAME_BUFFER_OFFSET_X);	//reset the x
			lcd_setPage(i);							//increment the page
			
			for (j = 0 ; j < FRAME_BUFFER_WIDTH ; j++)
				lcd_writeData(frameBuffer[index++]);
		}
	}
}



//////////////////////////////////////////////
//Clears everything except the framebuffer 
//area, the score page (full width) and player
//page (full width).  Basically, the left and right
//margins over the height of the frame buffer
void lcd_clearBackground(uint8_t value)
{
	uint8_t i, j;			

	for (i = FRAME_BUFFER_START_PAGE ; i < FRAME_BUFFER_STOP_PAGE + 1 ; i++)
	{
		lcd_setPage(i);
		
		//left margin
		lcd_setColumn(0);		
		for (j = 0 ; j < FRAME_BUFFER_OFFSET_X ; j++)
			lcd_writeData(value);
		
		//right margin
		lcd_setColumn(LCD_WIDTH - FRAME_BUFFER_OFFSET_X);
		for (j = 0 ; j < FRAME_BUFFER_OFFSET_X ; j++)
			lcd_writeData(value);
	}
}


//////////////////////////////////////////////
//Update the display with the contents of the
//framebuffer.  The buffer is displayed at 
//FRAME_BUFFER_OFFSET_X and FRAME_BUFFER_START_PAGE
//over a height of FRAME_BUFFER_NUM_PAGES
//
void lcd_updateFrameBuffer(void)
{
	uint8_t i, j;
	uint16_t element = 0;
	
	volatile uint8_t *ptr = frameBuffer;
		
	for (i = FRAME_BUFFER_START_PAGE ; i < FRAME_BUFFER_STOP_PAGE + 1 ; i++)
	{
		lcd_setColumn(FRAME_BUFFER_OFFSET_X);
		lcd_setPage(i);
		lcd_writeDataBurst(ptr, FRAME_BUFFER_WIDTH);		
		ptr += FRAME_BUFFER_WIDTH;
	}	
}




////////////////////////////////////////////////////
//LCD_drawString.
//Print string of characters to the LCD.
//Note: the far keyword is required for this to 
//work since the project is using the tiny memory model.
//If we were using the other memory model, I think you dont 
//need it because the variable would be placed into far region
//of memory by default.  Leaving the keyword out crashes the program
//and using near keyword crashes the program.
//When calling this function, there is no need to cast the input
//parameter as char *far.
void lcd_drawString(uint8_t row, uint8_t col, char *myString)
{
	uint8_t count = 0;
	uint8_t position = col;

	unsigned int line;
	unsigned int value0;
	uint8_t width = 8;
	unsigned int i = 0;
	
	//set the x and y start positions
	lcd_setPage(row);
	
	while ((myString[count] != 0x00) && (position < LCD_WIDTH))
	{
		lcd_setColumn(position);

		if ((position + width) < LCD_WIDTH)
		{
			line = myString[count] - 27;		//ie, for char 32 " ", it's on line 5
			value0 = (line-1) << 3;

			for (i = 0 ; i < width ; i++)
			{
				lcd_writeData(font_table[value0 + i]);
			}
		}

		position += width;
		count++;
	}
}



//////////////////////////////////////////////////
//Draws a string of characters at starting
//row and col.
void lcd_drawStringLength(uint8_t row, uint8_t col, char *mystring, uint8_t length)
{
	unsigned int i, j = 0;
	unsigned char position = col;
	uint8_t width = 8;
	unsigned int value0 = 0;
	unsigned int line = 0;

	//set the x and y start positions
	lcd_setPage(row);

	for (i = 0 ; i < length ; i++)
	{
		lcd_setColumn(position);

		if ((position + width) < LCD_WIDTH)
		{
			line = mystring[i] - 27;		//ie, for char 32 " ", it's on line 5
			value0 = (line-1) << 3;

			for (j = 0 ; j < width ; j++)
			{
				lcd_writeData(font_table[value0 + j]);
			}
		}

		position += width;
	}
}



//////////////////////////////////////////
//Print val into buffer of max size size.
//returns the number of characters in the
//the buffer.  Assumes the buffer
//is clear prior to writing to it.
uint8_t lcd_decimalToBuffer(unsigned int val, char *buffer, uint8_t size)
{
    uint8_t i = 0;
    char digit;
    uint8_t num = 0;
    char t;
    
    //test for a zero value
    if (val == 0)
    {
    	buffer[0] = '0';
    	buffer[1] = 0x00;
    	return 1;
    }

    while (val > 0)
    {
        digit = (char)(val % 10);
        buffer[num] = (0x30 + digit) & 0x7F;
        num++;
        val = val / 10;
    }

    //reverse in place
    for (i = 0 ; i < num / 2 ; i++)
    {
        t = buffer[i];
        buffer[i] = buffer[num - i - 1];
        buffer[num - i - 1] = t;
    }

    buffer[num] = 0x00;     //null terminated

    return num;
}




/////////////////////////////////////////////////////////
//LCD_putPixelRam
//Modifies a single bit in the framebuffer and updates
//the display when update = 1
//Note:
//Framebuffer width is not the same as LCD_WIDTH
//all x and y aligned to the edge of the frame buffer
//
void lcd_putPixelRam(uint16_t x, uint16_t y, uint8_t color, uint8_t update)
{
	uint16_t element = 0x00;    //frame buffer element
	uint8_t elementValue = 0x00;    
	uint8_t bitShift = 0x00;

    //test for valid input
	if ((x > (FRAME_BUFFER_WIDTH - 1)) || (y > (FRAME_BUFFER_HEIGHT - 1)))
		return;
	
	//element the frame buffer to read / write
	element = ((y >> 3) * FRAME_BUFFER_WIDTH) + x;
	
	//offset - MSB on bottom
	if (y < 8)
		bitShift = (uint8_t)y;
	else if ((y % 8) == 0)
		bitShift = 0;
	else
		bitShift = y % 8;
	
	//read
	elementValue = frameBuffer[element];
	
	//modify
	if (color == 1)
		elementValue |= (1 << bitShift);        //add 1
	else    
		elementValue &=~ (1 << bitShift);       //clear 1
	
    //write
    frameBuffer[element] = elementValue;
	
	//update
	if (update > 0)
	{
		//update the display - offset by position
		//of the framebuffer
		lcd_setColumn(x + FRAME_BUFFER_OFFSET_X);
		lcd_setPage((y >> 3) + FRAME_BUFFER_START_PAGE);
		lcd_writeData(elementValue);
	}	
}






//////////////////////////////////////////////
//Draw bitmap functions -
//draws image data into frame buffer.  update set to 
//1 updates the display after the ram is updated.
//images assumed to be 1 bit per pixel, aligned 
//left to right, multiple of 8 in width.  offset is from
//edge of frame buffer
//transparency assumed to be 1, such that it only draws
//dark pixels
void lcd_drawIcon(uint32_t xPosition, uint32_t yPosition, const ImageData *pImage, uint8_t update)
{
    uint8_t bitValue = 0;
	uint8_t p = 0;
	uint16_t i = 0;
	uint16_t j = 0;
	uint16_t counter = 0x00;
	uint8_t data = 0x00;
	uint8_t sizeX = 0x00;
	uint8_t sizeY = 0x00;    
	uint16_t x = xPosition;
	uint16_t y = yPosition;
    
	//set the pointer
	uint8_t *ptr = pImage->pImageData;
	sizeX = pImage->xSize;
	sizeY = pImage->ySize;
	         
    for (i = 0 ; i < sizeY ; i++)
    {
		x = xPosition;        //reset the x position
		
		//1bpp - 8 pixels per element
		for (j = 0 ; j < (sizeX / 8) ; j++)
		{   
			data = ptr[counter];
			
			//work counter 8 times
			p = 8;                  //reset p
			while (p > 0)
			{                
				bitValue = (data >> (p-1)) & 0x01;
				
				if (bitValue == 1)
				{
					lcd_putPixelRam(x, y, 1, 0);
				}
				
				x++;            //increment the x
				p--;
			}
			
			counter++;
		}
		
		y++;        //increment the row
	}
    
    //check for update == 1, update the display
    if (update == 1)
    {
        lcd_updateFrameBuffer();
    }
}

////////////////////////////////////////////////////
//Draw bitmap function - 
//Draws image data into ram frame buffer, updates the
//display after the image is updated in RAM when update
//is set to 1.  Image is wrapped around the screen.  Same
//as above function, just that the image wraps.
//images assumed to be 1 bit per pixel, aligned 
//left to right, multiple of 8 in width
//transparency assumed to be 1, such that it only draws
//dark pixels
void lcd_drawIconWrap(uint32_t xPosition, uint32_t yPosition, const ImageData *pImage, uint8_t update)
{
    uint8_t bitValue = 0;
	uint8_t p = 0;
	uint16_t i = 0;
	uint16_t j = 0;
	uint16_t counter = 0x00;
	uint8_t data = 0x00;
	uint8_t sizeX = 0x00;
	uint8_t sizeY = 0x00;    
	uint16_t x = xPosition;
	uint16_t y = yPosition;
    
	//set the pointer
	uint8_t *ptr = pImage->pImageData;
	sizeX = pImage->xSize;
	sizeY = pImage->ySize;
	         
    for (i = 0 ; i < sizeY ; i++)
    {
		x = xPosition;        //reset the x position
		
		//1bpp - 8 pixels per element
		for (j = 0 ; j < (sizeX / 8) ; j++)
		{   
			data = ptr[counter];
			
			//work counter 8 times
			p = 8;                  //reset p
			while (p > 0)
			{                
				bitValue = (data >> (p-1)) & 0x01;
				
				if (bitValue == 1)
				{
					lcd_putPixelRam(x, y, 1, 0);
				}
				
                //increment or wrap
                if (x < FRAME_BUFFER_WIDTH - 1)
                    x++;            //increment the x
                else
                    x = 0;
                    
				p--;
			}
			
			counter++;
		}

        if (y < FRAME_BUFFER_HEIGHT - 1)
            y++;        //increment the row
        else
            y = 0;
	}
    
    //check for update == 1, update the display
    if (update == 1)
    {
        lcd_updateFrameBuffer();
    }
}










