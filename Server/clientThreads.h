#ifndef _H_CLIENTTHREADS
#define _H_CLIENTTHREADS

#include "clientStruct.h"
#include "internetFuncs.h"
#include "collision.h"

#define MAX_PLAYERS 6
#define TRUE 1
#define FALSE 0
#define RANGE 600

//Keeps the scores.
int redpoints, bluepoints;

//This is the main fuction for the client, it handles the other threads and the input from the client
void *client_handler_function(void *parameters);

//This thread makes all the movement calculations and health.
void *tank_calculations(void *parameters);

//Calculates th bullets movement
void *bullet_movement_calc(void *parameters);

//Broadcast the clients position and bullet to all the other connected clients.
void *broadcast_location();


#endif
