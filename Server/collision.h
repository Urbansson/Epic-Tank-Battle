#ifndef _H_COLLISION
#define _H_COLLISION

#define MAP_WIDTH 2400
#define MAP_HIGHT 1800

#define MAX_PLAYERS 6

#define HITBOX_WIDTH 64
#define HITBOX_HIGHT 45


struct collisionCord
{
    int x;
    int y;
};

//int check_player_collision(int x, int y, int slot, struct collisionCord clientCollision[]);

int check_tank_tank_collision(float * x, float * y, int slot, struct collisionCord otherTanks[]);

void map_edge_collision(float * x, float * y);




#endif
