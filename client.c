//
//  client.c
//  
//
//  Created by Theodor Brandt on 2013-04-08.
//
//

#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include <unistd.h> /* close() */

#include <pthread.h>


#define LOCAL_PORT 6000
#define MAX_MSG 512

struct udpData
{
    int udpSd;
    struct sockaddr_in servAddr;
    int xCord;
    int yCord;
};


int client_udp_init()
{
    struct sockaddr_in cliAddr;
    int udpSd, bindCheck;
    
    udpSd=socket(AF_INET, SOCK_DGRAM, 0);
    if(udpSd<0) {
        printf("cannot open socket \n");
        return(-1);
    }
    
    cliAddr.sin_family = AF_INET;
    cliAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    cliAddr.sin_port = htons(LOCAL_PORT);

    bindCheck = bind (udpSd, (struct sockaddr *) &cliAddr,sizeof(cliAddr));
    if(bindCheck<0) {
        printf("cannot bind port number %d \n", LOCAL_PORT);
        return(-1);
    }

    return udpSd;
}

int client_tcp_init(char *serverIp, char *serverPort)
{

    int tcpSd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[100];
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    
    if ((rv = getaddrinfo(serverIp, serverPort, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return -1;
    }
    
    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((tcpSd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            perror("client: socket");
            continue;
        }
        
        if (connect(tcpSd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(tcpSd);
            perror("client: connect");
            continue;
        }
        
        break;
    }
    
    
    if (p == NULL) {
        fprintf(stderr, "Anslutningen misslyckades\n");
        return -1;
    }
    
    freeaddrinfo(servinfo); // all done with this structure


    return tcpSd;
}




void handle_keyboard(SDL_Event * event, int * quit, int tcpSd)
{
        
    while( SDL_PollEvent ( event ))
    {
        switch (event->type)
        {
            case SDL_MOUSEBUTTONDOWN:
            {
                switch(event->button.button)
                {
                    case SDL_BUTTON_LEFT: printf("LMB \n"); break;
                    case SDL_BUTTON_RIGHT: printf("RMB \n"); break;
                }
            }
            case SDL_KEYDOWN:
            {
                switch(event -> key.keysym.sym)
                {
                    case SDLK_w:
                        send(tcpSd, "W", sizeof("W"), 0);
                        //printf("W \n");
                        break;
                    case SDLK_s:
                        send(tcpSd, "S", sizeof("S"), 0);
                        //printf("S \n");
                        break;
                    case SDLK_a:
                        send(tcpSd, "A", sizeof("A"), 0);
                        //printf("A \n");
                        break;
                    case SDLK_d:
                        send(tcpSd, "D", sizeof("D"), 0);
                        //printf("D \n");
                        break;
                }
            }

        }
    }

        if(event->type==SDL_QUIT)
        {
            *quit = 1;
        }
}


int init()
{
    if(SDL_Init( SDL_INIT_EVERYTHING) < 0)
    {
        return 0;
    }

    if(SDL_SetVideoMode(800,600,32,SDL_SWSURFACE)==NULL)
    {
        return 0;
    }
    return 1;
}

void * recive_data( void *parameters )
{
    char msg[MAX_MSG];
    socklen_t servLen;
    
    printf("HEJSAN FRÅN THREAD!\n");
    
    struct udpData * udpInfo = (struct udpData*)parameters;

    while (1)
    {
        

    
    memset(msg, 0, MAX_MSG);    
    servLen = sizeof(udpInfo->servAddr);
    
    recvfrom(udpInfo->udpSd, msg, sizeof(MAX_MSG), 0, (struct sockaddr *) &udpInfo->servAddr, &servLen);
    
    printf("Message from server: %s\n", msg);
    }
    /*
    
    while (1)
    {
        memset(msg, 0, MAX_MSG);
        
        servLen = sizeof(udpInfo->servAddr);
        recvfrom(udpInfo->udpSd, msg, MAX_MSG, 0, (struct sockaddr *) &udpInfo->servAddr, &servLen);
        
        printf("Message from server: %s", msg);
        
    }

*/

}




int main(int argc, char *argv[])
{
    //int udpSd;
    
    struct udpData udpInfo;
    
    //struct sockaddr_in servAddr;
    socklen_t servLen;
    char msg[MAX_MSG];

    int tcpSd;
    
    pthread_t reciveFromServer;
    
    
    udpInfo.udpSd = client_udp_init();
    if (udpInfo.udpSd == -1)
    {
        printf("udp init failed!\n");
        return;
    }
    
    
    tcpSd = client_tcp_init(argv[1], argv[2]);
    if (tcpSd == -1)
    {
        printf("tcp init failed!\n");
        return;
    }
    
    pthread_create( &reciveFromServer, NULL, recive_data, &(udpInfo));

    
    int quit=0;

    init();

    SDL_Event event;
    while (quit==0)
    {
        handle_keyboard(&event,&quit, tcpSd);
    }
    
    /*
    
    while (1)
    {

        memset(msg, 0, MAX_MSG);
    
        servLen = sizeof(udpInfo.servAddr);
        recvfrom(udpInfo.udpSd, msg, MAX_MSG, 0, (struct sockaddr *) &udpInfo.servAddr, &servLen);
    
        printf("Message from server: %s", msg);
    
   }
    
    */
    close(tcpSd);
    close(udpInfo.udpSd);
    
    
    return 0;
}