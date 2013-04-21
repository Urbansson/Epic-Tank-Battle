#ifndef _GRAPHICFUNCS_H
#define _GRAPHICFUNCS_H

#define SCREEN_WIDTH 800
#define SCREEN_HIGHT 600
#define SCREEN_BPP 32

#define HITBOX 30

#define TRUE 1
#define FALSE 0

struct playerInfo
{
    int slot;
    int xCord, yCord;
    int xVel, yVel;
};

int init();

int initGL();

void draw(struct playerInfo * player);

void handel_input(struct playerInfo * player, SDL_Event * event, int tcpSd );

#endif
