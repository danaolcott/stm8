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
unsigned char mSoundOverwriteFlag = 0x00;
SoundData *mSoundCurrent = 0x00;
static uint8_t mSoundOn = 0;



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
    mSoundOverwriteFlag = 0x00; //prevent overwrite when = 1
    
}


/////////////////////////////////////////////
//Sound_play
//set the sound pointer, counter, and enable
//and turn on the timer - timer 2.  loop is the 
//number of times it loops
//preventOverwrite - set to one if you want to make sure
//the sound plays through and not over write by another sound
//
void Sound_play(const SoundData *sound, unsigned int loop, unsigned char preventOverwrite)
{
    if ((preventOverwrite == 1) && (!mSoundOverwriteFlag))
    {
        mSoundOverwriteFlag = 1;            //clears when sound completed
        mSoundCounter = (uint16_t)(sound->length);
        pSoundValue = (uint8_t*)(sound->pSoundData);
        mSoundEnable = 1;
        mSoundLoopCounter = loop;
        mSoundCurrent = (SoundData*)sound;
        
        TIM2_start();
    }
    
    //
    else if (!mSoundOverwriteFlag)
    {
        mSoundCounter = (uint16_t)sound->length;
        pSoundValue = (uint8_t*)sound->pSoundData;
        mSoundEnable = 1;
        mSoundLoopCounter = loop;
        mSoundCurrent = (SoundData*)sound;
        
        TIM2_start();
    }
}


///////////////////////////////////////////////
//Sound_isPlaying - returns the status
//of the sound, is it playing or not...
unsigned char Sound_isPlaying(void)
{
    return mSoundEnable;
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
            mSoundOverwriteFlag = 0;    //clear any overwrites
            mSoundEnable = 0;       //disable sound
            mSoundCounter = 0;      //reset the counter
            mSoundLoopCounter = 0;
            pSoundValue = 0x00;     //point to null
            DAC_set(0x00);
            TIM2_stop();            //stop the 11khz timer
        }
    }
}



void Sound_playPlayerShoot(void)
{
    if (mSoundOn == 1)
        Sound_play(&sound_playerShoot, 1, 0);
}


void Sound_playEnemyExplode(void)
{
    if (mSoundOn == 1)
        Sound_play(&sound_enemyExplode, 1, 0);
}





void Sound_playLevelUp(void)
{
    if (mSoundOn == 1)
    {
        Sound_play(&soundSquare_4ms_500hz, 30, 0);
        while(Sound_isPlaying() == 1);
        Sound_play(&soundSquare_4ms_1000hz, 30, 0);
        while(Sound_isPlaying() == 1);
        Sound_play(&soundSquare_4ms_500hz, 30, 0);
        while(Sound_isPlaying() == 1);
        Sound_play(&soundSquare_4ms_1000hz, 30, 0);
        while(Sound_isPlaying() == 1);
        
    }
}



void Sound_playGameOver(void)
{
    if (mSoundOn == 1)
    {
        Sound_play(&soundSquare_4ms_1000hz, 30, 0);
        while(Sound_isPlaying() == 1);
        Sound_play(&soundSquare_4ms_500hz, 30, 0);
        while(Sound_isPlaying() == 1);
        Sound_play(&soundSquare_4ms_250hz, 30, 0);
        while(Sound_isPlaying() == 1);
        
        Sound_play(&soundSquare_4ms_1000hz, 30, 0);
        while(Sound_isPlaying() == 1);
        Sound_play(&soundSquare_4ms_500hz, 30, 0);
        while(Sound_isPlaying() == 1);
        Sound_play(&soundSquare_4ms_250hz, 30, 0);
        while(Sound_isPlaying() == 1);
        
    }
}





void Sound_on(void)
{
    mSoundOn = 1;
}

void Sound_off(void)
{
    mSoundOn = 0;
}

void Sound_toggle(void)
{
    if (mSoundOn == 1)
        mSoundOn = 0;
    else
        mSoundOn = 1;
}



