#ifndef UTIL
#define UTIL
#include "vex.h"

void resetDriveEncoders();

float avgEncoderMotor();

float avgEncoderMotorLeft();

float avgEncoderMotorRight();

float to_rad(float angle_deg);

float to_deg(float angle_rad);

float reduce_negative_180_to_180(float angle);

float reduce_0_to_360(float angle);

float clamp(float input, float min, float max);

bool is_line_settled(float desired_X, float desired_Y, float desired_angle_deg, float current_X, float current_Y);

float left_voltage_scaling(float drive_output, float heading_output);

float right_voltage_scaling(float drive_output, float heading_output);

float clamp_min_voltage(float drive_output, float drive_min_voltage);

float reduce_negative_90_to_90(float angle);

#endif