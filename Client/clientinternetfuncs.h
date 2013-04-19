#ifndef _CLIENTINTERNETFUNCS_H
#define _CLIENTINTERNETFUNCS_H

#define LOCAL_PORT 6000

int udp_init();

int tcp_init(char *serverIp, char *serverPort);


#endif
