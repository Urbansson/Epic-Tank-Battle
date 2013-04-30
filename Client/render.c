#ifndef _RENDER_C
#define _RENDER_C

#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"

#include "render.h"

void draw_self(struct playerInfo * player, struct cameraInfo * camera)
{
    
    glTranslatef( 400-HITBOX_WIDTH/2, 300-HITBOX_HIGHT/2, 0);
    
    glBegin(GL_QUADS);
    
    glVertex2f( 0,      0      );
    glVertex2f( HITBOX_WIDTH, 0      );
    glVertex2f( HITBOX_WIDTH, HITBOX_HIGHT );
    glVertex2f( 0,      HITBOX_HIGHT );
    
    glEnd();
    
    //Resets
    glLoadIdentity();
}

void draw_other(struct playerInfo * player, struct cameraInfo * camera)
{
    
    glTranslatef( player->xCord+ camera->xCord+ 400-HITBOX_WIDTH/2, player->yCord + camera->yCord+300-HITBOX_HIGHT/2, 0);
    
    glBegin(GL_QUADS);
    
    glColor3f(1,0,0);
    
    glVertex2f( 0,      0      );
    glVertex2f( HITBOX_WIDTH, 0      );
    glVertex2f( HITBOX_WIDTH, HITBOX_HIGHT );
    glVertex2f( 0,      HITBOX_HIGHT );
    
    glEnd();
    
    //Resets
    glLoadIdentity();
}


void draw_bullet(struct playerInfo * player, struct cameraInfo * camera)
{
    
    glTranslatef( player->bulletX + camera->xCord+ 400-HITBOX_WIDTH/2, player->bulletY + camera->yCord+300-HITBOX_HIGHT/2, 0);
    
    glBegin(GL_QUADS);
        
    glVertex2f( 0,      0      );
    glVertex2f( 15, 0      );
    glVertex2f( 15, 10 );
    glVertex2f( 0,      10 );
    
    glEnd();
    
    //Resets
    glLoadIdentity();
}


void map(struct playerInfo * player)
{
    
    glTranslatef( -1*player->xCord+400-HITBOX_WIDTH/2, -1*player->yCord+300-HITBOX_HIGHT/2, 0);
    
    glBegin(GL_QUADS);
    
    glColor3f(0,1,0);
    
    glVertex2f( 0,         0      );
    glVertex2f( MAP_WIDTH, 0      );
    glVertex2f( MAP_WIDTH, MAP_HIGHT );
    glVertex2f( 0,         MAP_HIGHT );
    
    glEnd();
    
    //resets
    glLoadIdentity();
    
}

void draw_image(GLuint * texture)
{
    // Clear the screen before drawing
    glClear( GL_COLOR_BUFFER_BIT );
    
    glTranslatef(300,200,0.0);
    
    //glRotatef( spin, 0.0, 0.0, 1.0 );
    
    // Bind the texture to which subsequent calls refer to

    
    glBindTexture( GL_TEXTURE_2D, *texture );
    glBegin( GL_QUADS );
    
    // Top-left vertex (corner)
    glTexCoord2i( 0, 0 );
    glVertex3f( -64, -64, 0 );
    
    // Bottom-left vertex (corner)
    glTexCoord2i( 1, 0 );
    glVertex3f( 64, -64, 0 );
    
    
    // Bottom-right vertex (corner)
    glTexCoord2i( 1, 1 );
    glVertex2f( 64, 64 );
    
    
    // Top-right vertex (corner)
    glTexCoord2i( 0, 1 );
    glVertex3f( -64, 64, 0 );
    
    
    glEnd();
    glLoadIdentity();
    //SDL_GL_SwapBuffers();
}

void draw_map(GLuint * texture, struct playerInfo * player)
{
    glTranslatef( -1*player->xCord+100-HITBOX_WIDTH/2, -1*player->yCord+100-HITBOX_HIGHT/2, 0);
    
    // Clear the screen before drawing
    glClear( GL_COLOR_BUFFER_BIT );
    
    glTranslatef(300,200,0.0);
    
    //glRotatef( spin, 0.0, 0.0, 1.0 );
    
    // Bind the texture to which subsequent calls refer to
    
    
    glBindTexture( GL_TEXTURE_2D, *texture );
    glBegin( GL_QUADS );
    
    // Top-left vertex (corner)
    glTexCoord2i( 0, 0 );
    glVertex2f( 0,         0      );
    
    // Bottom-left vertex (corner)
    glTexCoord2i( 1, 0 );
    glVertex2f( MAP_WIDTH, 0      );
    
    
    // Bottom-right vertex (corner)
    glTexCoord2i( 1, 1 );
    glVertex2f( MAP_WIDTH, MAP_HIGHT );
    
    
    // Top-right vertex (corner)
    glTexCoord2i( 0, 1 );
    glVertex2f( 0,         MAP_HIGHT );
    
    
    glEnd();
    glLoadIdentity();
    //SDL_GL_SwapBuffers();
}


#endif