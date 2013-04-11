#ifndef _H_INTERNETFUNCS
#define _H_INTERNETFUNCS


#define PORT 5000  // the port users will be connecting to
#define BACKLOG 10     // how many pending connections queue will hold

#define ERROR -1
#define SUCCESS 0

/*
struct tcp_info
{
    int sd;
    struct sockaddr_in cliAddr;
    socklen_t cliLen;
};
*/
 
struct udp_info {
    int udpsocksd;
    struct addrinfo hints, *servinfo, *p;
};

int tcp_init(void);

int udp_init(char * remote_ip, char * port, struct udp_info * udpCliInfo);

#endif
