////////////////////////////////////////////
/*
Image Name: imgAsteroidMD.bmp
MonoChrome Image 1 Bit Per Pixel
Width: 16
Height: 16
Pixel Format: Format32bppArgb
*/
///////////////////////////////////////////////////



#include <stdlib.h>
#include "bitmap.h"

static const uint8_t _acimgAsteroidMDBmp[] =
{
0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x1F, 0xC0, 0x3F, 0xF0,
0x3F, 0xF0, 0x3F, 0xF8, 0x3F, 0xFC, 0x1F, 0xFC, 0x1F, 0xF8,
0x1F, 0xF8, 0x0F, 0xF8, 0x0F, 0xF8, 0x03, 0xF0, 0x01, 0xE0,
0x00, 0x00};


const ImageData bmimgAsteroidMDBmp = {
16, //xSize
16, //ySize
2, //bytesPerLine
1, //bits per pixel
(uint8_t*)_acimgAsteroidMDBmp,
};
/////////////////// End of File  ///////////////////////////