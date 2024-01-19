/*
4/20/20
Sound controller file.  Works with DAC output
on PB4 and Timer to
*/


#ifndef __SOUND_H
#define __SOUND_H


typedef struct 
{
    unsigned int length;
	const unsigned char* pSoundData;
}SoundData;


//sound arrays
extern const SoundData soundSquare_4ms_250hz;
extern const SoundData soundSquare_4ms_500hz;
extern const SoundData soundSquare_4ms_1000hz;
extern const SoundData soundSquare_4ms_1800hz;
extern const SoundData soundSquare_4ms_2600hz;


//prototypes
void Sound_init(void);
void Sound_play(const SoundData *sound, unsigned int loop, unsigned char preventOverwight);
unsigned char Sound_isPlaying(void);
void Sound_interruptHandler(void);


void Sound_on(void);
void Sound_off(void);
void Sound_toggle(void);

#endif

