#include "vex.h"
using namespace vex;
competition Competition;

int autonNumber = 4 ;

void drawGui()
{
  Brain.Screen.setFillColor(red);

  Brain.Screen.drawRectangle(1, 0, 500, 50);
  Brain.Screen.drawRectangle(1, 50, 500, 50);
  Brain.Screen.drawRectangle(1, 100, 500, 50);
  Brain.Screen.drawRectangle(1, 150, 500, 50);
  Brain.Screen.setPenColor(white);
  Brain.Screen.setFont(mono20);
  Brain.Screen.printAt(50, 25, "Red WP");
  Brain.Screen.printAt(50, 75, "Blue WP");
  Brain.Screen.printAt(50, 125, "Red 4 Ring");
  Brain.Screen.printAt(50, 175, "Blue 4 Ring");
}
void selectDrive()
{

  int touchX = Brain.Screen.xPosition();
  int touchY = Brain.Screen.yPosition();

  if (touchY <= 49 && touchX >= 1)
  {
    autonNumber = 1;
    Brain.Screen.clearLine(200);
    Brain.Screen.setFont(mono12);
    Brain.Screen.setPenColor(green);
    Brain.Screen.printAt(50, 210, "Red WP");
    Brain.Screen.printAt(50, 230, "Number Selected: %d", autonNumber);
  }

  if (touchY >= 50 && touchY <= 99 && touchX >= 1)
  {
    autonNumber = 2;
    Brain.Screen.clearLine(200);
    Brain.Screen.setFont(mono12);
    Brain.Screen.setPenColor(green);
    Brain.Screen.printAt(50, 210, "Blue WP");
    Brain.Screen.printAt(50, 230, "Number Selected: %d", autonNumber);
  }

  if (touchY >= 100 && touchY <= 149 && touchX >= 1)
  {
    autonNumber = 3;
    Brain.Screen.clearLine(200);
    Brain.Screen.setFont(mono12);
    Brain.Screen.setPenColor(green);
    Brain.Screen.printAt(50, 210, "Red 4 Ring");
    Brain.Screen.printAt(50, 230, "Number Selected: %d", autonNumber);
  }

  if (touchY >= 150 && touchX >= 1)
  {
    autonNumber = 4;
    Brain.Screen.clearLine(200);
    Brain.Screen.setFont(mono12);
    Brain.Screen.setPenColor(green);
    Brain.Screen.printAt(50, 210, "Blue 4 Ring");
    Brain.Screen.printAt(50, 230, "Number Selected: %d", autonNumber);
  }
}

void pre_auton(void)
{
  op.setLightPower(25, percent);
  drawGui();
  Brain.Screen.pressed(selectDrive);
  liftAngle.resetPosition();
  imu.calibrate();
  sideWaysTracker.resetPosition();
  resetDriveEncoders();
  driving.setStopping(coast);
}

// int autonSelected = 2;
float const drivekP = 0.69;
float const drivekD = 0.017;
float const drivekI = 0.03;
float const turnkP = 0.2;
float const turnkD = 1.05;
float const turnkI = 0.07;
float const odomkP = 1.1;
float const odomkD = 0;
float const odomkI = 0;
float const oturnkP = 0.095;
float const oturnkD = 0.023;
float const oturnkI = 0;
float const rightArckP = 0.55;
float const rightArckD = 3.3;
float const rightArckI = 0.016;
float const leftArckP = 0.55;
float const leftArckD = 3.3;
float const leftArckI = 0.016;
float const liftkP = 1.2;
float const liftkD = 0.02;
float const driveIntegralTresh = 0;
float const turnIntegralTresh = 0;
float const driftIntegralTresh = 0;
float const arcIntegralTresh = 15;
float const drive_settle_error = 1.5;
float const turn_settle_error = 1;
float const arc_settle_error = 2;
float const drive_settle_time = 150;
float const turn_settle_time = 150;
float const arc_settle_time = 300;
float const default_settle_time = 100;
float const drive_timeout = 4000;
float const turntimeout = 2500;
float const arctimeout = 2500;

