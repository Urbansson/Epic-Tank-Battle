#ifndef _H_AUDIO
#define _H_AUDIO
#include "SDL/SDL_mixer.h"

void cleanup();

Mix_Chunk *loadSound(char *name);

void playSound(Mix_Chunk *sfx);

void load_soundfiles();

Mix_Chunk *soundExplosion;

Mix_Chunk *soundShoot;

Mix_Chunk *soundMotor;

Mix_Chunk *soundHit;

Mix_Music *music;

#endif
