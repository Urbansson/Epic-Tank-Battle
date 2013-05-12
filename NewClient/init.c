#ifndef _INIT_C
#define _INIT_C

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"

#include <stdio.h>

#include "init.h"
#include "audio.h"
#include "clientinternetfuncs.h"


SDL_Surface *init_sdl()
{
    SDL_Surface* screen = NULL;

    // Init SDL
    if (SDL_Init( SDL_INIT_EVERYTHING ) < 0)
    {
        return FALSE;
    }
    
    // Creates Window    width, hight, bpp
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HIGHT, SCREEN_BPP, SDL_DOUBLEBUF|SDL_HWSURFACE);
    if ( screen == NULL)
    {
        return FALSE;
    }
    
    if (Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 4096) != 0)
    {
        printf("Could not open audio: %s\n", Mix_GetError());
        
        exit(1);
    }
    
    //Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
        return FALSE;
    }
    
    load_soundfiles();    
    SDL_WM_SetCaption("Client", NULL);
    
    
    return screen;
}

int init_udp_tcp(int * udpSd, int * tcpSd, char *serverIp, char *serverPort)
{
    
    *udpSd = udp_init();
    if (udpSd == -1)
    {
        printf("udp init failed!\n");
        return FALSE;
    }
    
    *tcpSd = tcp_init(serverIp, serverPort);
    if (tcpSd == -1)
    {
        printf("tcp init failed!\n");
        return FALSE;
    }
    
    return TRUE;
}

#endif
