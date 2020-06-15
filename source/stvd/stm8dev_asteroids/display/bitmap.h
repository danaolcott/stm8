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

extern const ImageData bmimgPlayer0Bmp;
extern const ImageData bmimgPlayer26Bmp;
extern const ImageData bmimgPlayer45Bmp;
extern const ImageData bmimgPlayer63Bmp;
extern const ImageData bmimgPlayer90Bmp;
extern const ImageData bmimgPlayer116Bmp;
extern const ImageData bmimgPlayer135Bmp;
extern const ImageData bmimgPlayer153Bmp;
extern const ImageData bmimgPlayer180Bmp;
extern const ImageData bmimgPlayer206Bmp;
extern const ImageData bmimgPlayer225Bmp;
extern const ImageData bmimgPlayer243Bmp;
extern const ImageData bmimgPlayer270Bmp;
extern const ImageData bmimgPlayer296Bmp;
extern const ImageData bmimgPlayer315Bmp;
extern const ImageData bmimgPlayer333Bmp;

extern const ImageData bmimgPlayerExp1Bmp;
extern const ImageData bmimgPlayerExp2Bmp;
extern const ImageData bmimgPlayerExp3Bmp;

extern const ImageData bmimgAsteroidSMBmp;
extern const ImageData bmimgAsteroidMDBmp;

extern const ImageData bmimgPlayerIcon_1;
extern const ImageData bmimgPlayerIcon_2;
extern const ImageData bmimgPlayerIcon_3;



#endif /* BITMAP_H_ */
