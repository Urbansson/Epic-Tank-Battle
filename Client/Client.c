#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <sys/types.h>
#include "audio.h"

#include "SDLfuncs.h"
#include "timers.h"
#include "clientinternetfuncs.h"
#include "protocol.h" 
#include "render.h"

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
    GLuint texture[2];
    
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
    
    texture[0] = load_image("flower.png");
    texture[1] = load_image("./Images/WorldMap1.png");

    
    recv(tcpSd, buffer, sizeof(buffer), 0);
    myId = atoi(buffer);
    
    camera.xCord = 0;
    camera.yCord = 0;
    player[1].xCord = 0;
    player[1].yCord = 0;

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
             
             handel_input(&event, tcpSd );
            
         }
        
        //printf("Player x: %d player y: %d\n", player[myId].xCord, player[myId].yCord);
        //printf("CAm x: %d Y: %d\n", camera.xCord, camera.yCord);
        camera.xCord = -player[myId].xCord;
        camera.yCord = -player[myId].yCord;
        
        
        //In its own thread.
        
        //Clears the screen
        glClear( GL_COLOR_BUFFER_BIT );
        
        //map(&player[myId]);
        draw_map(&texture[1], &player[myId]);

        //draws your tank on screen
        draw_self(&player[myId], &camera);
        
        if (player[myId].fire == 1)
        {
            draw_bullet(&player[myId], &camera);
        }
        //Draws the other players tanks on the screen
        //draw_other(&player[1], &camera);
        

        //draw_image(&texture[0]);
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
    
        printf("recived from server: %s \n", buffer);

        
        sscanf(buffer, "%d,%d,%d,%d,%d,%d,%d,%d", &moveInfo.x, &moveInfo.y , &moveInfo.player, &moveInfo.mouseX, &moveInfo.mouseY,&moveInfo.fire, &moveInfo.bulletX, &moveInfo.bulletY);
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
        
        player[moveInfo.player].bulletX = moveInfo.bulletX;
        player[moveInfo.player].bulletY = moveInfo.bulletY;
        player[moveInfo.player].fire = moveInfo.fire;

        
    }
    
}
