#ifndef _GRAPHICFUNCS_H
#define _GRAPHICFUNCS_H

#define SCREEN_WIDTH 800
#define SCREEN_HIGHT 600
#define SCREEN_BPP 32

#define TRUE 1
#define FALSE 0

int init();

int initGL();

GLuint load_image(char *fileName);

void handel_input(SDL_Event * event, int tcpSd );

#endif
