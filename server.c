#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
#include <errno.h>
#include <string.h>
//#include <sys/types.h>
#include <sys/socket.h>
//#include <netinet/in.h>
#include <netdb.h>
//#include <arpa/inet.h>
//#include <sys/wait.h>
//#include <signal.h>

#define BUFLEN 512


#define PORT "5000"  // the port users will be connecting to


#define BACKLOG 10     // how many pending connections queue will hold

struct udp_info {
    int udpsockfd;
    struct addrinfo hints, *servinfo, *p;
} very_ugly_array;


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void prepare_udp_stuff_in_very_ugly_array(char * remote_ip, char * port)
{
    int rv;
    
    memset(&(very_ugly_array.hints), 0, sizeof very_ugly_array.hints);
    very_ugly_array.hints.ai_family = AF_UNSPEC;
    very_ugly_array.hints.ai_socktype = SOCK_DGRAM;
    
    if ((rv = getaddrinfo(  remote_ip , port, &(very_ugly_array.hints), &(very_ugly_array.servinfo))) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(EXIT_FAILURE);
    }
    
    // loop through all the results and make a socket
    for(very_ugly_array.p = very_ugly_array.servinfo;
        very_ugly_array.p != NULL;
        very_ugly_array.p = very_ugly_array.p->ai_next) {
        if ((very_ugly_array.udpsockfd = socket(very_ugly_array.p->ai_family, very_ugly_array.p->ai_socktype,
                                                       very_ugly_array.p->ai_protocol)) == -1) {
            perror("Preparing UDP: socket");
            continue;
        }
        break;
    }
    
    if (very_ugly_array.p == NULL) {
        fprintf(stderr, "Preparing UDP: failed to bind socket\n");
        exit(EXIT_FAILURE);
    }
    
}


int main(void)
{
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;
    char buffer[7]= "Hejsan!";
    
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP
    
    
    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }
    
    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }
        
        /*
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                       sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }
        */
        
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
    
    freeaddrinfo(servinfo); // all done with this structure
    
    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }
    
    printf("server: waiting for connections...\n");
        
    while(1)
    {  // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }
        
        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);

        prepare_udp_stuff_in_very_ugly_array(s, "6000");

        
        send(new_fd, "Hello, world!\n", 14, 0);
        
        
        sendto(very_ugly_array.udpsockfd, buffer, strlen(buffer)+1, 0, very_ugly_array.p->ai_addr, very_ugly_array.p->ai_addrlen);
        

        
        printf("server: got connection from %s\n", s);
        
        close(new_fd);  // parent doesn't need this
        
        
        }
    
    return 0;
}
