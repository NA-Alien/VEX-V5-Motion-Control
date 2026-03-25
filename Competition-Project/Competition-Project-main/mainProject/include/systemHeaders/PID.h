#ifndef proporid
#define proporid
#include "vex.h"

class PID
{
public:
    float error = 0;
    float kP = 0;
    float kD = 0;
    float kI = 0;
    float totalError = 0;
    float integralThresh = 0;
    float timeout = 0;
    float preverror = 0;
    float output = 0;
    float driveDerivative = 0;
    float time_spent_running = 0;
    float time_spent_settled = 0;
    float settle_time = 0;
    float settle_error = 0;

    PID(float error, float kp, float ki, float kd, float starti);

    PID(float error, float kP, float kD, float kI, float integralThresh, float settle_error, float settle_time, float timeout);

    float calculate(float error);
    bool is_settled();
};

#endif