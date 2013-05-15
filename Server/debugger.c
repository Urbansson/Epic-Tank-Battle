#ifndef _DEBUGGER_C
#define _DEBUGGER_C

#include <stdio.h>
#include <math.h>

#include "debugger.h"

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
        sleep(5);//refresh timer
        printf("///////////////////////////////////////////////////////////\n");
    }
}


#endif
