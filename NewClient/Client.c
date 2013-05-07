//sdl Includes
#include "SDL/SDL.h"

//Standard includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Thread includes
#include <pthread.h>
#include <sys/types.h>

//Custom includes
#include "init.h"
#include "protocol.h"
#include "SDLfuncs.h"
#include "clientinternetfuncs.h"
#include "timers.h"
#include "SDL_rotozoom.h"
#include "calculations.h"



#define FPS 60

struct udpData udpInfo;

void * recive_udp_data(void * parameters);

int main(int argc, char *argv[])
{
    
    //Network vars
    int udpSd, tcpSd;
    char buffer[5];
    int myId;
    
    //SDL vars
    SDL_Event event;
    SDL_Surface* screen = NULL;
    SDL_Surface* blueTank = NULL;
    SDL_Surface* blueCannon = NULL;
    SDL_Surface* worldMap = NULL;
    SDL_Surface* rotatedImage = NULL;
    SDL_Surface* bullet = NULL;
    SDL_Surface* rotatedbullet = NULL;





    
    //Thread vars
    pthread_t reciveUdpData;

    //Game vars
    int run;
    struct playerInfo player[6];
    struct timerInfo fps;
    int oldAngle[6];
    struct cameraInfo camera;
    int bulletAngle[6];
    

    //inits Sdl and opens the screen
    screen = init_sdl();
    
    //Makes the connection to the server
    if(!(init_udp_tcp(&udpSd, &tcpSd, argv[1], argv[2])))
    {
        printf("Error making connection\n");
        return 0;
    }
    
    //load all the images
    blueTank = load_image( "./images/blueTank.bmp" );
    blueCannon = load_image( "./images/blueCannon.bmp" );

    worldMap = load_image( "./images/worldMap.bmp" );
    
    bullet = load_image( "./images/bullet.bmp" );

    
    
    //Moves udp info to global var
    udpInfo.udpSd = udpSd;
    strcpy(udpInfo.serverIp, argv[1]);

    
    //Recives the first information from the server
    recv(tcpSd, buffer, sizeof(buffer), 0);
    myId = atoi(buffer);
    
    pthread_create( &reciveUdpData, NULL, recive_udp_data, &(player));

    while (run)
    {
        //Start the timer
        timer_start(&fps);

        while( SDL_PollEvent( &event ) )
        {
            
            if( event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE)
            {
                run = FALSE;
            }
            
            handel_input(&event, tcpSd );
            
        }
        
        camera.xCord = -player[myId].xCord;
        camera.yCord = -player[myId].yCord;
        
        
        
        //From here we draw stuff on the screen
        SDL_FillRect(screen,NULL, 0x000000);

        
        //Draws WorldMAps
        apply_surface(-1*player[myId].xCord+400-32,-1*player[myId].yCord+300-22, worldMap, screen);
        
        
        //Draws Tank body centered
        apply_surface(400-(32),300-(23), blueTank, screen);
        
        
        
        
        //Calculates the angle
        player[myId].angle = calculate_angle(player[myId].mouseY, player[myId].mouseX);
        
        if (player[myId].angle != oldAngle[myId])
        {
            printf("Angle is: %d\n", player[myId].angle);
            
            SDL_FreeSurface( rotatedImage );
            //rotatedImage = rotate_image(blueCannon, player[myId].angle);
            rotatedImage = rotozoomSurface(blueCannon,player[myId].angle,1.0,0);
            oldAngle[myId] = player[myId].angle;
        }
        
        SDL_Rect offset = {400,300,0,0};
        
        offset.x -= (rotatedImage->w/2);
        offset.y -= rotatedImage->h/2;
        
        SDL_BlitSurface(rotatedImage, NULL, screen, &offset);

        
        if (player[myId].fire == 1)
        {
            if (bulletAngle[myId] == 0)
            {
                SDL_FreeSurface( rotatedbullet );
                rotatedbullet = rotozoomSurface(bullet,player[myId].angle,1.0,0);
            }
            
            draw_bullet(&player[myId], &camera, rotatedbullet, screen );
            bulletAngle[myId] = 1;
        }
        
        if (player[myId].fire == 0)
        {
            bulletAngle[myId]=0;
        }
    /// funktioner

        //Update Screen
        SDL_Flip( screen );
        
        
        
        
        
        //Cap the frame rate
        if( timer_get_ticks(&fps) < 1000 / FPS )
        {
            SDL_Delay( ( 1000 / FPS ) - timer_get_ticks(&fps) );
        }

    }
    
    
    pthread_cancel(reciveUdpData);
    return 0;
}

void * recive_udp_data(void * parameters)
{
    
    char buffer[64];
    struct stcInfo moveInfo;
    
    struct playerInfo * player = (struct playerInfo*) parameters;
    
    while (1)
    {
        recvfrom(udpInfo.udpSd, buffer, sizeof(buffer), 0, udpInfo.serverIp, sizeof(udpInfo.serverIp));
        
        printf("recived from server: %s \n", buffer);
        
        sscanf(buffer, "%d,%d,%d,%d,%d,%d,%d,%d", &moveInfo.x, &moveInfo.y , &moveInfo.player, &moveInfo.mouseX, &moveInfo.mouseY,&moveInfo.fire, &moveInfo.bulletX, &moveInfo.bulletY);
        
        
        //Saves the incoming data in the players struct.
        player[moveInfo.player].slot = moveInfo.player;
        
        player[moveInfo.player].xCord = moveInfo.x;
        player[moveInfo.player].yCord = moveInfo.y;
        
        player[moveInfo.player].mouseX = moveInfo.mouseX;
        player[moveInfo.player].mouseY = moveInfo.mouseY;
        
        player[moveInfo.player].bulletX = moveInfo.bulletX;
        player[moveInfo.player].bulletY = moveInfo.bulletY;
        player[moveInfo.player].fire = moveInfo.fire;
        
    
    }
    
}

