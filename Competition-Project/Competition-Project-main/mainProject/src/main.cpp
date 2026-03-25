#include "vex.h"

using namespace vex;

// --- Global Objects ---
competition Competition;
brain Brain;

// --- Autonomous Selection Variables ---
int autonNumber = 4;

/**
 * @brief Draws the autonomous selection buttons on the V5 Brain Screen.
 */
void drawGui() {
    Brain.Screen.clearScreen();
    Brain.Screen.setFillColor(red);
    
    // Draw selection boxes
    for(int i = 0; i < 4; i++) {
        Brain.Screen.drawRectangle(1, i * 50, 480, 50);
    }

    Brain.Screen.setPenColor(white);
    Brain.Screen.setFont(mono20);
    Brain.Screen.printAt(50, 30, "Red WP");
    Brain.Screen.printAt(50, 80, "Blue WP");
    Brain.Screen.printAt(50, 130, "Red 4 Ring");
    Brain.Screen.printAt(50, 180, "Blue 4 Ring");
}

/**
 * @brief Handles touch events on the Brain screen to select the autonomous routine.
 */
void selectDrive() {
    int touchX = Brain.Screen.xPosition();
    int touchY = Brain.Screen.yPosition();
    const char* selectedName = "";

    if (touchX < 1) return;

    if (touchY <= 49) {
        autonNumber = 1;
        selectedName = "Red WP";
    } else if (touchY <= 99) {
        autonNumber = 2;
        selectedName = "Blue WP";
    } else if (touchY <= 149) {
        autonNumber = 3;
        selectedName = "Red 4 Ring";
    } else if (touchY <= 199) {
        autonNumber = 4;
        selectedName = "Blue 4 Ring";
    }

    // Feedback on selection
    Brain.Screen.setPenColor(green);
    Brain.Screen.setFont(mono12);
    Brain.Screen.clearLine(210);
    Brain.Screen.clearLine(230);
    Brain.Screen.printAt(50, 210, "Selected: %s", selectedName);
    Brain.Screen.printAt(50, 230, "Auton ID: %d", autonNumber);
}

// --- PID and Motion Constants ---
// Organized by motion type for easy tuning
namespace Config {
    const float drivekP = 0.69, drivekI = 0.03, drivekD = 0.017;
    const float turnkP  = 0.2,  turnkI  = 0.07, turnkD  = 1.05;
    const float arckP   = 0.55, arckI   = 0.016, arckD  = 3.3;
    const float liftkP  = 1.2,  liftkD  = 0.02;

    const float drive_settle_error = 1.5;
    const float turn_settle_error  = 1.0;
    const float drive_timeout      = 4000;
}

void pre_auton(void) {
    op.setLightPower(25, percent);
    drawGui();
    
    // Setup callbacks and sensors
    Brain.Screen.pressed(selectDrive);
    liftAngle.resetPosition();
    imu.calibrate();
    sideWaysTracker.resetPosition();
    resetDriveEncoders();
    
    driving.setStopping(coast);
}

void autonomous(void) {
    switch (autonNumber) {
        case 1: // Red WP
            driveFor(40.5, Config::drivekP, Config::drivekD, Config::drivekI, 0, Config::drive_settle_error, 10, 1000, 0, 9);
            leftArcTurn(9.5, Config::arckP, Config::arckD, Config::arckI, 15, 2, 10, 500);
            doinker.open();
            wait(150, msec);
            driveFor(-10, Config::drivekP, Config::drivekD, Config::drivekI, 0, Config::drive_settle_error, 10, 1000, 0, 9);
            doinker.close();
            // ... rest of routine
            break;

        case 3: // Red 4 Ring
            driveFor(-19, Config::drivekP, Config::drivekD, Config::drivekI, 0, Config::drive_settle_error, 100, 500, 0, 9);
            rightArcTurn(30, Config::arckP, Config::arckD, Config::arckI, 15, 2, 150, 500);
            clamp1.open();
            wait(150, msec);
            // ... rest of routine
            break;

        case 4: // Blue 4 Ring
            driveFor(-21, Config::drivekP, Config::drivekD, Config::drivekI, 0, Config::drive_settle_error, 50, 1000, 0, 9);
            // ... rest of routine
            break;

        default:
            // Placeholder for Case 2 or Case 5
            break;
    }
}

void usercontrol(void) {
    Brain.Screen.clearScreen();
    
    // Motor Initialization
    Left.stop(coast);
    Right.stop(coast);
    liftAngle.setPosition(360, degrees);

    // Controller Callbacks
    master.ButtonR1.pressed(liftUp);
    master.ButtonR1.pressed(liftDown);
    master.ButtonRight.pressed(doinkerClose);
    master.ButtonRight.pressed(doinkerOpen);
    master.ButtonY.pressed(clampOpen);
    master.ButtonY.pressed(clampClose);
    master.ButtonR2.pressed(liftScore1);

    while (true) {
        // Tank Drive or Arcade Drive (change based on preference)
        driveMove(master.Axis3.value(), master.Axis2.value(), 2);
        
        intakeCode();
        liftScore1();
        
        wait(20, msec); 
    }
}

int main() {
    Competition.autonomous(autonomous);
    Competition.drivercontrol(usercontrol);

    pre_auton();

    while (true) {
        wait(100, msec);
    }
}
