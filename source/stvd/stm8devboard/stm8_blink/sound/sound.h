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
extern const SoundData soundTest1;
extern const SoundData soundTest2;

extern const SoundData wavSoundPlayerFire;
extern const SoundData wavSoundEnemyFire;
extern const SoundData wavSoundEnemyExplode;


//prototypes
void Sound_init(void);
void Sound_play(const SoundData *sound, unsigned int loop);
void Sound_interruptHandler(void);

#endif

