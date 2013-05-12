#ifndef _CLIENTTHREADS_C
#define _CLIENTTHREADS_C

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <pthread.h>

#include <math.h>

#include "internetFuncs.h"
#include "calculations.h"
#include "collision.h"

#include "protocol.h"
#include "clientStruct.h"
#include "clientThreads.h"


void *client_handler_function(void *parameters)
{
    
    char buffer[64], temp[10];
    int pos, devider, number = 0;
    struct ctsCommands commands; //client to server commands
    
    pthread_t broadcastLocation;
    pthread_t tankCalculations;
    pthread_t bulletCalculations;
    
    
    struct client * clientInfo = (struct client*)parameters;
    
    printf("Connection from: %s\n", clientInfo->client_ip_addr);
    
    //Thread that sends the location of the clients position to all connected clients
    pthread_create( &broadcastLocation, NULL, broadcast_location, (clientInfo));
    //Thread that calculates the clients movement
    pthread_create( &tankCalculations, NULL, tank_calculations, (clientInfo));
    
    int i;
    //Recives the clients input and makes the appropriate changes
    while ( 0 < recv(clientInfo->sd, buffer, sizeof(buffer), 0))
    {
        sscanf(buffer, "%d, %d, %c, %c",& commands.mouseX, &commands.mouseY, &commands.mouseInput,&commands.keyboardInput);
        
        clientInfo->mouseX = commands.mouseX;
        clientInfo->mouseY = commands.mouseY;
        clientInfo->mouseInput = commands.mouseInput;
        
        switch(commands.keyboardInput)
        {
            case 'W':
                clientInfo->forward = TRUE;
                break;
            case 'S':
                clientInfo->backward = TRUE;
                break;
            case 'A':
                clientInfo->turnRight = TRUE;
                break;
            case 'D':
                clientInfo->turnLeft = TRUE;
                break;
                
            case 'w':
                clientInfo->forward = FALSE;
                break;
            case 's':
                clientInfo->backward = FALSE;
                break;
            case 'a':
                clientInfo->turnRight = FALSE;
                break;
            case 'd':
                clientInfo->turnLeft = FALSE;
                break;
            default:
                break;
        }
        
        
        if (clientInfo->mouseInput == 'L' &&     clientInfo->fire == 0)
        {
            pthread_create( &bulletCalculations, NULL, bullet_movement_calc, (clientInfo));
        }
    
        
    }
    
    //If the clients quits or connection is lost cancel all the threads
    pthread_cancel(broadcastLocation);
    pthread_cancel(tankCalculations);
    pthread_cancel(bulletCalculations);
    
    //Close sd
    close(clientInfo->sd);
    
    printf("Disconnection from: %s\n", clientInfo->client_ip_addr);
    
    //Clears upd socket so server dont spend time sending to nothing
    udpCliInfo[clientInfo->mySlot].udpsocksd = -1;
    
    //Clear clint information strcut so a new connection can take it
    clear_client_struct(&clientInfo[clientInfo->mySlot]);
    
    return NULL;
}

