/*
 * bitmap.h
 *
 *  Created on: Sep 4, 2019
 *      Author: danao
 *  
 *  Icons and Images Used in the Game
 */

#ifndef BITMAP_H_
#define BITMAP_H_

#include <stdint.h>

////////////////////////////////////////
//ImageData
//Assumes aligned horizontal
typedef struct ImageData
{
    uint8_t xSize;              // pixels in x-direction
    uint8_t ySize;              // pixels in y-direction
    uint8_t bytesPerLine;
    uint8_t bitsPerPixel;
    const uint8_t * const pImageData;
};

typedef struct ImageData ImageData;


//////////////////////////////////////////
//ImageDataVertical
//Assumes aligned top to bottom, LSB first
typedef struct ImageDataVertical
{
    uint8_t xSize;              // pixels in x-direction
    uint8_t numPages;           // num pages
    const uint8_t * const pImageData;
};

typedef struct ImageDataVertical ImageDataVertical;



//used for showing how many players left
extern const ImageDataVertical bmimgPlayerIcon_1;
extern const ImageDataVertical bmimgPlayerIcon_2;
extern const ImageDataVertical bmimgPlayerIcon_3;


extern const ImageData imgPlayerShip;

extern const ImageData imgEnemy1;
extern const ImageData imgEnemy2;
extern const ImageData imgEnemy3;
extern const ImageData imgEnemy4;






#endif /* BITMAP_H_ */
