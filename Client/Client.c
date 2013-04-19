#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <sys/types.h>

#include "SDLfuncs.h"
#include "timers.h"
#include "clientinternetfuncs.h"
#include "protocol.h" 

#define FPS 60


//void * calculate_movement(void * parameters);

int main(int argc, char *argv[])
{
    pthread_t movCalc;
    int run = 1;
    struct playerInfo player[6];
    struct timerInfo fps;
    int udpSd, tcpSd;

    SDL_Event event;

    char buffer[32];
    char temp[5];
    int pos, devider = 0;
    
    struct stcMoveInfo moveInfo;
    
    udpSd = udp_init();
    if (udpSd == -1)
    {
        printf("udp init failed!\n");
        return;
    }
    
    
    tcpSd = tcp_init(argv[1], argv[2]);
    if (tcpSd == -1)
    {
        printf("tcp init failed!\n");
        return;
    }

    
    init();

    //player[].xCord = 0;
    //player[].yCord = 0;
    //player[].yVel = 0;
    //player.xVel = 0;
    
    //pthread_create( &movCalc, NULL, calculate_movement, &(player));

    while (run)
    {
        timer_start(&fps);
        
         while( SDL_PollEvent( &event ) )
         {
             
             if( event.type == SDL_QUIT )
             {
                 run = FALSE;
             }
             
             handel_input(&player[0], &event, tcpSd );
            
         }

        
        recvfrom(udpSd, buffer, sizeof(buffer), 0, argv[1], sizeof(argv[1]));
        
        for (pos = 0; pos < 9; pos++)
        {
            if (buffer[pos] == ',')
            {
                if (devider == 0)
                {
                    strncpy(temp, buffer, pos);
                    temp[pos] = '\0';
                    moveInfo.x = atoi(temp);
                    devider = pos;
                }
                else
                {
                    strncpy(temp, &buffer[devider+1], pos);
                    temp[pos] = '\0';
                    moveInfo.y = atoi(temp);
                    
                    strcpy(temp, &buffer[pos+1]);
                    temp[1] = '\0';
                    moveInfo.player = atoi(temp);
                    devider = 0;
                    break;
                }
            }
        }
        
        
        player[moveInfo.player].xCord = moveInfo.x;
        player[moveInfo.player].yCord = moveInfo.y;
        
        printf("recived from server: %s\n", buffer);
        
        //Clears the screen
        glClear( GL_COLOR_BUFFER_BIT );
        
        
        //draws box on screen
        draw(&player[0]);
        draw(&player[1]);

        
        
        //Waits until everything is drawn on the screen
        glFlush();

    
        //Draws on screen
        SDL_GL_SwapBuffers();
        
        
        //Cap the frame rate
        if( timer_get_ticks(&fps) < 1000 / FPS )
        {
            SDL_Delay( ( 1000 / FPS ) - timer_get_ticks(&fps) );
        }

            
    }

    pthread_cancel(movCalc);
    return 0;
}


//This will be done on the server

/*
void * calculate_movement(void * parameters)
{
    struct playerInfo * player = (struct playerInfo*) parameters;
    
    struct timerInfo calcs;
    
    
    for (;;)
    {
        calcs.ticks = SDL_GetTicks();
 
        player->xCord += player->xVel;
        
        if (player->xCord < 0 || player->xCord + HITBOX > SCREEN_WIDTH )
        {
            player->xCord -= player->xVel;

        }
        
        player->yCord += player->yVel;
        
        if (player->yCord < 0 || player->yCord + HITBOX > SCREEN_HIGHT )
        {
            player->yCord -= player->yVel;
        }
        
        //Allows 200 calculations every sec
        if( timer_get_ticks(&calcs) < 1000 / 200 )
        {
            SDL_Delay( ( 1000 / 200 ) - timer_get_ticks(&calcs) );
        }
    }

}
*/
