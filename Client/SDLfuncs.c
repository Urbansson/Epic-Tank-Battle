#ifndef _SDLFUNCS_C
#define _SDLFUNCS_C

#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
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

void handel_input(SDL_Event * event, int tcpSd )
{
    struct ctsCommands commands;
    char buffer[20];
    
    commands.mouseX = 0;
    commands.mouseX = 0;
    commands.keyboardInput = '0';
    commands.mouseInput = '0';
    
    SDL_GetMouseState(&commands.mouseX, &commands.mouseY);
    
    
    //player->mouseX = commands.mouseX;
    //player->mouseY = commands.mouseY;
    //SDL_GetMouseState(&player->mouseX, &player->mouseY);
    
    if( event->type == SDL_MOUSEBUTTONDOWN )
    {
        switch(event->button.button)
        {
            case SDL_BUTTON_LEFT:
                commands.mouseInput = 'L';
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
                //send(tcpSd, "W", sizeof("W"), 0);
                //printf("W \n");
                break;
            case SDLK_s:
                commands.keyboardInput = 'S';
                //send(tcpSd, "S", sizeof("S"), 0);
                //printf("S \n");
                break;
            case SDLK_a:
                commands.keyboardInput = 'A';
                //send(tcpSd, "A", sizeof("A"), 0);
                //printf("A \n");
                break;
            case SDLK_d:
                commands.keyboardInput = 'D';
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
