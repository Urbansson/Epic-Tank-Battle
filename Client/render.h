//
//  timers.h
//  
//
//  Created by Theodor Brandt on 2013-04-19.
//
//

#ifndef _RENDER_H
#define _RENDER_H

#define HITBOX_WIDTH 64
#define HITBOX_HIGHT 45

#define MAP_WIDTH 2400
#define MAP_HIGHT 1800

struct playerInfo
{
    int slot;
    int xCord, yCord;
    int xVel, yVel;
    int mouseX, mouseY;
    int fire;
    int bulletX, bulletY;
};

struct cameraInfo
{
    int xCord;
    int yCord;
};

void draw_self(struct playerInfo * player, struct cameraInfo * camera);

void draw_other(struct playerInfo * player, struct cameraInfo * camera);

void draw_bullet(struct playerInfo * player, struct cameraInfo * camera);

void map(struct playerInfo * player);

void draw_image(GLuint * texture);

void draw_map(GLuint * texture, struct playerInfo * player);




#endif
