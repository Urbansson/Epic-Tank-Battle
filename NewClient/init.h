#ifndef _INIT_H
#define _INIT_H

#define SCREEN_WIDTH 800
#define SCREEN_HIGHT 600
#define SCREEN_BPP 32

#define TRUE 1
#define FALSE 0

SDL_Surface *init_sdl();

int init_udp_tcp(int * udpSd, int * tcpSd, char *serverIp, char *serverPort);

#endif
