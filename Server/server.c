#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <pthread.h>
#include "internetFuncs.h"

#include "protocol.h"

#define MAX_PLAYERS 6


struct client
{
    pthread_t threadId;         //Thread Id
    int mySlot;                 //My Slot on the server
    int sd;                     //TCP-Socket descriptor
    char client_ip_addr[20];    //Clients ip-address
    int free;                   //Flag that indicates if the slot is free. 1 == taken 0 == free
    
    int xLocation;
    int yLocation;
    int xVelocity;
    int yVelocity;
    
    int mouseX;
    int mouseY;
    char mouseInput;
    
};

struct udp_info udpCliInfo[MAX_PLAYERS];

void clear_client_struct(struct client *clientInfo);

void *client_handler_function(void *parameters);

void server_debugger_print(struct client clientInfo, int place);

void * debeugger_print_thread(void *parameters);

int find_free_slot(struct client clientInfo[], int n);

void *movement_calculations(void *parameters);


void *broadcast_location();


int main(void)
{
    
    int sd, newSd;              //Socket descriptors sd is the one main listens on newSd is the one clients gets.
    struct sockaddr_in cliAddr; //Information about the client
    socklen_t cliLen;           //
    char buffer[5];

    pthread_t server_db_print;  //Debug print thread information
    
    struct client clientInfo[MAX_PLAYERS];  // Where all necessary information about the client is stored
    
    int clientSlot;             // Stores in slot the new connection is going to get 
    
    int i;
    
    //char buffer[128];
    
    
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
//        udpCliInfo[1].udpsocksd = -1;
    }
    
    
    //Starts the debug print thread
    //pthread_create(&server_db_print,NULL,debeugger_print_thread,clientInfo);

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
        
        sprintf(buffer, "%d", clientSlot);
        send(newSd, buffer, sizeof(buffer), 0);

        
        clientInfo[clientSlot].free = 1;            // Sets the client slot so it is taken
        clientInfo[clientSlot].sd = newSd;          // Gives the slot the socket descriptor
        clientInfo[clientSlot].mySlot = clientSlot; // Gives the Slot number 
                
        // Gets the clients ip address and stores it
        inet_ntop(cliAddr.sin_family, &cliAddr.sin_addr, clientInfo[clientSlot].client_ip_addr, sizeof (clientInfo[clientSlot].client_ip_addr));
        
            
        //Main thread for the connected client, Main programm will serv new connections
        pthread_create( &clientInfo[clientSlot].threadId, NULL, client_handler_function, &(clientInfo[clientSlot]));

        
    }


    // Will mostlikly never happen
    close(sd);

    
    return 0;
}


int find_free_slot(struct client clientInfo[], int n)
{
    int i;
    for(i=0;i<n;i++)
        if(clientInfo[i].free == 0)
            return i;           // Returns the fist free spot.
    return -1;                  // Return -1 if no spots.
}


void clear_client_struct(struct client *clientInfo)
{
    clientInfo->mySlot = -1;                            //My Slot on the server
    clientInfo->threadId = 0;                           //Thread Id
    clientInfo->sd =  0;                                //TCP-Socket descriptor
    strcpy(clientInfo->client_ip_addr, "0.0.0.0");      //Clients ip-address
    clientInfo->free = 0;                               //Flag that indicates if the slot is free. 1 == taken 0 == free
}

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



void *client_handler_function(void *parameters)
{
    
    char buffer[64], temp[10];
    int pos, devider, number = 0;
    struct ctsCommands commands; //client to server commands
    
    struct stcInfo info;    //Server to client info

    pthread_t broadcastLocation;
    pthread_t calculations;
    
    struct client * clientInfo = (struct client*)parameters;
    
    //Set start postion for clitent
    clientInfo->xLocation = 0;
    clientInfo->yLocation = 0;
    
    
    
    printf("Connection from: %s\n", clientInfo->client_ip_addr);
    
    /*
    sprintf(buffer, "%d", clientInfo->mySlot);
    send(clientInfo->sd, buffer, sizeof(buffer), 0);
    printf("Sending: %s\n", buffer);
    */
    
    //Thread that sends the location of the clients position to all connected clients
    pthread_create( &broadcastLocation, NULL, broadcast_location, (clientInfo));
    //Thread that calculates the clients movement
    pthread_create( &calculations, NULL, movement_calculations, (clientInfo));


    //Recives the clients input and makes the appropriate changes
    while ( 0 < recv(clientInfo->sd, buffer, sizeof(buffer), 0))
    {
        
        
     //buffer[sizeof(buffer)] = '\n';
        sscanf(buffer, "%d, %d, %c, %c",& commands.mouseX, &commands.mouseY, &commands.mouseInput,&commands.keyboardInput);
        
        //printf("X: %d Y: %d Mouse: %c Key: %c\n", commands.mouseX, commands.mouseY,commands.mouseInput,commands.keyboardInput);
        
        clientInfo->mouseX = commands.mouseX;
        clientInfo->mouseY = commands.mouseY;
        clientInfo->mouseInput = commands.keyboardInput;
        
        switch(commands.keyboardInput)
        {
            case 'W':
                clientInfo->yVelocity = -1;
                break;
            case 'S':
                clientInfo->yVelocity = +1;
                break;
            case 'A':
                clientInfo->xVelocity = -1;
                break;
            case 'D':
                clientInfo->xVelocity = +1;
                break;
                
            case 'w':
                clientInfo->yVelocity += 1;
                break;
            case 's':
                clientInfo->yVelocity -= 1;
                break;
            case 'a':
                clientInfo->xVelocity += 1;
                break;
            case 'd':
                clientInfo->xVelocity -= 1;
                break;
         }
    }

    
    
    //If the clients quits or connection is lost cancel the broadcast thread
    pthread_cancel(broadcastLocation);
    pthread_cancel(calculations);


    //Close sd
    close(clientInfo->sd);
    
    //Clears upd socket so server dont spend time sending to nothing
    udpCliInfo[clientInfo->mySlot].udpsocksd = -1;
    //Clear clint information strcut so a new connection can take it
    clear_client_struct(&clientInfo[clientInfo->mySlot]);
    
    return NULL;
}

void *movement_calculations(void *parameters)
{
    
    struct client * clientInfo = (struct client*)parameters;

    for (;;)
    {
        
        //fast fix for bugging controlls
        if (clientInfo->xVelocity > 1)
        {
            clientInfo->xVelocity = 1;
        }
        if (clientInfo->xVelocity < -1)
        {
            clientInfo->xVelocity = -1;
        }
        if (clientInfo->yVelocity > 1)
        {
            clientInfo->yVelocity = 1;
        }
        if (clientInfo->yVelocity < -1)
        {
            clientInfo->yVelocity = -1;
        }
        
        
        
        
        //Limits the client to the screen
        clientInfo->xLocation += clientInfo->xVelocity;
        
        if (clientInfo->xLocation < 0 || clientInfo->xLocation+ 64 > 2400 )
        {
            clientInfo->xLocation -= clientInfo->xVelocity;
        }
        
        clientInfo->yLocation += clientInfo->yVelocity;
        
        if (clientInfo->yLocation < 0 || clientInfo->yLocation + 45 > 1800 )
        {
            clientInfo->yLocation -= clientInfo->yVelocity;
        }
        
        
        
        
        //around 200 calcs every sec
        usleep(5000);
    }

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
        sprintf(buffer, "%d,%d,%d,%d,%d", clientInfo->xLocation,clientInfo->yLocation, clientInfo->mySlot, clientInfo->mouseX, clientInfo->mouseY);
        
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
        usleep(35000);
    }
    
}












