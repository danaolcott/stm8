////////////////////////////////////////////////
//5/21/20
//Dana Olcott
//
//Images for enemy.  These are listed left to right,
//one bit per pixel.  Intended to be drawn into framebuffer.
//Note: Not the same as player images, which are drawn
//LSB at top, page aligned, left to right
//
#include "bitmap.h"
#include "register.h"


const uint8_t _acenemy1Bmp[] =
{
0x08, 0x20, 0x04, 0x40, 0x0F, 0xE0, 0x1B, 0xB0,
0x3F, 0xF8, 0x2F, 0xE8, 0x28, 0x28, 0x06, 0xC0};


const ImageData bmenemy1Bmp = 
{
    16, //xSize
    8, //ySize
    (uint8_t*)_acenemy1Bmp,
};
