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

#define MAX_PLAYERS 6

#define HITBOX_WIDTH 64
#define HITBOX_HIGHT 45
#define RANGE 600

#define TRUE 1
#define FALSE 0

void clear_client_struct(struct client *clientInfo);

void server_debugger_print(struct client clientInfo, int place);

void * debeugger_print_thread(void *parameters);

int find_free_slot(struct client clientInfo[], int n);

int find_team(struct client clientInfo[], int n);

//void * bullet_hit_thread(void *parameters);

void load_map_collision_array();

int main(void)
{
    int sd, newSd;              //Socket descriptors sd is the one main listens on newSd is the one clients gets.
    struct sockaddr_in cliAddr; //Information about the client
    socklen_t cliLen;           //
    char buffer[32];

    pthread_t server_db_print;  //Debug print thread information
    pthread_t bullet_hit;
    struct client clientInfo[MAX_PLAYERS];  // Where all necessary information about the client is stored
    
    int clientSlot;             // Stores in slot the new connection is going to get 
    
    int i;
    
    //load_map_collision_array();
    
    // Init tcp
    sd = tcp_init();
    if (sd == -1)
    {
        return ERROR;           // Exits if error
    }

    //Cleares the Clients structs and makes it ready for use.
    for (i = 0; i < MAX_PLAYERS; i++)
    {
        clear_client_struct(&clientInfo[i]);
    }
    redpoints = 0;
    bluepoints = 0;
    
    //Starts the debug print thread
    //pthread_create(&server_db_print,NULL,debeugger_print_thread,clientInfo);
    
    pthread_create(&bullet_hit,NULL,bullet_hit_thread,clientInfo);

    
    while (1)
    {
        cliLen = sizeof(cliAddr);
        newSd = accept(sd, (struct sockaddr *) &cliAddr, &cliLen);
        
        if (newSd == -1)
        {
            perror("accept");
            continue;
        }
        
        //Finds the first free slot
        clientSlot = find_free_slot(clientInfo, MAX_PLAYERS);
                
        if(clientSlot==-1)
        {
            send(newSd,"No free slots, try again later!\n", sizeof("No free slots, try again later!\n"), 0);
            close(newSd); // Close the socket descriptor to indicate to client that
            continue;     // we have no more space for it. Then goto beginning of loop.
        }
        
        clientInfo[clientSlot].free = 1;            // Sets the client slot so it is taken
        clientInfo[clientSlot].sd = newSd;          // Gives the slot the socket descriptor
        clientInfo[clientSlot].mySlot = clientSlot; // Gives the Slot number 
        clientInfo[clientSlot].team = find_team(clientInfo, MAX_PLAYERS);  //Finds a team and givs in to the client
        
        // Gets the clients ip address and stores it
        inet_ntop(cliAddr.sin_family, &cliAddr.sin_addr, clientInfo[clientSlot].client_ip_addr, sizeof (clientInfo[clientSlot].client_ip_addr));
        
        sprintf(buffer, "%d", clientSlot);
        send(newSd, buffer, sizeof(buffer), 0);
        
        //Main thread for the connected client, Main programm will continue serv new connections
        pthread_create( &clientInfo[clientSlot].threadId, NULL, client_handler_function, &(clientInfo[clientSlot]));        
    }

    //This will never happen unless we quit the programm
    
    
    pthread_cancel(server_db_print);
    pthread_cancel(bullet_hit);
    // Will never happen
    close(sd);

    return 0;
}

void load_map_collision_array()
{
    FILE *file;
    
    file = fopen("./maps/worldMap.bin", "rb");
    
    if (!file)
    {
        printf("Unable to open file");
    }
    else
    {
        fread(mapArray, sizeof(mapArray), 1, file);
    }
    
    fclose(file);
    
    printf("WorldMap loaded!\n");
}

