// To try it out:
// telnet 127.0.0.1 5000
//

#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
//#include <netinet/in.h>
#include <netdb.h>
//#include <arpa/inet.h>
//#include <sys/wait.h>
//#include <signal.h>

#define SUCCESS 0
#define ERROR   1

#define PORT "5000"
#define BACKLOG 10

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


int main(void)
{
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo;   // servers's information
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    char s[20];
    
    //struct sigaction sa;
    //int yes=1;
    //char s[INET6_ADDRSTRLEN];
    //int rv;
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP
    
    
    /*
    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }
    */
    getaddrinfo(NULL, PORT, &hints, &servinfo);

    
    sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    
    bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen);

    
    /*
    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }
        
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                       sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }
        
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }
        
        break;
    }
    
    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        return 2;
    }
    
     */
    
    freeaddrinfo(servinfo); // all done with this structure
    
    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }
    
    printf("server: waiting for connections...\n");
    
    while (1)
    {
        sin_size = sizeof(sin_size);
        new_fd = accept(sockfd, (struct sockaddr *) &sin_size, &sin_size);
        if(new_fd<0) {
            perror("cannot accept connection ");
            return ERROR;
        }
        send(new_fd, "Hello World!\n", 14, 0);
        
        
        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
        printf("server: got connection from %s \n", s);
        
        close(new_fd);
    }
    
    /*
    int sd, newSd;  // listen on sock_fd, new connection on new_fd
    struct sockaddr_in cliAddr, servAddr;
    socklen_t cliLen;

    /* create socket */   /*
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if(sd<0) {
        perror("cannot open socket ");
        return ERROR;
    }
    
    /* bind server port *//*
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
    
    */
    
    
    return 0;
}