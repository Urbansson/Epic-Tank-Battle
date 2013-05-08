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
    //Screen
    SDL_Surface* screen = NULL;
    //Tanks
    SDL_Surface* blueTank = NULL;
    SDL_Surface* blueCannon = NULL;
    SDL_Surface* redTank = NULL;
    SDL_Surface* redCannon = NULL;
    
    //Bullet
    SDL_Surface* bullet = NULL;
    //WorldMap
    SDL_Surface* worldMap = NULL;
    
    //rotation Images
    SDL_Surface* rotatedTank[6] = {NULL,NULL,NULL,NULL,NULL,NULL};
    SDL_Surface* rotatedCannon[6] = {NULL,NULL,NULL,NULL,NULL,NULL};
    SDL_Surface* rotatedBullet[6] = {NULL,NULL,NULL,NULL,NULL,NULL};
    
    
    //Thread vars
    pthread_t reciveUdpData;

    //Game vars
    int run;
    struct playerInfo player[6];
    struct timerInfo fps;
    struct cameraInfo camera;
    int bulletAngle[6];
    
    int oldCannonAngle[6];
    int oldTankAngle[6] = {1000,1000,1000,1000,1000,1000};


    

    //inits Sdl and opens the screen
    screen = init_sdl();
    
    //Makes the connection to the server
    if(!(init_udp_tcp(&udpSd, &tcpSd, argv[1], argv[2])))
    {
        printf("Error making connection\n");
        return 0;
    }
    
    //load the images (Function maybe)
    blueTank = load_image( "./images/blueTank.bmp" );
    blueCannon = load_image( "./images/blueCannon.bmp" );
    redTank = load_image( "./images/redTank.bmp" );
    redCannon = load_image( "./images/redCannon.bmp" );
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
        draw_map(player[myId].xCord,player[myId].yCord, worldMap, screen);

        /*
        //Draws Tank body centered
         SDL_Rect tempoffset = {400-32,300-23,0,0};
        
         SDL_BlitSurface(blueTank, NULL, screen, &tempoffset);
        //apply_surface(400-(32),300-(23), blueTank, screen);
          */
        
        if (player[myId].tankAngle != oldTankAngle[myId])
        {
            SDL_FreeSurface( rotatedTank[myId] );

            rotatedTank[myId] = rotozoomSurface(blueTank,player[myId].tankAngle,1.0,0);
            oldTankAngle[myId] = player[myId].cannonAngle;
        }
        
        SDL_Rect SelfTankoffset = {400,300,0,0};
        
        //printf("FUNKAR HÄR\n");

        SelfTankoffset.x -= (rotatedTank[myId]->w/2);
        SelfTankoffset.y -= (rotatedTank[myId]->h/2);
        
        //printf("FUNKAR OCH HÄR\n");

        SDL_BlitSurface(rotatedTank[myId], NULL, screen, &SelfTankoffset);
        

        
        
        //ROTATES YOUR CANNON++++++++++++++++++++++++++++++        
        if (player[myId].cannonAngle != oldCannonAngle[myId])
        {            
            SDL_FreeSurface( rotatedCannon[myId] );
            rotatedCannon[myId] = rotozoomSurface(blueCannon,player[myId].cannonAngle,1.0,0);
            oldCannonAngle[myId] = player[myId].cannonAngle;
        }
        
        SDL_Rect SelfCannonoffset = {400,300,0,0};
        
        SelfCannonoffset.x -= (rotatedCannon[myId]->w/2);
        SelfCannonoffset.y -= (rotatedCannon[myId]->h/2);
        
        SDL_BlitSurface(rotatedCannon[myId], NULL, screen, &SelfCannonoffset);
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        
        
        //DISPLAYS YOUR BULLET++++++++++++++++++++++++++++++
        if (player[myId].fire > 0)
        {
            if (bulletAngle[myId] == 0)
            {
                SDL_FreeSurface( rotatedBullet[myId] );
                rotatedBullet[myId] = rotozoomSurface(bullet,player[myId].cannonAngle,1.0,0);
            }
            
            draw_bullet(&player[myId], &camera, rotatedBullet[myId], screen );
            bulletAngle[myId] = 1;
        }
        
        if (player[myId].fire == 0)
        {
            bulletAngle[myId]=0;
        }
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        
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
        
        sscanf(buffer, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", &moveInfo.x, &moveInfo.y , &moveInfo.player, &moveInfo.mouseX, &moveInfo.mouseY,&moveInfo.fire, &moveInfo.bulletX, &moveInfo.bulletY, &moveInfo.tankAngle, &moveInfo.cannonAngle);
        
        
        //Saves the incoming data in the players struct.
        player[moveInfo.player].slot = moveInfo.player;
        
        player[moveInfo.player].xCord = moveInfo.x;
        player[moveInfo.player].yCord = moveInfo.y;
        
        player[moveInfo.player].mouseX = moveInfo.mouseX;
        player[moveInfo.player].mouseY = moveInfo.mouseY;
        
        player[moveInfo.player].bulletX = moveInfo.bulletX;
        player[moveInfo.player].bulletY = moveInfo.bulletY;
        player[moveInfo.player].fire = moveInfo.fire;
        
        player[moveInfo.player].tankAngle = -1*moveInfo.tankAngle;
        player[moveInfo.player].cannonAngle = -1*moveInfo.cannonAngle;
        
        sleep(0);
    }
    
}

