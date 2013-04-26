#ifndef _GRAPHICFUNCS_H
#define _GRAPHICFUNCS_H

#define SCREEN_WIDTH 800
#define SCREEN_HIGHT 600
#define SCREEN_BPP 32

#define HITBOX_WIDTH 64
#define HITBOX_HIGHT 45

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

void draw_self(struct playerInfo * player, struct cameraInfo * camera);

void draw_other(struct playerInfo * player, struct cameraInfo * camera);

void map(struct playerInfo * player);

void handel_input(SDL_Event * event, int tcpSd );

#endif
