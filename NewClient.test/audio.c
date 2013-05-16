#ifndef _C_AUDIO
#define _C_AUDIO

/*SDL audio header*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
/*#include "SDL/SDL_mixer.h"*/
#include "audio.h"

SDL_Surface *screen;

//I denna variabel laddar vi in explosionsljudet, fyll på med fler ljudeffekter

Mix_Music *music = NULL;

//The sound effects that will be used
Mix_Chunk *soundExplosion = NULL;
Mix_Chunk *soundShoot = NULL;
Mix_Chunk *soundMotor = NULL;
Mix_Chunk *soundHit = NULL;


//Lägg till i header
//Laddar in filer
void load_soundfiles()
{
	
	music = Mix_LoadMUS("./sounds/musicTitle.wav");
	if(music == NULL)
	{
		//Music
		printf("Error with music\n");
		exit(0);
	}
	
	//Sound effects
	soundExplosion = Mix_LoadWAV("./sounds/soundExplosion.wav");

	
	if((soundExplosion == NULL))
		{
			printf("Error with exp\n");
			exit(0);
		}
	soundShoot = Mix_LoadWAV("./sounds/soundShoot.wav");	
		if((soundShoot == NULL))
		{
			printf("Error with shoot");
			exit(0);
		}
	
		soundHit = Mix_LoadWAV("./sounds/soundHit.wav");	
		if((soundHit == NULL))
		{
			printf("Error with hit");
			exit(0);
		}
		
			soundMotor = Mix_LoadWAV("./sounds/soundMotor.wav");	
		if((soundMotor == NULL))
		{
			printf("Error with motor");
			exit(0);
		}
	
}

void cleanup(Mix_Chunk *sound)
{
	Mix_FreeChunk( soundExplosion );
	Mix_FreeChunk( soundShoot );
	Mix_FreeChunk( soundHit );
	Mix_FreeChunk( soundMotor );
	
	Mix_FreeMusic( music );
	
	Mix_CloseAudio();
	
	SDL_Quit();
}


/*Mix_Chunk *loadSound(char *name)
{
	/* Load the sound specified by the filename 
	
	Mix_Chunk *sfx = Mix_LoadWAV(name);

	if (sfx == NULL)
	{
		printf("Failed to load sound %s\n", name);
	}
	
	return sfx;
}*/

void playSound(Mix_Chunk *sfx)
{
	/* Play the sound on the first free channel and only play it once */
	
	Mix_PlayChannel(-1, sfx, 0);
}


#endif
