#ifndef _SDLFUNCS_C
#define _SDLFUNCS_C

#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include <stdio.h>
#include "SDLfuncs.h"

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
    
    //Test for errors
    GLenum error = glGetError();
    if( error != GL_NO_ERROR )
    {
        return FALSE;
    }
    
    return TRUE;
    
}

void draw(struct playerInfo * player, struct cameraInfo * camera)
{
    
    glTranslatef( player->xCord+ camera->xCord, player->yCord+ camera->yCord, 0);
    
    glBegin(GL_QUADS);
    
    glColor3f(1,0,0);

    
    glVertex2f( 0,      0      );
    glVertex2f( HITBOX, 0      );
    glVertex2f( HITBOX, HITBOX );
    glVertex2f( 0,      HITBOX );
    
    glEnd();
    
    //Resets
    glLoadIdentity();
}

void map(struct cameraInfo * camera)
{

    glTranslatef( camera->xCord, camera->yCord, 0);
    
    glBegin(GL_QUADS);

    glColor3f(0,1,0);
    
    glVertex2f( 0,         0      );
    glVertex2f( MAP_WIDTH, 0      );
    glVertex2f( MAP_WIDTH, MAP_HIGHT );
    glVertex2f( 0,         MAP_HIGHT );
    
    glEnd();

    //resets
    glLoadIdentity();

}

void handel_input(struct playerInfo * player, SDL_Event * event, int tcpSd )
{
    
    SDL_GetMouseState(&player->mouseX, &player->mouseY);
    
    //If key is pressed down
    if( event->type == SDL_KEYDOWN )
    {
        switch( event->key.keysym.sym )
        {
            case SDLK_w:
                send(tcpSd, "W", sizeof("W"), 0);
                //printf("W \n");
                break;
            case SDLK_s:
                send(tcpSd, "S", sizeof("S"), 0);
                //printf("S \n");
                break;
            case SDLK_a:
                send(tcpSd, "A", sizeof("A"), 0);
                //printf("A \n");
                break;
            case SDLK_d:
                send(tcpSd, "D", sizeof("D"), 0);
                //printf("D \n");
                break;
        }
    }
    //If a key was released
    else if( event->type == SDL_KEYUP )
    {
        //Adjust the velocity
        switch( event->key.keysym.sym )
        {
            case SDLK_w:
                send(tcpSd, "w", sizeof("W"), 0);
                //printf("W \n");
                break;
            case SDLK_s:
                send(tcpSd, "s", sizeof("S"), 0);
                //printf("S \n");
                break;
            case SDLK_a:
                send(tcpSd, "a", sizeof("A"), 0);
                //printf("A \n");
                break;
            case SDLK_d:
                send(tcpSd, "d", sizeof("D"), 0);
                //printf("D \n");
                break;
        }
    }
}




#endif
