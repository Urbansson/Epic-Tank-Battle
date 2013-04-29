#ifndef _INTERNETFUNCS_C
#define _INTERNETFUNCS_C


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>

#include "internetFuncs.h"

int tcp_init(void)
{
    int sd;
    struct sockaddr_in servAddr;
    int yes = 1;
    
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd <0 )
    {
        perror("Error opening socket!\n");
        return ERROR;
    }
    
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(PORT);
    memset(servAddr.sin_zero,'\0',sizeof(servAddr.sin_zero));
    
    if (setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) {
        perror("setsockopt");
        return ERROR;
    }
    
    if (bind(sd, (struct sockaddr *) &servAddr, sizeof(servAddr))<0)
    {
        perror("Error binding socket\n");
        return ERROR;
    }
    
    
    listen(sd, BACKLOG);
    
    return sd;
}

int udp_init(char * remote_ip, char * port, struct udp_info * udpCliInfo)
{
    int rv;
    
    memset(&(udpCliInfo->hints), 0, sizeof udpCliInfo->hints);
    udpCliInfo->hints.ai_family = AF_UNSPEC;
    udpCliInfo->hints.ai_socktype = SOCK_DGRAM;
    
    if ((rv = getaddrinfo(remote_ip , port, &(udpCliInfo->hints), &(udpCliInfo->servinfo))) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return(ERROR);
    }
    
    for(udpCliInfo->p = udpCliInfo->servinfo;udpCliInfo->p != NULL;udpCliInfo->p = udpCliInfo->p->ai_next)
    {
        if ((udpCliInfo->udpsocksd = socket(udpCliInfo->p->ai_family, udpCliInfo->p->ai_socktype, udpCliInfo->p->ai_protocol)) == -1)
        {
            perror("Preparing UDP: socket");
            continue;
        }
        break;
    }
    
    
    if (udpCliInfo->p == NULL) {
        fprintf(stderr, "Preparing UDP: failed to bind socket\n");
        return(ERROR);
    }
}

#endif