void autonomous(void)
{
  switch (autonNumber)

  {
  case 1:
    // Red WP
    /*
    Brain.Screen.clearScreen();
    setCoords(0, 0, 0.04);
    turntoPoint(24, 24, 0,  10, turn_settle_error, 50, 1000, turnkP, turnkI, turnkD, turnIntegralTresh);
    translate(24, 24, 0, 12, 10, drive_settle_error, 50, drive_timeout, drivekP, drivekI, drivekD, driveIntegralTresh, turnkP, turnkI, turnkD, turnIntegralTresh);
    translate(0, 0, 0, 12, 10, drive_settle_error, 50, drive_timeout, drivekP, drivekI, drivekD, driveIntegralTresh, turnkP, turnkI, turnkD, turnIntegralTresh);
    turnFor(0, turnkP, turnkD, turnkI, turnIntegralTresh, turn_settle_error, 100, 1000);
    */

    driveFor(40.5, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, 10, 1000, 0, 9);
    leftArcTurn(9.5, leftArckP, leftArckD, leftArckI, arcIntegralTresh, arc_settle_error, 10, 500); // 295 -> 360 - 295 = 65
    doinker.open();
    wait(150, msec);
    driveFor(-10, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, 10, 1000, 0, 9);
    doinker.close();
    driveFor(2.5, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, 10, 1000, 0, 9);
    wait(200, msec);
    turnFor(220, rightArckP, rightArckD, rightArckI, arcIntegralTresh, arc_settle_error, 150, 1000);
    driveFor(-11.5, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, 10, 1000, 0, 9);
    clamp1.open();
    wait(250, msec);
    turnFor(134, turnkP, turnkD, turnkI, turnIntegralTresh, turn_settle_error, 150, 1000);
    intake.spin(reverse, 12, volt);
    driveFor(22, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, 10, 1000, 0, 9);
    wait(250, msec);
    clamp1.close();
    intake.stop(coast);
    turnFor(0, turnkP, turnkD, turnkI, turnIntegralTresh, turn_settle_error, 500, 1000);
    turnFor(90, turnkP, turnkD, turnkI, turnIntegralTresh, turn_settle_error, 100, 1000);
    driveFor(-39, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, 100, 1000, 0, 9);
    clamp1.open();
    intake.spin(reverse, 12, volt);
    wait(250, msec);
    turnFor(202, turnkP, turnkD, turnkI, turnIntegralTresh, turn_settle_error, 100, 1000);
    driveFor(16.9, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, 100, 1000, 0, 9);
    doinker.open();
    wait(200, msec);
    driveFor(-12, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, 100, 1000, 0, 9);
    doinker.close();
    wait(200, msec);
    turnFor(232, turnkP, turnkD, turnkI, turnIntegralTresh, turn_settle_error, 100, 1000);
    driveFor(18, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, 100, 1000, 0, 9);
 
    break;
  case 2:
    // Blue WP
    
    break;
  case 3:
    // red 4 ring
    /*
    Brain.Screen.clearScreen();
    setCoords(0, 0, 0.04);
    turntoPoint(24, 24, 0,  10, turn_settle_error, 50, 1000, turnkP, turnkI, turnkD, turnIntegralTresh);
    translate(24, 24, 0, 12, 10, drive_settle_error, 50, drive_timeout, drivekP, drivekI, drivekD, driveIntegralTresh, turnkP, turnkI, turnkD, turnIntegralTresh);
    translate(0, 0, 0, 12, 10, drive_settle_error, 50, drive_timeout, drivekP, drivekI, drivekD, driveIntegralTresh, turnkP, turnkI, turnkD, turnIntegralTresh);
    turnFor(0, turnkP, turnkD, turnkI, turnIntegralTresh, turn_settle_error, 100, 1000);
    */

    driveFor(-19, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, default_settle_time, 500, 0, 9);
    rightArcTurn(30, rightArckP, rightArckD, rightArckI, arcIntegralTresh, arc_settle_error, 150, 500); // Adjusted angle to 330
    driveFor(-7, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, default_settle_time, 500, 0, 9);
    clamp1.open();
    wait(150, msec);
    turnFor(103, turnkP, turnkD, turnkI, turnIntegralTresh, turn_settle_error, default_settle_time, 500); // Adjusted angle to 257
    intake.spin(reverse, 12, volt);
    wait(100, msec);
    driveFor(22, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, default_settle_time, 1000, 0, 7);
    turnFor(170, turnkP, turnkD, turnkI, turnIntegralTresh, turn_settle_error, 100, 500); // Adjusted angle to 190
    driveFor(16, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, default_settle_time, 1000, 0, 4);
    wait(550, msec);
    driveFor(-10, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, default_settle_time, 1000, 0, 4);
    turnFor(200, turnkP, turnkD, turnkI, turnIntegralTresh, turn_settle_error, default_settle_time, 500); // Adjusted angle to 160
    driveFor(12, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, default_settle_time, 1000, 0, 4);
    wait(650, msec);
    driveFor(-35, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, default_settle_time, 1000, 0, 9);
    turnFor(280, turnkP, turnkD, turnkI, turnIntegralTresh, turn_settle_error, default_settle_time, 500); // Adjusted angle to 80
    wait(4150, msec);
    driveFor(95, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, default_settle_time, 3000, 0, 5);


   /*

   /*

    Brain.Screen.clearScreen();
    setCoords(0, 0, 180);
    translate(-5.6, 29, 0, 8, 10, drive_settle_error, 125, drive_timeout, drivekP, drivekI, drivekD, driveIntegralTresh, otu, turnkI, turnkD, turnIntegralTresh);
    // turntoPoint(-7, 24, 180,  10, turn_settle_error, 50, 1000, turnkP, turnkI, turnkD, turnIntegralTresh);
    wait(100, msec);
    clamp1.open();
    wait(150, msec);
    turnFor(325, turnkP, turnkD, turnkI, turnIntegralTresh, turn_settle_error, 50, 1000);
    wait(50, msec);
    intake.spin(reverse, 12, volt);
    driveFor(23.5, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, default_settle_time, 1000, 0, 9);
    intake.stop(coast);
    turnFor(285, rightArckP, rightArckD, rightArckI, arcIntegralTresh, arc_settle_error, 50, 1000);
    intake.spin(reverse, 12, volt);
    driveFor(5.8, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, 400, 1000, 0, 9);
    turnFor(271, rightArckP, rightArckD, rightArckI, arcIntegralTresh, arc_settle_error, 150, 1000);
    driveFor(16.5, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, 400, 1000, 0, 9);
    rightArcTurn(295, rightArckP, rightArckD, rightArckI, arcIntegralTresh, arc_settle_error, 150, 500);
    driveFor(-38, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, 100, 1000, 0, 9);
    turnFor(265, rightArckP, rightArckD, rightArckI, arcIntegralTresh, arc_settle_error, 150, 1000);
    driveFor(27.5, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, 400, 1000, 0, 9);
    turnFor(108.75, rightArckP, rightArckD, rightArckI, arcIntegralTresh, arc_settle_error, 75, 1000);
    driveFor(34.21, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, 100, 1000, 0, 9);
    */
  /*
   doinker.open();
    wait(200, msec);
    driveFor(-15.4, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, 100, 1000, 0, 9);
    doinker.close();
    wait(100, msec);
    turnFor(130.3, rightArckP, rightArckD, rightArckI, arcIntegralTresh, arc_settle_error, 1, 1000);
    driveFor(13.27, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, 100, 1000, 0, 9);
    turnFor(280, turnkP, turnkD, turnkI, turnIntegralTresh, turn_settle_error, 1, 200);
    driveFor(13, 1, drivekD, drivekI, driveIntegralTresh, drive_settle_error, 150, 1000, 0, 9);
*/
    /*
      // red 4 Ring
      driveFor(-19, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, default_settle_time, 500, 0, 9);
      rightArcTurn(30, rightArckP, rightArckD, rightArckI, arcIntegralTresh, arc_settle_error, 150, 500);
      driveFor(-7, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, default_settle_time, 500, 0, 9);
      clamp1.open();
      wait(200, msec);
      turnFor(103, turnkP, turnkD, turnkI, turnIntegralTresh, turn_settle_error, default_settle_time, 500);
      intake.spin(reverse, 12, volt);
      wait(500, msec);
      driveFor(22, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, default_settle_time, 1000, 0, 7);
      turnFor(170, turnkP, turnkD, turnkI, turnIntegralTresh, turn_settle_error, 100, 500);
      driveFor(14.5, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, default_settle_time, 1000, 0, 4);
      wait(550, msec);
      driveFor(-13, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, default_settle_time, 1000, 0, 4);
      turnFor(200, turnkP, turnkD, turnkI, turnIntegralTresh, turn_settle_error, default_settle_time, 500);
      driveFor(12, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, default_settle_time, 1000, 0, 4);
      wait(650, msec);
      driveFor(-35, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, default_settle_time, 1000, 0, 9);
      turnFor(280, turnkP, turnkD, turnkI, turnIntegralTresh, turn_settle_error, default_settle_time, 500);
      // wait(4000, msec);
      // driveFor(100, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, default_settle_time, 3000, 0, 5);
      */

    break;
  case 4:
    // Blue 4 Ring
    Brain.Screen.clearScreen();
    driveFor(-21, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, 50, 1000, 0, 9);
    rightArcTurn(30, rightArckP, rightArckD, rightArckI, arcIntegralTresh, arc_settle_error, 150, 500);
    driveFor(-5, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, default_settle_time, 1000, 0, 9);
    wait(100, msec);
    clamp1.open();
    wait(150, msec);
    turnFor(210, turnkP, turnkD, turnkI, turnIntegralTresh, turn_settle_error, 75, 1000); // 325 -> 360 - 325 = 35
    wait(50, msec);
    intake.spin(reverse, 12, volt);
    driveFor(24.23, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, default_settle_time, 1000, 0, 9);
    intake.stop(coast);
    turnFor(260, leftArckP, leftArckD, leftArckI, arcIntegralTresh, arc_settle_error, 150, 1000); // 285 -> 360 - 285 = 75
    intake.spin(reverse, 12, volt);
    driveFor(5.8, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, 400, 1000, 0, 9);
    turnFor(275, leftArckP, leftArckD, leftArckI, arcIntegralTresh, arc_settle_error, 150, 1000); // 271 -> 360 - 271 = 89
    driveFor(16.5, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, 400, 1000, 0, 9);
    leftArcTurn(245, leftArckP, leftArckD, leftArckI, arcIntegralTresh, arc_settle_error, 150, 500); // 295 -> 360 - 295 = 65
    driveFor(-37.23, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, 100, 1000, 0, 9);
    turnFor(265, leftArckP, leftArckD, leftArckI, arcIntegralTresh, arc_settle_error, 150, 1000); // 265 -> 360 - 265 = 95
    driveFor(29.5, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, 520, 1000, 0, 9);
    turnFor(63, turnkP, turnkD, turnkI, turnIntegralTresh, turn_settle_error, 75, 1000); // 325 -> 360 - 325 = 35
    wait(1000, msec);
    driveFor(98, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, 100, 4000, 0, 9);
   /*
    doinker.open();
    wait(200, msec);
    driveFor(-15.4, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, 100, 1000, 0, 9);
    doinker.close();
    wait(100, msec);
    turnFor(65, leftArckP, leftArckD, leftArckI, arcIntegralTresh, arc_settle_error, 1, 1000); // 130.3 -> 360 - 130.3 = 229.7
    driveFor(14.87, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, 100, 1000, 0, 9);
    turnFor(163, turnkP, turnkD, turnkI, arcIntegralTresh, arc_settle_error, 1, 1000); // 50 -> 360 - 50 = 310
    driveFor(13.4, 1, drivekD, drivekI, driveIntegralTresh, drive_settle_error, 150, 1000, 0, 9);
   */
    break;

  case 5:
    // skills :(
    intake.spin(reverse, 12, volt);
    wait(500, msec);
    intake.stop(coast);
    driveFor(13.7, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, 200, 1000, 0, 9);
    turnFor(90, turnkP, turnkD, turnkI, turnIntegralTresh, turn_settle_error, 200, 1000);
    driveFor(-26.52, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, 200, 1000, 0, 9);
    clamp1.open();
    wait(500, msec);
    turnFor(0, turnkP, turnkP, turnkP, turnIntegralTresh, turn_settle_error, 200, 1000);
    intake.spin(reverse, 12, volt);
    driveFor(24, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, 200, 1000, 0, 9);
    wait(500, msec);
    turnFor(270, turnkP, turnkD, turnkI, turnIntegralTresh, turn_settle_error, 200, 1000);
    driveFor(26.65, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, 200, 1000, 0, 9);
    wait(500, msec);
    turnFor(180, turnkP, turnkD, turnkI, turnIntegralTresh, turn_settle_error, 200, 1000);
    driveFor(27, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, 200, 1000, 0, 9);
    wait(500, msec);
    driveFor(12, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, 200, 1000, 0, 9);
    wait(500, msec);
    driveFor(-12, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, 200, 1000, 0, 9);
    turnFor(275, turnkP, turnkD, turnkI, turnIntegralTresh, turn_settle_error, 200, 1000);
    driveFor(8, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, 200, 1000, 0, 9);
    wait(500, msec);
    turnFor(30, turnkP, turnkD, turnkI, turnIntegralTresh, turn_settle_error, 200, 1000);
    intake.stop(coast);
    driveFor(-19, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, 200, 1000, 0, 9);
    clamp1.close();
    wait(500, msec);
    driveFor(23, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, 200, 1000, 0, 9);
    turnFor(285, turnkP, turnkD, turnkI, turnIntegralTresh, turn_settle_error, 200, 1000);
    driveFor(-95, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, 200, 1000, 0, 9);
    clamp1.open();
    wait(750, msec);
    turnFor(90, turnkP, turnkD, turnkI, turnIntegralTresh, turn_settle_error, 200, 1000);
    intake.spin(reverse, 12, volt);
    driveFor(26.5, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, 200, 1000, 0, 9);
    wait(500, msec);
    driveFor(12, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, 200, 1000, 0, 9);
    wait(500, msec);
    turnFor(0, turnkP, turnkD, turnkI, turnIntegralTresh, turn_settle_error, 200, 1000);
    driveFor(-19, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, 200, 1000, 0, 9);
    clamp1.close();
    wait(500, msec);
    driveFor(20, drivekP, drivekD, drivekI, driveIntegralTresh, drive_settle_error, 200, 1000, 0, 9);
    break;
  }
}

