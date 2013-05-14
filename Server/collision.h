#ifndef _H_COLLISION
#define _H_COLLISION

#define MAP_WIDTH 2400
#define MAP_HIGHT 1800

#define MAX_PLAYERS 6

#define HITBOX_WIDTH 64
#define HITBOX_HIGHT 45

#define TRUE 1
#define FALSE 0

#include "clientStruct.h"

//Struct that saves x y cords where the playes is. (Uggly way to solve the problem)
struct collisionCord
{
    int x;
    int y;
};

//global so all the threads can reach it
struct collisionCord otherTanks[MAX_PLAYERS];

//the map array that stores all the collision objects on the map
char mapArray[2400][1800];

//Thread that checks if any bullet hits any tank
void * bullet_hit_thread(void *parameters);

//Uses the map array to check for collisions
int check_worldmap_collision(int x, int y);

//Checks for tank tank collision so two clients can drive over eachother
int check_tank_tank_collision(float * x, float * y, int slot, struct collisionCord otherTanks[]);

//Limits the client to the map.
void map_edge_collision(float * x, float * y);




#endif
