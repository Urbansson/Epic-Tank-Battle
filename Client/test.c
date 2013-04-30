#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_opengl.h"
#include <stdio.h>
#include <stdbool.h>


GLuint texture;//=NULL; //this is a handle to our texture object
GLuint texturesOn;//=NULL; //this is a handle to our texture object

GLenum texture_format;//=NULL;
//GLint nofcolors;
GLint  nOfColors;

SDL_Event event;

static GLfloat spin=0.0;
float x=100,y=100;
int LEFT=false;
int RIGHT=false;

/*
int loadImage()
{
    SDL_Surface *surface; // this surface will tell us the details of the image
    if ( (surface = IMG_Load("flower.png")) ) {
        // Check that the image's width is a power of 2
        if ( (surface->w & (surface->w - 1)) != 0 ) {
            printf("warning: image.bmp's width is not a power of 2\n");
        }
        // Also check if the height is a power of 2
        if ( (surface->h & (surface->h - 1)) != 0 ) {
            printf("warning: image.bmp's height is not a power of 2\n");
        }
        
        //get number of channels in the SDL surface
        nofcolors=surface->format->BytesPerPixel;
        
        
        //contains an alpha channel
        if(nofcolors==4)
        {
            if(surface->format->Rmask==0x000000ff)
                texture_format=GL_RGBA;
            else
                texture_format=GL_BGRA;
        }
        
        
        
        else if(nofcolors==3) //no alpha channel
        {
            if(surface->format->Rmask==0x000000ff)
                texture_format=GL_RGB;
            else
                texture_format=GL_BGR;
        }
        
        else
        {
            printf("warning: the image is not truecolor...this will break ");
        }
        // Have OpenGL generate a texture object handle for us
        glGenTextures( 1, &texture );
        // Bind the texture object
        glBindTexture( GL_TEXTURE_2D, texture );
        // Set the texture's stretching properties
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexImage2D( GL_TEXTURE_2D, 0, nofcolors, surface->w, surface->h, 0,
                     texture_format, GL_UNSIGNED_BYTE, surface->pixels );
    }
    else {
        printf("SDL could not load image.bmp: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    // Free the SDL_Surface only if it was successfully created
    if ( surface ) {
        SDL_FreeSurface( surface );
    }
}
*/
int loadImage()
{
    SDL_Surface *surface;

    if ( (surface = IMG_Load("flower.png")) ) {
        
        // Check that the image's width is a power of 2
        if ( (surface->w & (surface->w - 1)) != 0 ) {
            printf("warning: bag.png's width is not a power of 2\n");
        }
        
        // Also check if the height is a power of 2
        if ( (surface->h & (surface->h - 1)) != 0 ) {
            printf("warning: bag.png's height is not a power of 2\n");
        }
        
        // get the number of channels in the SDL surface
        nOfColors = surface->format->BytesPerPixel;
        if (nOfColors == 4)     // contains an alpha channel
        {
            if (surface->format->Rmask == 0x000000ff)
                texture_format = GL_RGBA;
            //else
            //texture_format = GL_BGRA;
        } else if (nOfColors == 3)     // no alpha channel
        {
            if (surface->format->Rmask == 0x000000ff)
                texture_format = GL_RGB;
            //else
            //texture_format = GL_BGR;
        } else {
            printf("warning: the image is not truecolor..  this will probably break\n");
            // this error should not go unhandled
        }
        
        // Have OpenGL generate a texture object handle for us
        glGenTextures( 1, &texture );
        
        // Bind the texture object
        glBindTexture( GL_TEXTURE_2D, texture );
        
        // Set the texture's stretching properties
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        
        // Edit the texture object's image data using the information SDL_Surface gives us
        glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
                     texture_format, GL_UNSIGNED_BYTE, surface->pixels );
    }
    else {
        printf("SDL could not load bag.png: %s\n", SDL_GetError());
        SDL_Quit();
        //return 1;
    }
    
    // Free the SDL_Surface only if it was successfully created
    if ( surface ) { 
        SDL_FreeSurface( surface );
    }


}


void drawImage()
{
    // Clear the screen before drawing
    glClear( GL_COLOR_BUFFER_BIT );
    
    glTranslatef(300.0,200.0,0.0);
    
    glRotatef( spin, 0.0, 0.0, 1.0 );
    
    // Bind the texture to which subsequent calls refer to

    
    
    glBindTexture( GL_TEXTURE_2D, texture );
    glBegin( GL_QUADS );
    
    // Top-left vertex (corner)
    glTexCoord2i( 0, 0 );
    glVertex3f( -64, -64, 0 );
    
    // Bottom-left vertex (corner)
    glTexCoord2i( 1, 0 );
    glVertex3f( 64, -64, 0 );

    
    // Bottom-right vertex (corner)
    glTexCoord2i( 1, 1 );
    glVertex3f( 64, 64, 0 );
    
    
    // Top-right vertex (corner)
    glTexCoord2i( 0, 1 );
    glVertex3f( -64, 64, 0 );
    
    
    glEnd();
    glLoadIdentity();
    SDL_GL_SwapBuffers();
}

int init()
{
    SDL_Surface *screen;
    // Slightly different SDL initialization
    if ( SDL_Init(SDL_INIT_VIDEO) != 0 ) {
        printf("Unable to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    screen = SDL_SetVideoMode( 640, 480, 16, SDL_OPENGL);
    if ( !screen ) {
        printf("Unable to set video mode: %s\n", SDL_GetError());
        return 1;
    }
}
void init_GL()
{
    // Set the OpenGL state after creating the context with SDL_SetVideoMode
    glClearColor( 0, 0, 0, 0 );
    glEnable( GL_TEXTURE_2D ); // Need this to display a texture
    glViewport( 0, 0, 640, 480 );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0, 640, 480, 0, 0, 1 );
    glMatrixMode( GL_MODELVIEW );
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glLoadIdentity();
}
void clean_up()
{
    glDeleteTextures( 1, &texture );
    SDL_Quit();
}
int main(int argc, char *argv[])
{
    //Make sure the program waits for a quit
    bool quit = false;
    init();
    init_GL();
    loadImage();
    drawImage();
    while( quit == false )
    {
        if( SDL_PollEvent( &event ) )
        {
            //If a key was pressed
            if( event.type == SDL_KEYDOWN )
            {
                //Adjust the velocity
                switch( event.key.keysym.sym )
                {
                    case SDLK_LEFT:
                        LEFT=true;
                        break;
                    case SDLK_RIGHT:
                        RIGHT=true;
                        break;
                    case SDLK_ESCAPE:
                        quit=true;
                        break;
                }
            }
            //If a key was released
            else if( event.type == SDL_KEYUP )
            {
                //Adjust the velocity
                switch( event.key.keysym.sym )
                {
                    case SDLK_LEFT:
                        LEFT=false;
                        break;
                    case SDLK_RIGHT:
                        RIGHT=false;
                        break;
                }
            }
        }
        if(LEFT==true){
            spin=spin+2;
            if(spin>360.0)
                spin=spin-360.0;
            drawImage();
        }
        else if(RIGHT==true){
            spin=spin-2;
            if(spin>360.0)
                spin=spin-360.0;
            drawImage();
        }
    }
    clean_up();
    return 0;
}