//
//  timers.h
//  
//
//  Created by Theodor Brandt on 2013-04-19.
//
//

#ifndef _PROTOCOL_H
#define _PROTOCOL_H

// Server to Client Info sent via UDP 
struct stcInfo
{
    char destination[20];   //The destination ip for the information
    int x, y;               //Where the player is
    int player;             //Who the players is
    int mouseX, mouseY;     //where the players mouse is pointing
    int fire;               //if the player is firing
};

//Client to server commands on how to move the controlled unit sent via tcp
struct ctsCommands
{
    int mouseX, mouseY;     //Where the mouse is at the screen
    char mouseInput;        //If the mouse is pressed or not
    char keyboardInput;     //What key is pressed
};

#endif
