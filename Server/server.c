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


#include "protocol.h"

#define MAX_PLAYERS 6

#define HITBOX_WIDTH 64
#define HITBOX_HIGHT 45
#define RANGE 600

#define TRUE 1
#define FALSE 0

struct client
{
    pthread_t threadId;         //Thread Id
    int mySlot;                 //My Slot on the server
    int sd;                     //TCP-Socket descriptor
    char client_ip_addr[20];    //Clients ip-address
    int free;                   //Flag that indicates if the slot is free. 1 == taken 0 == free
    
    int xLocation;
    int yLocation;
    
    int forward;
    int backward;
    int turnLeft;
    int turnRight;

    int tankAngle;
    int cannonAngle;
    
    int mouseX;
    int mouseY;
    char mouseInput;
    
    int fire; // If We fire or not
    int bulletX;
    int bulletY;
    int bulletHit;
    
    int speed;
    int collision;
};

struct udp_info udpCliInfo[MAX_PLAYERS];

void clear_client_struct(struct client *clientInfo);

void *client_handler_function(void *parameters);

void server_debugger_print(struct client clientInfo, int place);

void * debeugger_print_thread(void *parameters);

int find_free_slot(struct client clientInfo[], int n);

void *tank_calculations(void *parameters);

void *bullet_movement_calc(void *parameters);

void *_calculations(void *parameters);


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

    // Will never happen
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
    clientInfo->cannonAngle = 0;                         
    clientInfo->tankAngle = 0;
    clientInfo->fire = 0;
    clientInfo->speed = 1;
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
    pthread_t tankCalculations;
    pthread_t bulletCalculations = 0;

    
    struct client * clientInfo = (struct client*)parameters;
    
    printf("Connection from: %s\n", clientInfo->client_ip_addr);
    
    //Thread that sends the location of the clients position to all connected clients
    pthread_create( &broadcastLocation, NULL, broadcast_location, (clientInfo));
    //Thread that calculates the clients movement
    pthread_create( &tankCalculations, NULL, tank_calculations, (clientInfo));


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
    
    //If the clients quits or connection is lost cancel the broadcast thread
    pthread_cancel(broadcastLocation);
    pthread_cancel(tankCalculations);
    pthread_cancel(bulletCalculations);

    //Close sd
    close(clientInfo->sd);
    
    //Clears upd socket so server dont spend time sending to nothing
    udpCliInfo[clientInfo->mySlot].udpsocksd = -1;
    //Clear clint information strcut so a new connection can take it
    clear_client_struct(&clientInfo[clientInfo->mySlot]);
    
    return NULL;
}

void *tank_calculations(void *parameters)
{
    double tempAngle;
    float tempX = 120, tempY = 850;
    
    
    struct client * clientInfo = (struct client*)parameters;

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
        
        
        clientInfo->xLocation = tempX;
        clientInfo->yLocation = tempY;
        
        //Calculates the 
        clientInfo->cannonAngle = (atan2(clientInfo->mouseY-300, clientInfo->mouseX-400))*(180/M_PI);

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

    angle = (atan2(clientInfo->mouseY-300, clientInfo->mouseX-400));
    
    for (i = 30; i <= RANGE+30; i++)
    {
        clientInfo->bulletX = tempXLocation+i*cos(angle);
        clientInfo->bulletY = tempYLocation+i*sin(angle);
        usleep(3000);
        
        clientInfo->fire++;
    }
    
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
        sprintf(buffer, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", clientInfo->xLocation,clientInfo->yLocation, clientInfo->mySlot, clientInfo->mouseX, clientInfo->mouseY,clientInfo->fire, clientInfo->bulletX, clientInfo->bulletY, clientInfo->tankAngle, clientInfo->cannonAngle);
        
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
        usleep(3000);
    }
    
}












