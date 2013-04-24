//
//  timers.h
//  
//
//  Created by Theodor Brandt on 2013-04-19.
//
//

#ifndef _PROTOCOL_H
#define _PROTOCOL_H

// Server to Client Move Info
struct stcMoveInfo
{
    int x, y;       //Where the player is
    int player;     //Who the players is
};

struct ctsCommands
{
    int mouseX, mouseY; //Where the mouse is at the screen
    char mouseInput;    //If the mouse is pressed or not
    char keyboardInput; //What key is pressed
};

#endif
