#ifndef _GRAPHICFUNCS_H
#define _GRAPHICFUNCS_H

#define TRUE 1
#define FALSE 0

#define SCREEN_WIDTH 800
#define SCREEN_HIGHT 600

#define HITBOX_WIDTH 64
#define HITBOX_HIGHT 45

struct playerInfo
{
    int slot;
    int xCord, yCord;
    int xVel, yVel;
    int mouseX, mouseY;
    int fire;
    int bulletX, bulletY;
    int tankAngle;
    int cannonAngle;
    
    int team;
    int healthPoints;
    int dead;
};

struct cameraInfo
{
    int xCord;
    int yCord;
};


void handel_input(SDL_Event * event, int tcpSd );

SDL_Surface *load_image( char filename[] );

void draw_map( int x, int y, SDL_Surface* source, SDL_Surface* destination );

void draw_bullet(struct playerInfo * player, struct cameraInfo * camera, SDL_Surface* source, SDL_Surface* destination);

void draw_UI( int x, int y, SDL_Surface* text, SDL_Surface* screen);



#endif
