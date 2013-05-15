#ifndef _H_DEBUGGER
#define _H_DEBUGGER

#include "clientStruct.h"

#define MAX_PLAYERS 6

#define HITBOX_WIDTH 64
#define HITBOX_HIGHT 45
#define RANGE 600

#define TRUE 1
#define FALSE 0

void server_debugger_print(struct client clientInfo, int place);

void * debeugger_print_thread(void *parameters);

#endif
