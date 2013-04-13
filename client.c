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

#define LOCAL_PORT 6000
#define MAX_MSG 512

int client_udp_init(/*char portc[]*/)
{
    struct sockaddr_in cliAddr;
    int udpSd, bindCheck;
    
    /*
    int port;
    
    port = atoi(portc);
    */
    
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




void handle_keyboard(SDL_Event * event, int * quit)
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
                    case SDLK_w: printf("W \n"); break;
                    case SDLK_s: printf("S \n"); break;
                    case SDLK_a: printf("A \n"); break;
                    case SDLK_d: printf("D \n"); break;
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


int main(int argc, char *argv[])
{
    int udpSd;
    
    struct sockaddr_in servAddr;
    socklen_t servLen;
    char msg[MAX_MSG];

    int tcpSd;
    
    
    udpSd = client_udp_init(argv[3]);
    if (udpSd == -1)
    {
        printf("udp init failed!\n");
        return;
    }
    
    
    tcpSd = client_tcp_init(argv[1], argv[2]);
    if (udpSd == -1)
    {
        printf("tcp init failed!\n");
        return;
    }
    
    
    //printf("waiting for data on port UDP %s\n", argv[3]);
    
    printf("waiting for data on port UDP %u\n", LOCAL_PORT);

    
    
    /*
    recv(tcpSd, msg, sizeof(msg), 0);
    
    printf("message from tcp: %s", msg);
    
    
    strcpy(msg, argv[3]);
    
    send(tcpSd, msg, sizeof(msg), 0);

    
    recv(tcpSd, msg, sizeof(msg), 0);
    
    
    printf("message from tcp is: %s\n", msg);
    
    
    
    //sleep(5);
    
     
     */

    
    
    //while (1)
    //{

        int quit=0;

        init();

        SDL_Event event;
        while (quit==0)
        {   
             handle_keyboard(&event,&quit);
        }
        
        memset(msg, 0, MAX_MSG);

    
        servLen = sizeof(servAddr);
        recvfrom(udpSd, msg, MAX_MSG, 0, (struct sockaddr *) &servAddr, &servLen);
    
        printf("Message from server: %s", msg);
    
   // }
    
    close(tcpSd);
    close(udpSd);
    
    
    return 0;
}