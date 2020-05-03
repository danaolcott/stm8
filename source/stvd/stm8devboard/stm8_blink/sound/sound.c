/*
4/20/20
Sound Controller File for output
sound to DAC PB4.  Works with Timer 2

*/

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#include "register.h"
#include "sound.h"
#include "dac.h"
#include "timer.h"


////////////////////////////////////
//Sound variables, counter, etc
static unsigned int mSoundCounter = 0x00;   //down counter for sound array
static unsigned char mSoundLoopCounter = 0x00;
unsigned char *pSoundValue = 0x00;          //pointer to sound value
unsigned char mSoundEnable = 0x00;          //is it playing or not
SoundData *mSoundCurrent = 0x00;


void Sound_init(void)
{
    //initialize
    TIM2_stop();
    DAC_set8Bit(0x00);
    
    mSoundCounter = 0x00;      //down counter for sound array
    mSoundLoopCounter = 0x00;
    pSoundValue = 0x00;        //pointer to sound value
    mSoundEnable = 0x00;        //is it playing or not
    mSoundCurrent = 0x00;       //current sound file playing
    
    
}


/////////////////////////////////////////////
//Sound_play
//set the sound pointer, counter, and enable
//and turn on the timer - timer 2.  loop is the 
//number of times it loops
void Sound_play(const SoundData *sound, unsigned int loop)
{
    mSoundCounter = sound->length;
    pSoundValue = sound->pSoundData;
    mSoundEnable = 1;
    mSoundLoopCounter = loop;
    mSoundCurrent = sound;
    
    TIM2_start();
    
}


//////////////////////////////////////////////
void Sound_interruptHandler(void)
{    
    unsigned int value = 0x00;
    
    if (mSoundEnable == 1)
    {
        if ((mSoundCounter > 0) && (mSoundLoopCounter > 0))
        {
            //DAC_set8Bit(*pSoundValue);
            value = ((unsigned int)(*pSoundValue)) << 2;
            DAC_set(value);
            pSoundValue++;
            mSoundCounter--;
        }
        
        else if ((!mSoundCounter) && (mSoundLoopCounter > 0))
        {
            //array complete - reset and continue
            mSoundLoopCounter--;                        //decrement the counter
            mSoundEnable = 1;                           //continue sound
            mSoundCounter = mSoundCurrent->length;      //reset the counter            
            pSoundValue = mSoundCurrent->pSoundData;    //point to null

            //DAC_set8Bit(*pSoundValue);
            value = ((unsigned int)(*pSoundValue)) << 2;
            DAC_set(value);
              
            pSoundValue++;
            mSoundCounter--;
        }
        
        else
        {
            mSoundEnable = 0;       //disable sound
            mSoundCounter = 0;      //reset the counter
            mSoundLoopCounter = 0;
            pSoundValue = 0x00;     //point to null
            DAC_set(0x00);
            TIM2_stop();            //stop the 11khz timer
        }
    }
}



