/*Mouse coordinates by Emil Vikström written 13/4-2013 inspired by SDL tutorial*/


#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <string.h>
#include <time.h>


  //The attributes of the screen
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SCREEN_BPP 32


//Initiating SDL
int init()
{
  if( SDL_Init( SDL_INIT_EVERYTHING ) < 0)
    {
      return 0;
    }

  if( SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE) == NULL)
    {
      return 0;
    }

  return 1;
}

//Checks the coordinates of the mouse and returns x and y value
void CheckMouseHover(int *x, int *y)
{
    int mouse_x, mouse_y;

    SDL_PumpEvents();

    SDL_GetMouseState(&mouse_x, &mouse_y);
    usleep(500000);
    *x = mouse_x; *y = mouse_y;
     
}

int main(int argc, char *argv[])
{
    
  int mousePosX , mousePosY, quit = 0;
  int  x = 400;
  int y = 300;
  char mouseCoordinates[8];
  SDL_Event event;
  init();

  printf("Initierar\n");
  SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);

  while(!quit)
    {
      CheckMouseHover(&mousePosX,&mousePosY);
      sprintf(mouseCoordinates,"%d,%d",mousePosX,mousePosY);
      printf("%s\n",mouseCoordinates);
      SDL_PollEvent( &event );
      if( event.type == SDL_QUIT )
	quit = 1;
    }

  return 0;
}