/*
//Flytta till annan fil
void * bullet_hit_thread(void *parameters)
{
    int i, k;    
    struct client *clientInfo = (struct client *)parameters;

    while (TRUE)
    {
        for (i = 0; i < MAX_PLAYERS; i++)
        {
            for (k = 0; k < MAX_PLAYERS; k++)
            {
                if (clientInfo[i].mySlot > -1  && clientInfo[k].mySlot > -1 && i != k && clientInfo[i].bulletHit == 0)
                {                    
                    if (clientInfo[i].bulletX <= clientInfo[k].xLocation + 64 && clientInfo[i].bulletX + 10 >= clientInfo[k].xLocation)
                    {
                        if (clientInfo[i].bulletY <= clientInfo[k].yLocation + 45 && clientInfo[i].bulletY + 10 >= clientInfo[k].yLocation)
                        {
                            clientInfo[i].bulletHit = 1;
                            clientInfo[k].bulletHitMe = 1;
                        }
                    }
                }
            }
        }
    }
}
*/


//flytta till annan fil client 
int find_free_slot(struct client clientInfo[], int n)
{
    int i;
    for(i=0;i<n;i++)
        if(clientInfo[i].free == 0)
            return i;           // Returns the fist free spot.
    return -1;                  // Return -1 if no spots.
}

//flytta till annan fil
int find_team(struct client clientInfo[], int n)
{
    int i;
    int red = 0, blue = 0;
    
    for(i=0;i<n;i++)
    {
        if(clientInfo[i].team == 1)
            blue++;
        
        if(clientInfo[i].team == 2)
            red++;
    }
        
    if (blue < red)
        return 1;
    
    if (blue > red)
        return 2;
    
    return 1;
}

//flytta till annan fil
void clear_client_struct(struct client *clientInfo)
{
    clientInfo->mySlot = -1;                 //My Slot on the server
    clientInfo->sd = 0;                     //TCP-Socket descriptor
    strcpy(clientInfo->client_ip_addr, "0.0.0.0");      //Clients ip-address
    
    clientInfo->team = 0;                   //Indicates what team the client are in;
    
    clientInfo->xLocation = 0;              //Where the client are located on the x-axis
    clientInfo->yLocation = 0;              //Where the client are located on the y-axis
    
    clientInfo->forward = 0;                //if the client are moving forward
    clientInfo->backward = 0;               //if the client are moving backwards
    clientInfo->turnLeft = 0;               //if the client are turning left
    clientInfo->turnRight = 0;              //if the client are turning right
    
    clientInfo->tankAngle = 0;              //The angle of the tank
    clientInfo->cannonAngle = 0;            //The angle of the cannom
    
    clientInfo->mouseX = 0;                 //Mouse x pos
    clientInfo->mouseY = 0;                 //Mouse y pos
    clientInfo->mouseInput = 0;            //What button on the mouse the client is pressing
    
    clientInfo->fire = 0;                   // If the client fire or not
    clientInfo->bulletX = -1000;                //Bullet X
    clientInfo->bulletY = -1000;                //Bullet Y
    clientInfo->bulletHit = 0;              //IF the bullet hit something
    
    clientInfo->speed = 1;                  //THe tanks speed
    clientInfo->tankCollision = 0;          // Zero if no collision if collision the one you are col
    clientInfo->mapCollision = 0;          // Zero if no collision if collision the one you are col

    
    clientInfo->healthPoints = 100;           //Health of the tank 100 max 0 dead
    clientInfo->dead = 0;
    
    clientInfo->free = 0;                   //Flag that indicates if the slot is free. 1 == taken 0 == free
}



//DEBUGGER FILE
void server_debugger_print(struct client clientInfo, int place)
{
    printf("===============DEBUG PRINT=================\n");
    
    printf("Clients server Slot: %d\n", clientInfo.mySlot+1);
    printf("Clients Ip-address: %s\n", clientInfo.client_ip_addr);
    
    if (clientInfo.free == 0)
        printf("Server free flag: Free\n");
    else
        printf("Server free flag: Taken\n");
    
    printf("\n");
    printf("\n");
    printf("===========================================\n");
}


void * debeugger_print_thread(void *parameters)
{
    int i;
    
    struct client *clientInfo = (struct client *)parameters;
    for (;;)
    {
        for (i = 0; i < MAX_PLAYERS ; i++)
        {
            server_debugger_print(clientInfo[i], i);
        }
        sleep(2);//refresh timer
        printf("///////////////////////////////////////////////////////////\n");
    }
}




