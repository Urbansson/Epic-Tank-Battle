#ifndef _H_CLIENTTHREADS
#define _H_CLIENTTHREADS

#include "clientStruct.h"
#include "internetFuncs.h"
#include "collision.h"

#define MAX_PLAYERS 6
#define TRUE 1
#define FALSE 0
#define RANGE 600

int redpoints, bluepoints;

struct collisionCord otherTanks[MAX_PLAYERS];

//char mapArray[2400][1800];

struct udp_info udpCliInfo[MAX_PLAYERS];

void *client_handler_function(void *parameters);

void *tank_calculations(void *parameters);

void *bullet_movement_calc(void *parameters);

void *broadcast_location();


#endif
