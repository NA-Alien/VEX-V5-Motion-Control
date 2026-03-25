#include "vex.h"
void resetDriveEncoders()
{
  TR.resetPosition();
  TL.resetPosition();
  BR.resetPosition();
  BL.resetPosition();
  MR.resetPosition();
  ML.resetPosition();
}

float avgEncoderMotor()
{
  return (TR.position(degrees) + TL.position(degrees) +
          MR.position(degrees) + ML.position(degrees) +
          BR.position(degrees) + BL.position(degrees)) /
         6;
}

float avgEncoderMotorLeft()
{
  return (TL.position(degrees) +
          ML.position(degrees) +
          BL.position(degrees)) /
         3;
}

float avgEncoderMotorRight()
{
  return (TR.position(degrees) +
          MR.position(degrees) +
          BR.position(degrees)) /
         3;
}

float to_rad(float angle_deg)
{
  return (angle_deg / (180.0 / M_PI));
}

float to_deg(float angle_rad)
{
  return (angle_rad * (180.0 / M_PI));
}

float reduce_negative_180_to_180(float angle)
{
  while (!(angle >= -180 && angle < 180))
  {
    if (angle < -180)
    {
      angle += 360;
    }
    if (angle >= 180)
    {
      angle -= 360;
    }
  }
  return (angle);
}

float reduce_0_to_360(float angle)
{
  while (!(angle >= 0 && angle < 360))
  {
    if (angle < 0)
    {
      angle += 360;
    }
    if (angle >= 360)
    {
      angle -= 360;
    }
  }
  return (angle);
}

float clamp(float input, float min, float max)
{
  if (input > max)
  {
    return (max);
  }
  if (input < min)
  {
    return (min);
  }
  return (input);
}

bool is_line_settled(float desired_X, float desired_Y, float desired_angle_deg, float current_X, float current_Y)
{
  return ((desired_Y - current_Y) * cos(to_rad(desired_angle_deg)) <= -(desired_X - current_X) * sin(to_rad(desired_angle_deg)));
}

/**
 * Voltage scaling to keep from applying more than 12 volts to either side of the drive.
 * Divides both drive and heading output proportionally to get a similar result to the
 * desired one.
 *
 * @param drive_output The forward output of the drive.
 * @param heading_output The angular output of the drive.
 * @return The scaled voltage for the left side of the robot.
 */

float left_voltage_scaling(float drive_output, float heading_output)
{
  float ratio = std::max(std::fabs(drive_output + heading_output), std::fabs(drive_output - heading_output)) / 12.0;
  if (ratio > 1)
  {
    return (drive_output + heading_output) / ratio;
  }
  return drive_output + heading_output;
}

/**
 * Voltage scaling to keep from applying more than 12 volts to either side of the drive.
 * Divides both drive and heading output proportionally to get a similar result to the
 * desired one.
 *
 * @param drive_output The forward output of the drive.
 * @param heading_output The angular output of the drive.
 * @return The scaled voltage for the right side of the robot.
 */

float right_voltage_scaling(float drive_output, float heading_output)
{
  float ratio = std::max(std::fabs(drive_output + heading_output), std::fabs(drive_output - heading_output)) / 12.0;
  if (ratio > 1)
  {
    return (drive_output - heading_output) / ratio;
  }
  return drive_output - heading_output;
}

/**
 * Brings an output up to the minimum voltage if it's too slow.
 * Used for minimum voltage calculations for movement chaining.
 * Has no effect on 0 voltage output, because how do we know
 * which way it's supposed to be going?
 *
 * @param drive_output The forward output of the drive.
 * @param drive_min_voltage The minimum output of the drive.
 * @return The voltage with the minimum applied.
 */

float clamp_min_voltage(float drive_output, float drive_min_voltage)
{
  if (drive_output < 0 && drive_output > -drive_min_voltage)
  {
    return -drive_min_voltage;
  }
  if (drive_output > 0 && drive_output < drive_min_voltage)
  {
    return drive_min_voltage;
  }
  return drive_output;
}

float reduce_negative_90_to_90(float angle)
{
  while (!(angle >= -90 && angle < 90))
  {
    if (angle < -90)
    {
      angle += 180;
    }
    if (angle >= 90)
    {
      angle -= 180;
    }
  }
  return (angle);
}
