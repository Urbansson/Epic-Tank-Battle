#ifndef _CALCULATIONS_C
#define _CALCULATIONS_C

#include <math.h>

float ballistic_angle(int currentPositionX,int currentPositionY, int destinationPositionX, int destinationPositionY)
{
    return (float) (currentPositionY - destinationPositionY) / (float) (currentPositionX - destinationPositionX);
}



#endif
