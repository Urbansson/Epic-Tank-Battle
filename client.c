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

#include <unistd.h> /* close() */


#define LOCAL_PORT 6000
#define MAX_MSG 128

int main(void)
{
    int fd, bc;
    struct sockaddr_in cliAddr, servAddr;
    socklen_t servLen;
    char msg[MAX_MSG];
    
    /* socket creation */
    fd=socket(AF_INET, SOCK_DGRAM, 0);
    if(fd<0) {
        printf("cannot open socket \n");
        return(-1);
    }
    
    cliAddr.sin_family = AF_INET;
    cliAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    cliAddr.sin_port = htons(LOCAL_PORT);
    
    bc = bind (fd, (struct sockaddr *) &cliAddr,sizeof(cliAddr));
    if(bc<0) {
        printf("cannot bind port number %d \n", LOCAL_PORT);
        return(-1);
    }
    
    printf("waiting for data on port UDP %u\n", LOCAL_PORT);
    
    while (1)
    {
        memset(msg, 0, MAX_MSG);

        
    
        servLen = sizeof(servAddr);
        recvfrom(fd, msg, MAX_MSG, 0, (struct sockaddr *) &servAddr, &servLen);
    
        printf("Message from server: %s", msg);
    
    }
    
    return 0;
}