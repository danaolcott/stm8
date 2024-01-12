/*
 * Dana Olcott
 * 1/7/24
 * 
 * Images of the player icons (how many players remaining), enemy
   ships, etc.  images are aligned left to right, 16 pixels x 8 pixels.
   these are intended to be drawn into ram buffer and then the lcd
   updated with the contents of the buffer.  Player icons drawn assuming
   page type display with LSB at the top, MSB at the bottom of the page
 *
 */


#include "bitmap.h"



/////////////////////////////////////////////////
//Player icons - 3 players
//images showing how many ships are remaining
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
//Player icons - 2 players
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
//Player icons - 1 players
const uint8_t _acimgPlayerIcon_1[] = 
{
0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00
};

const ImageDataVertical bmimgPlayerIcon_1 = {
	8,
    1,
	(uint8_t*)_acimgPlayerIcon_1,
};




///////////////////////////////////////////////
//Player ship


const uint8_t _imgPlayerShip[] = 
{
0xFF, 0x80, 0xE0, 0x00, 0xFA, 0x00, 0xFF, 0x00,
0xFF, 0xFC, 0xFA, 0x00, 0xE0, 0x00, 0xFF, 0x80
};

const ImageData imgPlayerShip = {
    16,
    8,
    2,
    1,
    (uint8_t*)_imgPlayerShip
};


////////////////////////////////////////////
//Enemy1
const uint8_t _imgEnemy1[] = 
{
0xFF, 0xFF, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0xFF, 0xFF
};

const ImageData imgEnemy1 = {
    16,
    8,
    2,
    1,
    (uint8_t*)_imgEnemy1
};

////////////////////////////////////////////
//Enemy2
const uint8_t _imgEnemy2[] = 
{
0x08, 0x20, 0x04, 0x40, 0x0F, 0xE0, 0x1B, 0xB0,
0x3F, 0xF8, 0x2F, 0xE8, 0x28, 0x28, 0x06, 0xC0
};

const ImageData imgEnemy2 = {
    16,
    8,
    2,
    1,
    (uint8_t*)_imgEnemy2
};

////////////////////////////////////////////
//Enemy3
const uint8_t _imgEnemy3[] = 
{
0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA
};

const ImageData imgEnemy3 = {
    16,
    8,
    2,
    1,
    (uint8_t*)_imgEnemy3
};









