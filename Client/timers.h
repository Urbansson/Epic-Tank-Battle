//
//  timers.h
//  
//
//  Created by Theodor Brandt on 2013-04-19.
//
//

#ifndef _TIMERS_H
#define _TIMERS_H

struct timerInfo
{
    int ticks;
    int running;
};


void timer_start(struct timerInfo * timer);

int timer_get_ticks(struct timerInfo * timer);


#endif
