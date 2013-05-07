#ifndef _CLIENTINTERNETFUNCS_H
#define _CLIENTINTERNETFUNCS_H

#define LOCAL_PORT 6000

#include "protocol.h"

struct udpData
{
    int udpSd;
    char serverIp[16];
};

int udp_init();

int tcp_init(char *serverIp, char *serverPort);

void format_send_ctsCommand(struct ctsCommands *commands, int tcpSd);


#endif
