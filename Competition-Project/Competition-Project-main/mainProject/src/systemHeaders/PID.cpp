#include "vex.h"

PID::PID(float error, float kP, float kD, float kI, float integralThresh) : error(error),
                                                                            kP(kP),
                                                                            kD(kD),
                                                                            kI(kI),
                                                                            integralThresh(integralThresh)

{
}

PID::PID(float error, float kP, float kD, float kI, float integralThresh, float settle_error, float settle_time, float timeout) : error(error),
                                                                                                                                  kP(kP),
                                                                                                                                  kD(kD),
                                                                                                                                  kI(kI),
                                                                                                                                  integralThresh(integralThresh),
                                                                                                                                  settle_error(settle_error),
                                                                                                                                  settle_time(settle_time),
                                                                                                                                  timeout(timeout)
{
}

float PID::calculate(float error)

{

  if (fabs(error) < integralThresh)
  {
    totalError += error;
  }
  else
  {
    totalError = 0;
  }

  if ((error > 0 && preverror < 0) || (error < 0 && preverror > 0))
  {
    totalError = 0;
  }

  output = kP * error + kI * totalError + kD * (error - preverror);

  preverror = error;

  if (fabs(error) < settle_error)
  {
    time_spent_settled += 10;
  }
  else
  {
    time_spent_settled = 0;
  }

  time_spent_running += 10;

  return output;
}

bool PID::is_settled()
{
  if (time_spent_running > timeout && timeout != 0)
  {
    return (true);
  }
  if (time_spent_settled > settle_time)
  {
    return (true);
  }
  return (false);
}
