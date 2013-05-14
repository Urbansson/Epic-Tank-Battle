#ifndef _H_INTERNETFUNCS
#define _H_INTERNETFUNCS


#define PORT 5000       // the port users will be connecting to
#define BACKLOG 10     // how many pending connections queue will hold

#define ERROR -1
#define SUCCESS 0

#define MAX_PLAYERS 6

// Struct that stores the information that is needed to send data over udp.
struct udp_info {
    int udpsocksd;
    struct addrinfo hints, *servinfo, *p;
};

//It needs to be global so all threads that send information can reach it.
struct udp_info udpCliInfo[MAX_PLAYERS];


//Initiates the tcp conection
int tcp_init(void);

//Initiates the udp conection
int udp_init(char * remote_ip, char * port, struct udp_info * udpCliInfo);

#endif
