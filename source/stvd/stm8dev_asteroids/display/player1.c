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
/////////////////////////////////////////////////////////
//images of the player ship go here
///////////////////////////////////////////////////////////



/////////////////////////////////////////////////
//Player icons - 3 players
const uint8_t _acimgPlayerIcon_3[] = 
{
0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00
};

const ImageData bmimgPlayerIcon_3 = {
	8,
	8,
    1,
    1,
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
    1,
    1,
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
    1,
    1,
	(uint8_t*)_acimgPlayerIcon_1,
};






