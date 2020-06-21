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

#include "register.h"


typedef struct ImageData
{
    uint8_t xSize;              // pixels in x-direction
    uint8_t ySize;              // pixels in y-direction
    uint8_t bytesPerLine;
    uint8_t bitsPerPixel;
    const uint8_t * const pImageData;
};

typedef struct ImageData ImageData;


extern const ImageData bmimgPlayerIcon_1;
extern const ImageData bmimgPlayerIcon_2;
extern const ImageData bmimgPlayerIcon_3;
extern const ImageData bmimgPlayer_24;
extern const ImageData bmimgBall_8;
extern const ImageData bmimgTile_8;
extern const ImageData bmimgTileBlank_8;


#endif /* BITMAP_H_ */
