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


struct udpData
{
    int udpSd;
    char serverIp[16];
} UdpInfo;

void * recive_udp_data(void * parameters);


//void * calculate_movement(void * parameters);

int main(int argc, char *argv[])
{
    pthread_t reciveUdpData;
    int run = 1;
    struct playerInfo player[6];
    struct timerInfo fps;
    int udpSd, tcpSd;
    struct cameraInfo camera;
    int myId;
    char buffer[5];

    SDL_Event event;
    
    strcpy(UdpInfo.serverIp, argv[1]);
    UdpInfo.udpSd = udp_init();
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
    
    recv(tcpSd, buffer, sizeof(buffer), 0);
    myId = atoi(buffer);
    
    camera.xCord = 0;
    camera.yCord = 0;

    pthread_create( &reciveUdpData, NULL, recive_udp_data, &(player));


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
        
        // PRob best in sep thread
        
        if (player[myId].mouseX <= 50 && camera.xCord <= 200)
        {
            camera.xCord += 6;
        }
        if (player[myId].mouseX >= 750 && camera.xCord >= -1800)
        {
            camera.xCord -= 6;
        }
        
        
        if (player[myId].mouseY <= 50  && camera.yCord <= 200)
        {
            camera.yCord += 6;
        }
        if (player[myId].mouseY >= 550 && camera.yCord >= -1400)
        {
            camera.yCord -= 6;
        }
        

        
        
        //printf("Player x: %d Camera x: %d");
        //printf("CAm x: %d Y: %d\n", camera.xCord, camera.yCord);
        
        //In its own thread.
        
        //Clears the screen
        glClear( GL_COLOR_BUFFER_BIT );
        
        map(&camera);

        //draws box on screen
        draw(&player[0], &camera);
        draw(&player[1], &camera);
        draw(&player[2], &camera);
        draw(&player[3], &camera);
        draw(&player[4], &camera);
        draw(&player[5], &camera);
        //If once connected player is never removed


        
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

    pthread_cancel(reciveUdpData);
    return 0;
}

void * recive_udp_data(void * parameters)
{
    
    char buffer[64];
    char temp[5];
    int pos, devider = 0;
    struct stcInfo moveInfo;
    
    struct playerInfo * player = (struct playerInfo*) parameters;
    
    
    while (1)
    {
        recvfrom(UdpInfo.udpSd, buffer, sizeof(buffer), 0, UdpInfo.serverIp, sizeof(UdpInfo.serverIp));
    
        sscanf(buffer, "%d,%d,%d,%d,%d", &moveInfo.x, &moveInfo.y , &moveInfo.player, &moveInfo.mouseX, &moveInfo.mouseY);
        //printf("%d,%d,%d,%d,%d\n", moveInfo.x, moveInfo.y , moveInfo.player, moveInfo.mouseX, moveInfo.mouseY);
        //printf("recived from server: %s \n", buffer);
    
        /*
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
    */
        
        //Saves the incoming data in the players struct.
        player[moveInfo.player].slot = moveInfo.player;
        player[moveInfo.player].xCord = moveInfo.x;
        player[moveInfo.player].yCord = moveInfo.y;
        player[moveInfo.player].mouseX = moveInfo.mouseX;
        player[moveInfo.player].mouseY = moveInfo.mouseY;
    }
    
}
