#ifndef _COLLISION_C
#define _COLLISION_C

#include <math.h>
#include <stdio.h>

#include "collision.h"

int check_tank_tank_collision(float * x, float * y, int slot, struct collisionCord otherTanks[])
{
    int i;
    
    for (i = 0; i < MAX_PLAYERS; i++)
    {
        if (slot != i && otherTanks[i].x > -1000)
        {
            if (*x <= (otherTanks[i].x + 64) && (*x + 64) >= otherTanks[i].x)
            {
                if (*y <= otherTanks[i].y + 45 && *y + 45 >= otherTanks[i].y)
                {
                    return 1;
                }
            }
            return 0;
        }
    }
}

void map_edge_collision(float * x, float * y)
{
     if (*y > 1800-HITBOX_HIGHT)
     {
         *y = 1800-HITBOX_HIGHT;
     }
     
     if (*y < 0)
     {
         *y = 0;
     }
     
     if (*x > 2400-HITBOX_WIDTH)
     {
         *x = 2400-HITBOX_WIDTH;
     }
     if (*x < 0)
     {
         *x = 0;
     }
}




#endif
