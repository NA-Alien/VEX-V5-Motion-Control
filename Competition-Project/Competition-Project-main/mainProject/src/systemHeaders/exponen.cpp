#include "vex.h"

EXPO::EXPO(float targetVelocity, float curve) :

                                                targetVelocity(targetVelocity),
                                                curve(curve)

{
}

float EXPO::velocityExpo(float targetVelocity, float curve)
{

    float output;

    // return output = (pow(M_E, -(curve/10)) + pow(M_E, ((abs(targetVelocity) - 127)) / 10 ) * (pow(1-M_E, -(curve/10))))(targetVelocity);
    return output = (((pow(M_E, -(curve / 10)) + pow(M_E, (abs(targetVelocity) - 127) / 10) * (1 - pow(M_E, -(curve / 10)))) * targetVelocity) / 127) * 12;
    printf("Output: %f", output);
}

void driveMove(float targetVelocityLeft, float targetVelocityRight, float curve)
{
    EXPO driveExpoLeft(targetVelocityLeft, curve);
    EXPO driveExpoRight(targetVelocityRight, curve);

    float outputLeft = driveExpoLeft.velocityExpo(targetVelocityLeft, curve); // After function
    float outputRight = driveExpoRight.velocityExpo(targetVelocityRight, curve);

    // Brain.Screen.printAt(1, 10, "Left Line Output: %f", outputLeft);
    // Brain.Screen.printAt(1, 30, "Right Line Output: %f", outputRight);

    Left.spin(fwd, outputLeft, volt);
    Right.spin(fwd, outputRight, volt);
}