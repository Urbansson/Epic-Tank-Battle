// To try it out:
// telnet 127.0.0.1 5000
//

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h> /* close */

#define SUCCESS 0
#define ERROR   1

#define SERVER_PORT 5000

int main(void)
{
    int sd, newSd;  // listen on sock_fd, new connection on new_fd
    struct sockaddr_in cliAddr, servAddr;
    socklen_t cliLen;

    /* create socket */
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if(sd<0) {
        perror("cannot open socket ");
        return ERROR;
    }
    
    /* bind server port */
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(SERVER_PORT);

    
    if(bind(sd, (struct sockaddr *) &servAddr, sizeof(servAddr))<0) {
        perror("cannot bind port ");
        return ERROR;
    }
    
    listen(sd,5);

    printf("server: waiting for connections...\n");

    
    while (1)
    {
        cliLen = sizeof(cliAddr);
        newSd = accept(sd, (struct sockaddr *) &cliAddr, &cliLen);
        if(newSd<0) {
            perror("cannot accept connection ");
            return ERROR;
        }
        send(newSd, "Hello World!\n", 14, 0);
        
        close(newSd);
    }
    
    
    return 0;
}