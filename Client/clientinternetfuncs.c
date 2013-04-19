#ifndef _CLIENTINTERNETFUNCS_C
#define _CLIENTINTERNETFUNCS_C

#include <stdio.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <string.h> //memset 

#include "clientinternetfuncs.h"


int udp_init()
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

int tcp_init(char *serverIp, char *serverPort)
{
    
    int tcpSd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[100];
    
    memset(&hints, 0, sizeof (hints));
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


#endif
