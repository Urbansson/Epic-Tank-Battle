#ifndef _CALCULATIONS_C
#define _CALCULATIONS_C

#include <math.h>

int calculate_angle(int mouseX, int mouseY)
{
    return (((atan2(mouseY-400, mouseX-300)))*(180/M_PI))-90;
}



#endif
