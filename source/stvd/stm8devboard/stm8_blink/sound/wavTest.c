/*
4/23/20
Test Sound File

*/

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#include "sound.h"


//make a test sound
const unsigned char pSoundTest1[] = {
127, 149, 170, 191, 209, 224, 237, 246,
252, 254, 252, 246, 237, 224, 208, 191,
170, 149, 127, 105,  83,  64,  45,  30,
17,    8,   2,   0,   2,   8,  17,  30,
45,   64,  84, 105
};

const SoundData soundTest1 =
{
    36,
    (const unsigned char*) pSoundTest1
};




const unsigned char pSoundTest2[] = {
2047,2402,2747,3070,3363,3615,3820,3971,
4063,4094,4063,3971,3820,3615,3363,3071,
2747,2402,2047,1691,1347,1024,731,479,
274,123,31,0,31,123,274,479,
731,1024,1347,1692

};

const SoundData soundTest2 =
{
    36,
    (const unsigned char*) pSoundTest2
};