void usercontrol(void)
{
  Brain.Screen.clearScreen();

  Right.stop(coast);
  Left.stop(coast);
  liftAngle.setPosition(360, degrees);
  master.ButtonR1.pressed(liftUp);
  master.ButtonR1.pressed(liftDown);
  master.ButtonRight.pressed(doinkerClose);
  master.ButtonRight.pressed(doinkerOpen);
  master.ButtonY.pressed(clampOpen);
  master.ButtonY.pressed(clampClose);
  master.ButtonR2.pressed(liftScore1);
  // master.ButtonB.pressed(liftHangUp);
  // master.ButtonB.pressed(liftHangDown);
  // master.ButtonUp.pressed(intakeOne);
  // master.ButtonUp.pressed(intakeBoth);
  sideWaysTracker.setPosition(0, degrees);
  Left.setPosition(0, degrees);
  Right.setPosition(0, degrees);
  // odom_test();

  while (1)
  {

    /*
  Brain.Screen.printAt(1, 50, "leftEncoders: %f \n", Left.position(deg) * (0.75/360.0*M_PI*2.75));//converted into inches
  Brain.Screen.printAt(1, 70, "rawleftEncoders: %f \n", Left.position(deg));
  Brain.Screen.printAt(1, 90, "rightEncoders: %f \n",  Right.position(deg) * (0.75/360.0*M_PI*2.75));
  Brain.Screen.printAt(1, 110, "rawleftEncoders: %f \n", Right.position(deg));
  Brain.Screen.printAt(1, 130, "RobotPose: %f \n", (Right.position(deg) * (0.75/360.0*M_PI*2.75) + Left.position(deg) * (0.75/360.0*M_PI*2.75) ) / 2);
  Brain.Screen.printAt(1, 150, "Theta %f", imu.heading(degrees));b
  */

    driveMove(master.Axis3.value(), master.Axis2.value(), 2);
    // tankDrive(master.Axis3.value(), master.Axis2.value(), 0);
    intakeCode();
    liftScore1();
    // liftControl();
    wait(20, msec);
  }
}

int main()
{

  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  pre_auton();

  while (true)
  {
    wait(100, msec);
  }
}
