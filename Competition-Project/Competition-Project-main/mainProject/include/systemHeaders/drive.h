#ifndef DRIVE
#define DRIVE
#include "vex.h"

using namespace vex;

extern brain Brain;
extern controller master;

extern motor TR;
extern motor TL;
extern motor MR;
extern motor ML;
extern motor BR;
extern motor BL;
extern motor intake;
extern motor lift;

extern motor_group Left;
extern motor_group Right;
extern drivetrain driving;
extern inertial imu;
extern pneumatics clamp1;
extern pneumatics doinker;
extern rotation liftAngle;
extern rotation sideWaysTracker;
extern optical op;

void driveFor(float distance, float drivekP, float drivekI, float drivekD, float driveIntegralTresh, float settle_drive_error, float drive_settle_time, float drive_timeout, float minVoltage, float maxVoltage);

void turnFor(float angle, float turnkP, float turnkD, float turnkI, float integralThresh, float turn_settle_error, float turn_settle_time, float turntimeout);

void rightArcTurn(float angle, float arckP, float arckD, float arckI, float arcIntegralThresh, float arc_settle_error, float arc_settle_time, float arctimeout);

void leftArcTurn(float angle, float arckP, float arckD, float arckI, float arcIntegralThresh, float arc_settle_error, float arc_settle_time, float arctimeout);

void antiDriftD(float distance, float heading, float drivekP, float drivekD, float drivekI, float turnkP, float turnkD, float turnkI, float driveintegralThresh, float driftIntegralThresh, float drive_settle_error, float drive_settle_time, float drive_timeout);

void liftTo(float angle, float liftkP, float liftkD, float liftkI, float integralThresh, float lift_settle_error, float lift_settle_time, float lifttimeout);

void trackPosition();

int positionTask();

void setCoords(float X_position, float Y_position, float orientation_deg);

float positionX();

float positionY();

void odom_test();

void translate(float X_position, float Y_position, float drive_min_voltage, float drive_max_voltage, float heading_max_voltage, float drive_settle_error, float drive_settle_time, float drive_timeout, float drive_kp, float drive_ki, float drive_kd, float drive_starti, float heading_kp, float heading_ki, float heading_kd, float heading_starti);

void turntoPoint(float X_position, float Y_position, float extra_angle_deg, float turn_max_voltage, float turn_settle_error, float turn_settle_time, float turn_timeout, float turn_kp, float turn_ki, float turn_kd, float turn_starti);

void tankDrive(float leftInput, float rightInput, float deadBand);

void arcadeDrive(float forward, float turn, float deadBand);
#endif