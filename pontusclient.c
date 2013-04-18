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

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SCREEN_BPP 32


const int SQUARE_WIDTH = 15;
const int SQUARE_HEIGHT = 15;


struct udpData
{
    int udpSd;
    struct sockaddr_in servAddr;
    int mySlot;
    int xCord;
    int yCord;
};

struct square_t
{
    float x, y;
    float xVel, yVel;
    float r, g, b;
};

struct timer_t2
{
    int startTicks;
    int pausedTicks;
    int paused;
    int started;
};

void CheckMouseHover(int *x, int *y)
{
    int mouse_x, mouse_y;

    SDL_PumpEvents();

    SDL_GetMouseState(&mouse_x, &mouse_y);
    usleep(500000);
    *x = mouse_x; *y = mouse_y;
     
}

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
                break;
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
                break;
            case SDL_KEYUP:
            {
                switch(event -> key.keysym.sym)
                {
                    case SDLK_w:
                        send(tcpSd, "w", sizeof("W"), 0);
                        //printf("W \n");
                        break;
                    case SDLK_s:
                        send(tcpSd, "s", sizeof("S"), 0);
                        //printf("S \n");
                        break;
                    case SDLK_a:
                        send(tcpSd, "a", sizeof("A"), 0);
                        //printf("A \n");
                        break;
                    case SDLK_d:
                        send(tcpSd, "d", sizeof("D"), 0);
                        //printf("D \n");
                        break;
                }
            }
                break;
        }
    }

        if(event->type==SDL_QUIT)
        {
            *quit = 1;
        }
}



int init_opengl()
{
    //Set clear color
    glClearColor(0,0,0,0);
    
    
    //Kolla vadfan som händer här
    // Set Projection
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1, 1 );
    
    //Initialize modelview matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    
    if( glGetError() != GL_NO_ERROR )
    {
        return 0;
    }
    
    return 1;
}

int init()
{
    SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);

    if (SDL_Init( SDL_INIT_EVERYTHING ) < 0)
    {
        return 0;
    }
    if (SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_OPENGL ) == NULL)
    {
        return 0;
    }
    
    if (init_opengl() == 0)
    {
        return 0;
    }
    
    SDL_WM_SetCaption("PixelCS", NULL);
}



void * recive_data( void *parameters )
{
    char msg[MAX_MSG], temp[4];
    socklen_t servLen;
    int pos,mousePosX , mousePosY;
    char mouseCoordinates[8];
    char comp = ',';
    
    struct udpData * udpInfo = (struct udpData*)parameters;

    while (1)
    {
    
        memset(msg, 0, MAX_MSG);    
        servLen = sizeof(udpInfo->servAddr);
    
        recvfrom(udpInfo->udpSd, msg, sizeof(msg), 0, (struct sockaddr *) &udpInfo->servAddr, &servLen);
        
        
        pos = 0;
        while (1)
        {
            //Bugg i virtual box går ej att msg[pos] == ','
            if (msg[pos] == comp)
            {
                break;
            
            }
            pos++;
        }
        
        
        strncpy(temp, msg, pos);
        temp[pos] = '\0';
        udpInfo->xCord = atoi(temp);
        
        strncpy(temp, &msg[pos+1], 3);
        temp[3] = '\0';
        udpInfo->yCord = atoi(temp);
        
        strncpy(temp, &msg[pos+4], 3);
        udpInfo->mySlot = atoi(temp);

        CheckMouseHover(&mousePosX,&mousePosY);
        sprintf(mouseCoordinates,"%d,%d",mousePosX,mousePosY);
        //printf("%s\n",mouseCoordinates);
        printf("Message from server: %s Mus X,Y: %s \n", msg ,mouseCoordinates);
    }

}


void show(struct square_t * square)
{
    glTranslatef( square->x, square->y, 0);
    
    glBegin( GL_QUADS );
    
    glColor4f( square->r, square->g, square->b, 1.0);
    
    glVertex2f( 0,0);
    glVertex2f( 15,0);
    glVertex2f( 15,15);
    glVertex2f( 0,15);
    
    glEnd();
    
    //Reset
    glLoadIdentity();
    
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

    struct square_t player[6];
    
    struct timer_t2 fps;

    
    player[0].x=0;
    player[0].y=0;
    player[0].xVel=0;
    player[0].yVel=0;
    player[0].r=1;
    player[0].g=0;
    player[0].b=0;
    
    
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
    
    init();
    
    pthread_create( &reciveFromServer, NULL, recive_data, &(udpInfo));

    
    
    int quit=0;



    SDL_Event event;
    
    
    while (quit==0)
    {
        handle_keyboard(&event,&quit, tcpSd);
    
        if (udpInfo.mySlot == 0)
        {
            player[0].x= udpInfo.xCord;
            player[0].y= udpInfo.yCord;
        }
        
        
    glClear( GL_COLOR_BUFFER_BIT );
    
    //Show the squares
    show(&player[0]);
        
    show(&player[1]);
    
    glFlush();
    
    SDL_GL_SwapBuffers();
    
    sleep(0);
    
    }
    
    close(tcpSd);
    close(udpInfo.udpSd);
    
    
    return 0;
}