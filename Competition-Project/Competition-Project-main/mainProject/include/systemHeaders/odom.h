#ifndef ODO
#define ODO
#include "vex.h"
class Big
{
private:
  float ForwardTracker_center_distance;
  float SidewaysTracker_center_distance;
  float ForwardTracker_position;
  float SideWaysTracker_position;

public:
  float X_position;
  float Y_position;
  float orientation_deg;

  void setPosition(float X_position, float Y_position, float orientation_deg, float ForwardTracker_position, float SidewaysTracker_position);
  void updatePosition(float ForwardTracker_position, float SidewaysTracker_position, float orientation_deg);
  void physicalDistances(float ForwardTracker_center_distance, float SidewaysTracker_center_distance);
};

void trackPosition();

int positionTask();

void setCoords(float X_position, float Y_position, float orientation_deg);
#endif