#ifndef _TIMERS_C
#define _TIMERS_C

#include <stdio.h>
#include "timers.h" 

void timer_init (struct timerInfo * timer)
{
    timer->ticks=0;
    timer->running=0;
};

void timer_stop(struct timerInfo * timer)
{
    //Stop the timer
    timer->running = 0;
};

void timer_start(struct timerInfo * timer)
{
    timer->running = 1;
    timer->ticks = SDL_GetTicks();
}

int timer_get_ticks(struct timerInfo * timer)
{
    //If the timer is running
    if( timer->running == 1 )
    {
        //Return the current time minus the start time
        //printf("Change is %d\n", SDL_GetTicks() - timer->ticks);
        return SDL_GetTicks() - timer->ticks;
        
    }
    //If the timer isn't running
    return 0;
}

#endif