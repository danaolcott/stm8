/*
 * Dana Olcott
 * 9/7/19
 * 
 * Images of the player ship, 16x8.  Images are converted assuming
 * a page-type display will be used with LSB at the top, MSB at the
 * bottom.  ie, one bit per pixel, 8 pixels vertical per byte.  Images
 * are a single page in height.  The original image is upside down because
 * the converter puts the MSB at the top and LSB at the bottom.
 * 
 * NOTE: 
 * Use the (uint8_t *far) cast due to the tiny memory model
 * used for this nxp processor. (see repo for nxp MC9S08QE8)
 *
 */


#include "bitmap.h"
#include "register.h"

///////////////////////////////////////////////////////
//Player ship
const uint8_t _acimgPlayerInvBmp[] =
{
0xC0, 0xF8, 0xF0, 0xF0, 0xF8, 0xF8, 0xFC, 0xFE, 0xFE, 0xFC,
0xF8, 0xF8, 0xF0, 0xF0, 0xF8, 0xC0};


const ImageData bmimgPlayerInvBmp = {
16, //xSize
8, //ySize
(uint8_t*)_acimgPlayerInvBmp,
};



////////////////////////////////////////////////////////
//Player ship - explosion1
const uint8_t _acimgPlayerInvExp1Bmp[] =
{
0xC0, 0xF9, 0xF0, 0xE1, 0xE0, 0xF4, 0xFE, 0xFE, 0xFE, 0xFC,
0xE0, 0xE0, 0xF5, 0xF2, 0xF8, 0xC0};


const ImageData bmimgPlayerInvExp1Bmp = {
16, //xSize
8, //ySize
(uint8_t*)_acimgPlayerInvExp1Bmp,
};


///////////////////////////////////////////////////////
//Player ship - explosion 2
const uint8_t _acimgPlayerInvExp2Bmp[] =
{
0xC0, 0x89, 0xC4, 0x80, 0xE0, 0xF4, 0xFF, 0xE6, 0xDB, 0xE9,
0xE1, 0xE0, 0xF0, 0xF0, 0xC8, 0xC1};


const ImageData bmimgPlayerInvExp2Bmp = {
16, //xSize
8, //ySize
(uint8_t*)_acimgPlayerInvExp2Bmp,
};


///////////////////////////////////////////////////////
//Player ship - explosion 3
const uint8_t _acimgPlayerInvExp3Bmp[] =
{
0xC0, 0x88, 0xC4, 0x81, 0x80, 0xC4, 0x9D, 0xA4, 0xCA, 0xA0,
0xE0, 0x80, 0x90, 0x80, 0xC8, 0xC2};


const ImageData bmimgPlayerInvExp3Bmp = {
16, //xSize
8, //ySize
(uint8_t*)_acimgPlayerInvExp3Bmp,
};


/////////////////////////////////////////////////////
//Player ship - explosion 4
const uint8_t _acimgPlayerInvExp4Bmp[] =
{
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


const ImageData bmimgPlayerInvExp4Bmp = {
16, //xSize
8, //ySize
(uint8_t*)_acimgPlayerInvExp4Bmp,
};




/////////////////////////////////////////////////
//Player icons - 3 players
const uint8_t _acimgPlayerIcon_3[] = 
{
0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00
};

const ImageData bmimgPlayerIcon_3 = {
	8,
	8,
	(uint8_t*)_acimgPlayerIcon_3,
};



/////////////////////////////////////////////////
//Player icons - 2 players
const uint8_t _acimgPlayerIcon_2[] = 
{
0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00
};

const ImageData bmimgPlayerIcon_2 = {
	8,
	8,
	(uint8_t*)_acimgPlayerIcon_2,
};



/////////////////////////////////////////////////
//Player icons - 1 players
const uint8_t _acimgPlayerIcon_1[] = 
{
0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00
};

const ImageData bmimgPlayerIcon_1 = {
	8,
	8,
	(uint8_t*)_acimgPlayerIcon_1,
};






