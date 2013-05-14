#ifndef _COLLISION_C
#define _COLLISION_C

#include <math.h>
#include <stdio.h>

#include "collision.h"

void * bullet_hit_thread(void *parameters)
{
    int i, k;
    struct client *clientInfo = (struct client *)parameters;
    
    while (TRUE)
    {
        for (i = 0; i < MAX_PLAYERS; i++)
        {
            for (k = 0; k < MAX_PLAYERS; k++)
            {
                if (clientInfo[i].mySlot > -1  && clientInfo[k].mySlot > -1 && i != k && clientInfo[i].bulletHit == 0)
                {
                    if (clientInfo[i].bulletX <= clientInfo[k].xLocation + 64 && clientInfo[i].bulletX + 10 >= clientInfo[k].xLocation)
                    {
                        if (clientInfo[i].bulletY <= clientInfo[k].yLocation + 45 && clientInfo[i].bulletY + 10 >= clientInfo[k].yLocation)
                        {
                            clientInfo[i].bulletHit = 1;
                            clientInfo[k].bulletHitMe = 1;
                        }
                    }
                }
            }
        }
    }
}


int check_worldmap_collision(int x, int y)
{
    int tempPos;
    
    for (tempPos = x; tempPos <= x + HITBOX_WIDTH; tempPos++)
    {
        if (mapArray[tempPos][y] == '1')
        {
            //printf("Krock\n");
            return 1;
        }
        if (mapArray[tempPos][y+HITBOX_HIGHT] == '1')
        {
            //printf("Krock\n");
            return 1;
        }
    }
    for (tempPos = y; tempPos <= y + HITBOX_HIGHT; tempPos++)
    {
        if (mapArray[x][tempPos] == '1')
        {
            //printf("Krock\n");
            return 1;
        }
        if (mapArray[x+HITBOX_WIDTH][tempPos] == '1')
        {
            //printf("Krock\n");
            return 1;
        }
    }
    /*
    printf("MapArray är: %c\n", mapArray[x][y]);
    
    if (mapArray[x][y] == '1')
    {
        //printf("Krock\n");
        return 1;
    }
    if (mapArray[x+HITBOX_WIDTH][y] == '1')
    {
        //printf("Krock\n");
        return 1;
    }
    if (mapArray[x][y+HITBOX_HIGHT] == '1')
    {
        //printf("Krock\n");
        return 1;
    }
    if (mapArray[x+HITBOX_WIDTH][y+HITBOX_HIGHT] == '1')
    {
        //printf("Krock\n");
        return 1;
    }
    
     */
    //MÅSTE VARA FORLOOP FÖR ALLA VÄRDEN
    return 0;
}

int check_tank_tank_collision(float * x, float * y, int slot, struct collisionCord otherTanks[])
{
    int i;
    
    for (i = 0; i < MAX_PLAYERS; i++)
    {
        if (slot != i && otherTanks[i].x > -1000)
        {
            if (*x <= (otherTanks[i].x + HITBOX_WIDTH) && (*x + HITBOX_WIDTH) >= otherTanks[i].x)
            {
                if (*y <= otherTanks[i].y + HITBOX_HIGHT && *y + HITBOX_HIGHT >= otherTanks[i].y)
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
