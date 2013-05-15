#ifndef _SDLFUNCS_C
#define _SDLFUNCS_C

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include <stdio.h>

#include "SDLfuncs.h"
#include "protocol.h" 
#include "clientinternetfuncs.h"
#include "audio.h"

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
                //playSound(soundShoot);
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

SDL_Surface *load_image( char filename[] )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;
    
    //The optimized image that will be used
    SDL_Surface* optimizedImage = NULL;
    
    //Load the image using SDL_image
    loadedImage = IMG_Load( filename );
    
    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized image
        optimizedImage = SDL_DisplayFormat( loadedImage );
        
        //Free the old image
        SDL_FreeSurface( loadedImage );
        
        //If the image was optimized just fine
        if( optimizedImage != NULL )
        {
            //Map the color key
            Uint32 colorkey = SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF );
            
            //Set all pixels of color R 0, G 0xFF, B 0xFF to be transparent
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, colorkey );
        }
    }
    
    //Return the optimized image
    return optimizedImage;
}

void draw_map( int x, int y, SDL_Surface* source, SDL_Surface* destination )
{
    //Make a temporary rectangle to hold the offsets
    SDL_Rect offset;
    
    //Give the offsets to the rectangle
    offset.x = -1*(x-SCREEN_WIDTH/2+HITBOX_WIDTH/2);
    offset.y = -1*(y-SCREEN_HIGHT/2+HITBOX_HIGHT/2);
    
    //Blit the surface
    SDL_BlitSurface( source, NULL, destination, &offset );
}

void draw_bullet(struct playerInfo * player, struct cameraInfo * camera, SDL_Surface* source, SDL_Surface* destination)
{

    //Make a temporary rectangle to hold the offsets
    SDL_Rect offset;
    
    //Give the offsets to the rectangle
    offset.x = player->bulletX + camera->xCord+ 400-HITBOX_WIDTH/2;
    offset.y = player->bulletY + camera->yCord+300-HITBOX_HIGHT/2;
    
    //Blit the surface
    SDL_BlitSurface( source, NULL, destination, &offset );
}

void draw_UI( int x, int y, SDL_Surface* text, SDL_Surface* screen)
{
    SDL_Rect offset;
    
    //Get offsets
    offset.x = x;
    offset.y = y;
    
    //Blit
    SDL_BlitSurface( text, NULL, screen, &offset );

}



#endif