void *tank_calculations(void *parameters)
{
    struct client * clientInfo = (struct client*)parameters;
    
    double tempAngle;
    float tempX = 120;
    float tempY = 850;
    int oldX, oldY;
    
    for (;;)
    {
        
        
        
        if (clientInfo->turnLeft)
        {
            clientInfo->tankAngle += 1;
        }
        if (clientInfo->turnRight)
        {
            clientInfo->tankAngle -= 1;
        }
        
        if (clientInfo->tankAngle >= 360 )
        {
            clientInfo->tankAngle -= 360;
        }
        if (clientInfo->tankAngle <= -360 )
        {
            clientInfo->tankAngle += 360;
        }
        
        tempAngle = clientInfo->tankAngle*(M_PI/180);
        
        if (clientInfo->forward)
        {
            tempX += clientInfo->speed*cos(tempAngle);
            tempY += clientInfo->speed*sin(tempAngle);
        }
            
        if (clientInfo->backward)
        {
            tempX -= clientInfo->speed*cos(tempAngle);
            tempY -= clientInfo->speed*sin(tempAngle);
        }

        clientInfo->tankCollision = check_tank_tank_collision( &tempX, &tempY, clientInfo->mySlot, otherTanks);
        
        if (clientInfo->tankCollision == 0)
        {
            oldX = tempX;
            oldY = tempY;
        }
        else
        {
            tempX = oldX;
            tempY = oldY;
        }

        map_edge_collision(&tempX, &tempY);
        
        // Sets The collision var and the movement var that will be sent to the server.
        otherTanks[clientInfo->mySlot].x = clientInfo->xLocation = tempX;
        otherTanks[clientInfo->mySlot].y =clientInfo->yLocation = tempY;
        
        //Calculates the Angle
        clientInfo->cannonAngle = (atan2(clientInfo->mouseY-300, clientInfo->mouseX-400))*(180/M_PI);
        
        
         if (clientInfo->bulletHitMe == 1)
         {
             clientInfo->healthPoints -= 35;
             clientInfo->bulletHitMe = 0;
         }
        
        
        if (clientInfo->healthPoints <= 0)
        {
            clientInfo->dead = 1;
            if (clientInfo->team == 1)
            {
                redpoints++;
            }
            else
            {
                bluepoints++;
            }
            sleep(4);
            clientInfo->dead = 0;
            //Random a number in the spawn point on the right side
            tempX = 120;
            tempY = 900;
            clientInfo->healthPoints = 100;
            clientInfo->bulletHitMe = 0;
        }
        
        
        //around 200 calcs every sec
        usleep(5000);
    }
}

void *bullet_movement_calc(void *parameters)
{
    double angle;
    int newX, newY;
    float dxdy;
    int i;
    int tempXLocation, tempYLocation;
    
    struct client * clientInfo = (struct client*)parameters;
    
    clientInfo->fire = 1;
    
    tempXLocation = clientInfo->xLocation + HITBOX_WIDTH/2-7;
    tempYLocation = clientInfo->yLocation + HITBOX_HIGHT/2-7;
    
    angle = clientInfo->cannonAngle*(M_PI/180);
    
    for (i = 30; i <= RANGE+30; i++)
    {
        
        if (clientInfo->bulletHit == 0)
        {
            clientInfo->bulletX = tempXLocation+i*cos(angle);
            clientInfo->bulletY = tempYLocation+i*sin(angle);
        }
        else
        {
            clientInfo->bulletX = -1000;
            clientInfo->bulletY = -1000;        
        }
        
        clientInfo->fire++;
        usleep(2000);
        
    }
    
    clientInfo->bulletHit = 0;
    clientInfo->fire = 0;
    return NULL;
}


void *broadcast_location(void *parameters)
{
    int i;
    char buffer[128] = "\0";
    
    struct client * clientInfo = (struct client*)parameters;
    
    udp_init(clientInfo->client_ip_addr, "6000", &udpCliInfo[clientInfo->mySlot]);
    
    while (1)
    {
        //Formats the information for sending.
        sprintf(buffer, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", clientInfo->team,clientInfo->xLocation,clientInfo->yLocation, clientInfo->mySlot, clientInfo->mouseX, clientInfo->mouseY,clientInfo->fire, clientInfo->bulletX, clientInfo->bulletY, clientInfo->tankAngle, clientInfo->cannonAngle, clientInfo->dead, clientInfo->healthPoints, redpoints, bluepoints);
        //sprintf(buffer, "200,300,0");
        
        //printf("Sending: %s\n", buffer);
        
        //Sends to all Players
        for (i = 0; i < MAX_PLAYERS; i++)
        {
            //Skips slot if no connection
            if( udpCliInfo[i].udpsocksd > 0 )
            {
                if(sendto(udpCliInfo[i].udpsocksd, buffer, strlen(buffer)+1, 0, udpCliInfo[i].p->ai_addr, udpCliInfo[i].p->ai_addrlen) == -1)
                {
                    perror("Send: \n");
                    exit(1);
                }
            }
        }
        
        //waits to not overload client and the network
        usleep(40000);
    }    
}


#endif
