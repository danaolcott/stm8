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
//Player icons - 3 players - vertically aligned
const uint8_t _acimgPlayerIcon_3[] = 
{
0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00
};

const ImageDataVertical bmimgPlayerIcon_3 = {
	8,
	1,
	(uint8_t*)_acimgPlayerIcon_3,
};


  


/////////////////////////////////////////////////
//Player icons - 2 players - vertically aligned
const uint8_t _acimgPlayerIcon_2[] = 
{
0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00
};

const ImageDataVertical bmimgPlayerIcon_2 = {
	8,
    1,
	(uint8_t*)_acimgPlayerIcon_2,
};



/////////////////////////////////////////////////
//Player icons - 1 players - vertically aligned
const uint8_t _acimgPlayerIcon_1[] = 
{
0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00
};

const ImageDataVertical bmimgPlayerIcon_1 = {
	8,
    1,
	(uint8_t*)_acimgPlayerIcon_1,
};


////////////////////////////////////////////
//Player - Simple solid line, 24 pixels
//wide by 2 pixels high, drawn left to right
const uint8_t _player_24[] = {
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF    
};

const ImageData bmimgPlayer_24 = {
    24,
    2,
    3,
    1,
    (uint8_t*)_player_24,
};


////////////////////////////////////////////
//Player - Simple solid line, 16 pixels
//wide by 2 pixels high, drawn left to right
const uint8_t _player_16[] = {
0xFF, 0xFF, 0xFF, 0xFF    
};

const ImageData bmimgPlayer_16 = {
    16,
    2,
    2,
    1,
    (uint8_t*)_player_16,
};


///////////////////////////////////////////////
//Ball image 8x8
//
const uint8_t _ball_8[] = {
0x00, 0x00, 0x3C, 0x7E, 0x7E, 0x3C, 0x00, 0x00
};

const ImageData bmimgBall_8 = {
    8,
    8,
    1,
    1,
    (uint8_t*)_ball_8,
};


/////////////////////////////////////////////////////
//Default Tile - 8x8
const uint8_t _tile_8[] = {
0xFF, 0x81, 0xFF    
};

const ImageData bmimgTile_8 = {
    8,
    3,
    1,
    1,
    (uint8_t*)_tile_8,
};



/////////////////////////////////////////////////////
//Default Tile - 8x8
const uint8_t _tileBlank_8[] = {
0x00, 0x00, 0x00    
};

const ImageData bmimgTileBlank_8 = {
    8,
    3,
    1,
    1,
    (uint8_t*)_tileBlank_8,
};






