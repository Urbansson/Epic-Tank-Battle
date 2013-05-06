


/*soundtest main*/
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <SDL/SDL.h>
#include "audio.h"

Mix_Chunk *soundExplosion;
Mix_Chunk *soundShoot;
Mix_Chunk *soundHit;
Mix_Chunk *soundMotor;


void getInput()
{
  SDL_Event event;
	
  /* Loop through waiting messages and process them */
	
  while (SDL_PollEvent(&event))
    {
      switch (event.type)
	{
	  /*
	  ** Closing the Window or pressing Escape will exit the program
	  ** Pressing Space will play the sound effect
	  */
			
	case SDL_QUIT:
	  exit(0);
	  break;
			
	case SDL_KEYDOWN:
	  switch (event.key.keysym.sym)
	    {
	    case SDLK_s:
	      //Mix_PlayChannel(-1, soundExplosion,0);
	      playSound(soundExplosion);
	      break;
	    case SDLK_a:
	      playSound( soundShoot );	
	      break;
 	    case SDLK_d:
	      playSound( soundHit );	
	      break;
	    case SDLK_f:
				playSound( soundMotor );	
	      break;
	    case SDLK_9: 
          
	      //If there is no music playing
	      if( Mix_PlayingMusic() == 0 )
		{
		  //Play the music
		  if( Mix_PlayMusic( music, -1 ) == -1 )
		    {
		      exit(0);
		    }    
		}
	      else
		{
		  //If the music is paused
		  if( Mix_PausedMusic() == 1 )
		    {
		      //Resume the music
		      Mix_ResumeMusic();
		    }
		  //If the music is playing
		  else
		    {
		      //Pause the music
		      Mix_PauseMusic();
		    }
		}
                
	      break;

	    case SDLK_ESCAPE:
	      exit(0);
	      break;
					
	    default:
	      break;
	    }
	  break;
	}
    }
}

int main(void)
{
  int loop;
  //Initialize SDL
  init("Soundtest");
	
  atexit(cleanup);
	
  loop = 1;
	
  load_files();
	
  while(1)
    {
      getInput();
    }
	
  return 0;
}
