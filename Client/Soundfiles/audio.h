#ifndef _H_AUDIO
#define _H_AUDIO
/*SDL audio header*/
/*#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"*/
#include "SDL/SDL_mixer.h"

void init(char *title);
void cleanup();
Mix_Chunk *loadSound(char *name);
void playSound(Mix_Chunk *sfx);
Mix_Chunk *soundExplosion;
Mix_Chunk *soundShoot;
Mix_Music *music;

#endif
