#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>

#include <pthread.h>

#include "internetFuncs.h"

#define MAX_PLAYERS 6

/*
#define PORT 5000  // the port users will be connecting to
#define BACKLOG 10     // how many pending connections queue will hold

#define ERROR -1
#define SUCCESS 0

*/

struct client
{
    pthread_t threadId;         //Thread Id
    int mySlot;                 //My Slot on the server
    int sd;                     //TCP-Socket descriptor
    char client_ip_addr[20];    //Clients ip-address
    int free;                   //Flag that indicates if the slot is free. 1 == taken 0 == free
    int xLocation;
    int yLocation;
    
    
    
    
    
};

struct udp_info udpCliInfo[MAX_PLAYERS];


void clear_client_struct(struct client *clientInfo);

void *hello_message_function( void *parameters );

void *client_handler_function(void *parameters);

void server_debugger_print(struct client clientInfo, int place);

void * debeugger_print_thread(void *parameters);

int find_free_slot(struct client clientInfo[], int n);

void *broadcast_location();


int main(void)
{
    
    int sd, newSd;
    struct sockaddr_in cliAddr;
    socklen_t cliLen;
    
    
    pthread_t server_db_print;
    //struct udp_info udpCliInfo;
    
    
    struct client clientInfo[MAX_PLAYERS];
    
    int clientSlot;
    
    int i;
    
    char s[40];
    char buffer[128] = "Hello World!\n";
    
    sd = tcp_init();
    if (sd == -1)
    {
        return ERROR;
    }

    
    printf("Waiting for connection... \n");
    
    
    for (i = 0; i < MAX_PLAYERS; i++)
    {
        clear_client_struct(&clientInfo[i]);
        udpCliInfo[1].udpsocksd = -1;
    }
    
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
        
        clientSlot = find_free_slot(clientInfo, MAX_PLAYERS);   //Finds the first free slot
        
        if(clientSlot==-1)
        {
            send(newSd,"No free slots, try again later!\n", sizeof("No free slots, try again later!\n"), 0);
            close(newSd); // Close the socket descriptor to indicate to client that
            continue;     // we have no more space for it. Then goto beginning of loop.
        }
        
        clientInfo[clientSlot].free = 1;            // Sets the client slot so it is taken
        clientInfo[clientSlot].sd = newSd;          // Gives the slot the socket descriptor
        clientInfo[clientSlot].mySlot = clientSlot; // Gives the Slot number 
        
        
        inet_ntop(cliAddr.sin_family, &cliAddr.sin_addr, clientInfo[clientSlot].client_ip_addr, sizeof (clientInfo[clientSlot].client_ip_addr));
        // Gets the clients ip address and stores it
        
        //pthread_create( &clientInfo[clientSlot].threadId, NULL, hello_message_function, &(clientInfo[clientSlot]));
        
        
        pthread_create( &clientInfo[clientSlot].threadId, NULL, client_handler_function, &(clientInfo[clientSlot]));

        //Main thread for the connected client, Main programm will serv new connections

        
        
         /*
        udp_init(s, "6000", &udpCliInfo);
        
        sendto(udpCliInfo.udpsocksd, buffer, strlen(buffer)+1, 0, udpCliInfo.p->ai_addr, udpCliInfo.p->ai_addrlen);
                
         */
        
    }

    //close(sd);

    
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
    clientInfo->sd = 0;                                 //TCP-Socket descriptor
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
    
    //printf("Server Free Flag: %d\n", clientInfo.free);
    
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
        sleep(2);
        printf("///////////////////////////////////////////////////////////\n");
        //system("clear");
    }
}



void *hello_message_function( void *parameters )
{
    
    char buffer[100];
    
    struct client * clientInfo = (struct client*)parameters;
    
    send(clientInfo->sd, "Hello World\n", sizeof("Hello World\n"), 0);
    
    
    recv(clientInfo->sd, buffer, sizeof (buffer), 0);
    
    
    
    send(clientInfo->sd, buffer, sizeof(buffer), 0);

    
    
    
    sleep(5);
    
    close(clientInfo->sd);
    
    
    clear_client_struct(&clientInfo[clientInfo->mySlot]);

    
    return NULL;
}


void *client_handler_function(void *parameters)
{
    
    char buffer[64] = "HEllo Mister!\n";
    char port[10];
    pthread_t broadcastLocation;

    
    struct client * clientInfo = (struct client*)parameters;
    
    clientInfo->xLocation = 0;
    clientInfo->yLocation = 0;
    
    
    printf("Connection from: %s\n", clientInfo->client_ip_addr);
    
    pthread_create( &broadcastLocation, NULL, broadcast_location, (clientInfo));


    
    while ( 0 < recv(clientInfo->sd, buffer, sizeof(buffer), 0))
    {
     buffer[2] = '\n';
        
//        printf("Input from Client: %s\n", buffer);

        switch(buffer[0])
        {
            case 'W':
                clientInfo->yLocation -= 1;
                break;
            case 'S':
                clientInfo->yLocation += 1;
                break;
            case 'A':
                clientInfo->xLocation -= 1;
                break;
            case 'D':
                clientInfo->xLocation += 1;
                break;
        }
    }

    
    
    
    pthread_join (broadcastLocation, NULL);

    
    //sleep();
    
    close(clientInfo->sd);
    
    
    clear_client_struct(&clientInfo[clientInfo->mySlot]);
}



void *broadcast_location(void *parameters)
{
    int i;
    char buffer[512] = "Hello\n";


    struct client * clientInfo = (struct client*)parameters;

    udp_init(clientInfo->client_ip_addr, "6000", &udpCliInfo[clientInfo->mySlot]);

    sendto(udpCliInfo[clientInfo->mySlot].udpsocksd, buffer, strlen(buffer)+1, 0, udpCliInfo[clientInfo->mySlot].p->ai_addr, udpCliInfo[clientInfo->mySlot].p->ai_addrlen);

    
    
    

    while (1)
    {
        sprintf(buffer, "%d,%d", clientInfo->xLocation,clientInfo->yLocation);
        
        for (i = 0; i < MAX_PLAYERS; i++)
        {
            if( udpCliInfo[i].udpsocksd > 0 )
            {
                if(sendto(udpCliInfo[i].udpsocksd, buffer, strlen(buffer)+1, 0, udpCliInfo[i].p->ai_addr, udpCliInfo[i].p->ai_addrlen) == -1)
                {
                    perror("Send: \n");
                    exit(1);
                }
            }
        }
        
        usleep(500000);
    }
    
}












