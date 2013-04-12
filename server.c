#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>

#include <pthread.h>

#include "internetFuncs.h"

#define MAX_PLAYERS 2

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
};

struct udp_info udpCliInfo[MAX_PLAYERS];



void *hello_message_function( void *parameters )
{
    
    struct client * clientInfo = (struct client*)parameters;

    send(clientInfo->sd, "Hello World\n", sizeof("Hello World\n"), 0);

    
    
    
    sleep(10);
    
    clientInfo->free = 0;
    
    close(clientInfo->sd);
    
    return NULL;
}


void *client_handler_function(void *parameters)
{

    char buffer[128] = "HEllo Mister!\n";
    
    struct client * clientInfo = (struct client*)parameters;

    udp_init(clientInfo->client_ip_addr, "6000", &udpCliInfo[clientInfo->mySlot]);

    
    sendto(udpCliInfo[clientInfo->mySlot].udpsocksd, buffer, strlen(buffer)+1, 0, udpCliInfo[clientInfo->mySlot].p->ai_addr, udpCliInfo[clientInfo->mySlot].p->ai_addrlen);




}



int find_free_slot(struct client clientInfo[], int n)
{
    int i;
    for(i=0;i<n;i++)
        if(clientInfo[i].free == 0)
            return i;
    return -1;
}




int main(void)
{
    
    int sd, newSd;
    struct sockaddr_in cliAddr;
    socklen_t cliLen;
    
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
    
    
    for (i = 0; i <= MAX_PLAYERS; i++)
    {
        clientInfo[i].free = 0;
        printf("Setting on %d is %d\n", i, clientInfo[i].free);
    }
    
    
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


        pthread_create( &clientInfo[clientSlot].threadId, NULL, hello_message_function, &(clientInfo[clientSlot]));
        
        
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

