#ifndef _GRAPHICFUNCS_H
#define _GRAPHICFUNCS_H

#define SCREEN_WIDTH 800
#define SCREEN_HIGHT 600
#define SCREEN_BPP 32

#define HITBOX 30

#define MAP_WIDTH 2400
#define MAP_HIGHT 1800

#define TRUE 1
#define FALSE 0

struct playerInfo
{
    int slot;
    int xCord, yCord;
    int xVel, yVel;
    int mouseX, mouseY;
};

struct cameraInfo
{
    int xCord;
    int yCord;
};

int init();

int initGL();

void draw(struct playerInfo * player, struct cameraInfo * camera);

void map(struct cameraInfo * camera);

void handel_input(SDL_Event * event, int tcpSd );

#endif
