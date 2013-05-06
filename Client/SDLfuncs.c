#ifndef _SDLFUNCS_C
#define _SDLFUNCS_C

#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include "SDL/SDL_image.h"
#include "audio.h"

#include <stdio.h>

#include "SDLfuncs.h"
#include "protocol.h" 
#include "clientinternetfuncs.h"




int init()
{
    // Init SDL
    if (SDL_Init( SDL_INIT_EVERYTHING ) < 0)
    {
        return FALSE;
    }
    // Creates Window    width, hight, bpp
    if ( SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HIGHT, SCREEN_BPP, SDL_OPENGL) == NULL)
    {
        return FALSE;
    }
    
    // Init OpenGL
    if ( initGL() == FALSE)
    {
        return FALSE;
    }
    
   	if (Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 4096) != 0)
		{
			printf("Could not open audio: %s\n", Mix_GetError());
			
			exit(1);
		}
		
		load_soundfiles();
    SDL_WM_SetCaption("Client", NULL);
    
    return TRUE;
}

int initGL()
{
    //2d textures
    glEnable( GL_TEXTURE_2D );
    
    //background
    glClearColor( 0, 0, 0, 0 );
    
    //Viewport?
    //glViewport( 0, 0, 800, 600 );
    
    //Clear color buffer
    glClear( GL_COLOR_BUFFER_BIT );
    
    //
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    
    //Cordinate system
    glOrtho(0,800,600,0,0,1);
    
    //
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    
    //removes z-axis
    glDisable( GL_DEPTH_TEST );
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    //Test for errors
    GLenum error = glGetError();
    if( error != GL_NO_ERROR )
    {
        return FALSE;
    }
    
    return TRUE;
    
}

GLuint load_image(char *fileName)
{
    SDL_Surface *surface;
    GLenum texture_format;
    GLint  nOfColors;
    GLuint texture;
    
    if ( (surface = IMG_Load(fileName)) ) {
    
        
        // get the number of channels in the SDL surface
        nOfColors = surface->format->BytesPerPixel;
        if (nOfColors == 4)     // contains an alpha channel
        {
            if (surface->format->Rmask == 0x000000ff)
                texture_format = GL_RGBA;
            //else
            //texture_format = GL_BGRA;
        } else if (nOfColors == 3)     // no alpha channel
        {
            if (surface->format->Rmask == 0x000000ff)
                texture_format = GL_RGB;
            //else
            //texture_format = GL_BGR;
        } else {
            printf("warning: the image is not truecolor..  this will probably break\n");
            // this error should not go unhandled
        }
        
        // Have OpenGL generate a texture object handle for us
        glGenTextures( 1, &texture );
        
        // Bind the texture object
        glBindTexture( GL_TEXTURE_2D, texture );
        
        // Set the texture's stretching properties
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        
        // Edit the texture object's image data using the information SDL_Surface gives us
        glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
                     texture_format, GL_UNSIGNED_BYTE, surface->pixels );
    }
    else {
        printf("SDL could not load %s: %s\n", fileName,SDL_GetError());
        SDL_Quit();
        //return 1;
    }
    
    // Free the SDL_Surface only if it was successfully created
    if ( surface ) {
        SDL_FreeSurface( surface );
    }
    return texture;
}


void handel_input(SDL_Event * event, int tcpSd )
{
    struct ctsCommands commands;
    char buffer[20];
    
    commands.mouseX = 0;
    commands.mouseX = 0;
    commands.keyboardInput = '0';
    commands.mouseInput = '0';
    
    SDL_GetMouseState(&commands.mouseX, &commands.mouseY);
    
    if( event->type == SDL_MOUSEBUTTONDOWN )
    {
        switch(event->button.button)
        {
            case SDL_BUTTON_LEFT:
                commands.mouseInput = 'L';
								playSound(soundShoot);
                //printf("LMB \n");
                break;
            case SDL_BUTTON_RIGHT:
                commands.mouseInput = 'R';
                //printf("RMB \n");
                break;
            default:
                commands.mouseInput = 'N';
        }

        //format_send_ctsCommand(&commands, tcpSd);
    }
    
    //If key is pressed down
    else if( event->type == SDL_KEYDOWN )
    {
        switch( event->key.keysym.sym )
        {
            case SDLK_w:
                commands.keyboardInput = 'W';
                playSound(soundMotor);
                //send(tcpSd, "W", sizeof("W"), 0);
                //printf("W \n");
                break;
            case SDLK_s:
                commands.keyboardInput = 'S';
                playSound(soundMotor);
                //send(tcpSd, "S", sizeof("S"), 0);
                //printf("S \n");
                break;
            case SDLK_a:
                commands.keyboardInput = 'A';
                playSound(soundMotor);
                //send(tcpSd, "A", sizeof("A"), 0);
                //printf("A \n");
                break;
            case SDLK_d:
                commands.keyboardInput = 'D';
                playSound(soundMotor);
                //send(tcpSd, "D", sizeof("D"), 0);
                //printf("D \n");
                break;
        }
        
        //format_send_ctsCommand(&commands, tcpSd);

        
    }
    //If a key was released
    else if( event->type == SDL_KEYUP )
    {
        //Adjust the velocity
        switch( event->key.keysym.sym )
        {
            case SDLK_w:
                commands.keyboardInput = 'w';

                //send(tcpSd, "w", sizeof("W"), 0);
                //printf("W \n");
                break;
            case SDLK_s:
                commands.keyboardInput = 's';

                //send(tcpSd, "s", sizeof("S"), 0);
                //printf("S \n");
                break;
            case SDLK_a:
                commands.keyboardInput = 'a';

                //send(tcpSd, "a", sizeof("A"), 0);
                //printf("A \n");
                break;
            case SDLK_d:
                commands.keyboardInput = 'd';

                //send(tcpSd, "d", sizeof("D"), 0);
                //printf("D \n");
                break;
        }
        
        //format_send_ctsCommand(&commands, tcpSd);
    }
    
    format_send_ctsCommand(&commands, tcpSd);

    
}




#endif
