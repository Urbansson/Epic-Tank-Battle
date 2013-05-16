//sdl Includes
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"


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
#include "audio.h"

#define MAX_PLAYERS 6
#define FPS 60

struct udpData udpInfo;

void * recive_udp_data(void * parameters);

int redPoints;
int bluePoints;

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
    //SDL_Surface* rotatedTank[6] = {NULL,NULL,NULL,NULL,NULL,NULL};
    //SDL_Surface* rotatedCannon[6] = {NULL,NULL,NULL,NULL,NULL,NULL};
    SDL_Surface* rotatedBullet[6] = {NULL,NULL,NULL,NULL,NULL,NULL};
    
    //SDL_Rect Tankoffset[6] = {400,300,0,0};
    //SDL_Rect Cannonoffset[6] = {400,300,0,0};

    //UserInterface vars
    SDL_Surface* UIhealth;
    SDL_Surface* UIreload;
    SDL_Surface* UIredPoints;
    SDL_Surface* UIbluePoints;
    
    TTF_Font *font = NULL;
    TTF_Font *reloadFont = NULL;
    
    SDL_Color textColor = { 255, 255, 255 };
    char textBuffer[32];
    char reload[32] = "FIRE: ";
    
    
    //Thread vars
    pthread_t reciveUdpData;

    //Game vars
    int run;
    struct playerInfo player[6];
    struct timerInfo fps;
    struct cameraInfo camera;
    int bulletAngle[6];
    
    //int oldCannonAngle[6];
    //int oldTankAngle[6];
    
    //Other vars
    int i;
    
    //Inits the player struct
    for (i = 0; i < MAX_PLAYERS; i++)
    {
        player[i].slot = -1;
        player[i].connected = 0;
    }
    
    //inits Sdl and opens the screen
    screen = init_sdl();
    if(screen == 0)
    {
        printf("Error initializing SDL\n");
        return 0;
    }
    
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

    //Load the fonts
    font = TTF_OpenFont( "./fonts/Army.ttf", 24 );
    reloadFont = TTF_OpenFont( "./fonts/Armyfat.ttf", 24 );
    
    //Moves udp info to global var
    udpInfo.udpSd = udpSd;
    strcpy(udpInfo.serverIp, argv[1]);

    //Recives the first information from the server
    recv(tcpSd, buffer, sizeof(buffer), 0);
    myId = atoi(buffer);
    
    //Starts the Recive data thread
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

        
        //DISPLAYES YOUR TANK+++++++++++++++++++++++++++++
        if (player[myId].team == 1)
        {
            draw_tank_self(player[myId].tankAngle, blueTank, screen);
            draw_cannon_self(player[myId].cannonAngle, blueCannon, screen);
        }
        else
        {
            draw_tank_self(player[myId].tankAngle, redTank, screen);
            draw_cannon_self(player[myId].cannonAngle, redCannon, screen);
        }
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        //DISPLAYES OTHER TANKS+++++++++++++++++++++++++++++
        for (i = 0; i < MAX_PLAYERS; i++)
        {
            if (player[i].slot == myId)
            {
                continue;
            }
            
            if (player[i].slot > -1 && player[i].connected == 1)
            {
                if (player[i].team == 1)
                {
                    draw_tank_other(player[i].xCord,player[i].yCord,camera.xCord,camera.yCord,player[i].tankAngle,blueTank,screen);
                    if (player[i].dead == 0)
                        draw_cannon_other(player[i].xCord,player[i].yCord,camera.xCord,camera.yCord,player[i].cannonAngle,blueCannon,screen);
                }
                else
                {
                    draw_tank_other(player[i].xCord,player[i].yCord,camera.xCord,camera.yCord,player[i].tankAngle,redTank,screen);
                    if (player[i].dead == 0)
                        draw_cannon_other(player[i].xCord,player[i].yCord,camera.xCord,camera.yCord,player[i].cannonAngle,redCannon,screen);
                }
            }
        }
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        
        //DRAWS ALL THE BULLETS ON THE SCREEEN+++++++++++++++++++++++++++
        for (i = 0; i < MAX_PLAYERS; i++)
        {
            if (player[i].slot > -1 && player[i].connected == 1)
            {
                if (player[i].fire > 0)
                {
                    if (bulletAngle[i] == 0)
                    {
                        SDL_FreeSurface( rotatedBullet[i] );
                        rotatedBullet[i] = rotozoomSurface(bullet,player[i].cannonAngle,1.0,0);
                        playSound(soundShoot);
                    }
                    draw_bullet(&player[i], &camera, rotatedBullet[i], screen );
                    bulletAngle[i] = 1;
                }
                
                if (player[i].fire == 0)
                {
                    bulletAngle[i]=0;
                }
            }
        }
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        
        //DRAWS THE USER INTERFACE ON SCREEN+++++++++++++++++++++++++++++
        
        textColor.r=0;
        textColor.g=0;
        textColor.b=255;
        sprintf(textBuffer, "BLUE POINTS: %d", bluePoints);
        UIbluePoints = TTF_RenderText_Solid( font, textBuffer, textColor );
        draw_UI( 10, 10, UIbluePoints, screen);
        
        textColor.r=255;
        textColor.g=0;
        textColor.b=0;
        sprintf(textBuffer, "RED POINTS: %d", redPoints);
        UIredPoints = TTF_RenderText_Solid( font, textBuffer, textColor );
        draw_UI( 600, 10, UIredPoints, screen);
        
        textColor.r=255;
        textColor.g=0;
        textColor.b=0;
        sprintf(textBuffer, "HP: %d", player[myId].healthPoints);
        UIhealth = TTF_RenderText_Solid( font, textBuffer, textColor );
        draw_UI( 20, 570, UIhealth, screen);
        
        
        textColor.r=255;
        textColor.g=0;
        textColor.b=0;
        if (player[myId].fire == 0)
        {
            strcpy(reload, "FIRE: READY");
            UIreload = TTF_RenderText_Solid( font, reload, textColor );
        }
        else
        {
            strcpy(reload, "FIRE: RELOADING");
            UIreload = TTF_RenderText_Solid( font, reload, textColor );
        
        }
        draw_UI( 580, 570, UIreload, screen);


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
    
    struct timerInfo timeOut[MAX_PLAYERS];
    int i;
    char buffer[128];
    struct stcInfo moveInfo;
    
    struct playerInfo * player = (struct playerInfo*) parameters;
    
    
    
    //Init the timer
    for (i = 0; i < MAX_PLAYERS; i++)
    {
        timer_init(&timeOut[i]);
    }
    
    while (1)
    {
        
        recvfrom(udpInfo.udpSd, buffer, sizeof(buffer), 0, udpInfo.serverIp, sizeof(udpInfo.serverIp));
        
        //printf("recived from server: %s \n", buffer);
        
        sscanf(buffer, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", &moveInfo.team,&moveInfo.x, &moveInfo.y , &moveInfo.player, &moveInfo.mouseX, &moveInfo.mouseY,&moveInfo.fire, &moveInfo.bulletX, &moveInfo.bulletY, &moveInfo.tankAngle, &moveInfo.cannonAngle, &moveInfo.dead, &moveInfo.healthPoints, &moveInfo.redPoints, &moveInfo.bluePoints);
        
        timer_start(&timeOut[moveInfo.player]);
        
        //Saves the incoming data in the players struct.
        player[moveInfo.player].slot = moveInfo.player;
        
        player[moveInfo.player].team = moveInfo.team;

        player[moveInfo.player].xCord = moveInfo.x;
        player[moveInfo.player].yCord = moveInfo.y;
        
        player[moveInfo.player].mouseX = moveInfo.mouseX;
        player[moveInfo.player].mouseY = moveInfo.mouseY;
        
        player[moveInfo.player].bulletX = moveInfo.bulletX;
        player[moveInfo.player].bulletY = moveInfo.bulletY;
        player[moveInfo.player].fire = moveInfo.fire;
        
        player[moveInfo.player].tankAngle = -1*moveInfo.tankAngle;
        player[moveInfo.player].cannonAngle = -1*moveInfo.cannonAngle;
        
        player[moveInfo.player].healthPoints = moveInfo.healthPoints;
        player[moveInfo.player].dead = moveInfo.dead;
        
        player[moveInfo.player].connected = 1;
        
        redPoints = moveInfo.redPoints;
        bluePoints = moveInfo.bluePoints;

        //If no new udp from a client it disconnects after 0.5 sec
        for (i = 0; i < MAX_PLAYERS; i++)
        {
            if (timer_get_ticks(&timeOut[i]) > 500)
            {
                player[i].connected = 0;
                timer_stop(&timeOut[i]);
            }  
        }
        
        sleep(0);
    }
    
}

