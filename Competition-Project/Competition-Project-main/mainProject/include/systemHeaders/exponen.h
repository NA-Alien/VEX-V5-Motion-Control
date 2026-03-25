#ifndef EXPONENTIAL
#define EXPONENTIAL
#include "vex.h"

class EXPO
{
public:
    float targetVelocity = 0;
    float curve = 0;

    EXPO(float targetVelocity, float curve);

    float velocityExpo(float targetVelocity, float curve);
};

void driveMove(float targetVelocityLeft, float targetVelocityRight, float curve);

#endif