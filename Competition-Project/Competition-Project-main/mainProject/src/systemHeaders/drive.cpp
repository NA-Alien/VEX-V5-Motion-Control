#include "vex.h"
vex::brain Brain = vex::brain();

vex::controller master = vex::controller();

// TOP RIGHT
vex::motor TL(vex::PORT20, vex::gearSetting::ratio6_1, false); //
// TOP LEFT
vex::motor TR(vex::PORT18, vex::gearSetting::ratio6_1, true); //
// BOTTOM RIGHT
vex::motor BL(vex::PORT14, vex::gearSetting::ratio6_1, true); //
// BOTTOM LEFT
vex::motor BR(vex::PORT16, vex::gearSetting::ratio6_1, false); //
// MIDDLE RIGHT
vex::motor ML(vex::PORT19, vex::gearSetting::ratio6_1, true); //
// MIDDLE LEFT
vex::motor MR(vex::PORT17, vex::gearSetting::ratio6_1, false); //
// INTAKE
vex::motor intake(vex::PORT15, vex::gearSetting::ratio6_1, false);

vex::motor lift(vex::PORT12, vex::gearSetting::ratio36_1, false);

// vision sensor
vex::inertial imu = vex::inertial(PORT9);

motor_group Left{
    TL, BL, ML};

motor_group Right{
    TR, BR, MR};

vex::pneumatics clamp1 = vex::pneumatics(Brain.ThreeWirePort.B);
vex::pneumatics doinker = vex::pneumatics(Brain.ThreeWirePort.A);
vex::rotation liftAngle = vex::rotation(PORT21);

vex::drivetrain driving = {Right, Left};

vex::rotation sideWaysTracker = vex::rotation(PORT5);

vex::optical op = vex::optical(PORT8);

Big odom;

float wheelDiameter = 2.75;

float wheelRatio = 0.75;

void driveFor(float distance, float drivekP, float drivekD, float drivekI, float driveintegralThresh, float drive_settle_error, float drive_settle_time, float drive_timeout, float minVoltage, float maxVoltage)
{
    Brain.Screen.clearScreen();
    PID drivePID(distance, drivekP, drivekD, drivekI, driveintegralThresh, drive_settle_error, drive_settle_time, drive_timeout);
    resetDriveEncoders();
    float robotPos = ((Right.position(deg) * (0.75 / 360.0 * M_PI * 2.75) + Left.position(deg) * (0.75 / 360.0 * M_PI * 2.75)) / 2);

    while (drivePID.is_settled() == false)
    {
        robotPos = ((Right.position(deg) * (0.75 / 360.0 * M_PI * 2.75) + Left.position(deg) * (0.75 / 360.0 * M_PI * 2.75)) / 2);
        float driveError = distance - robotPos;
        float driveOutput = drivePID.calculate(driveError);

        printf("driveError: %f \n", driveError);
        Brain.Screen.printAt(1, 10, "leftEncoders: %f \n", Left.position(deg) * (0.75 / 360.0 * M_PI * 2.75)); // converted into inches
        Brain.Screen.printAt(1, 30, "rawleftEncoders: %f \n", Left.position(deg));
        Brain.Screen.printAt(1, 60, "rightEncoders: %f \n", Right.position(deg) * (0.75 / 360.0 * M_PI * 2.75));
        Brain.Screen.printAt(1, 90, "rawleftEncoders: %f \n", Right.position(deg));
        Brain.Screen.printAt(1, 90, "driveerror: %f \n", driveError);

        float newDriveOutput = clamp(driveOutput, minVoltage, maxVoltage);

        Left.spin(fwd, driveOutput, volt);
        Right.spin(fwd, driveOutput, volt);

        task::sleep(10);
    }
    driving.stop(hold);
    Brain.Screen.printAt(1, 200, "Done!");
}

void turnFor(float angle, float turnkP, float turnkD, float turnkI, float integralThresh, float turn_settle_error, float turn_settle_time, float turntimeout)
{
    // Brain.Screen.clearScreen();
    PID turnPID(reduce_negative_180_to_180(angle - imu.heading(degrees)), turnkP, turnkD, turnkI, integralThresh, turn_settle_error, turn_settle_time, turntimeout);
    while (turnPID.is_settled() == false)
    {

        float turnError = reduce_negative_180_to_180(angle - imu.heading(degrees));
        printf("turnError: %f \n", turnError);
        Brain.Screen.printAt(1, 150, "turnError: %f", turnError);
        float turnOutput = turnPID.calculate(turnError);

        Right.spin(fwd, -turnOutput, volt);
        Left.spin(fwd, turnOutput, volt);
        task::sleep(10);
    }
    driving.stop(hold);
    // Brain.Screen.printAt(1, 170, "Ending Theta: %f", imu.heading());
    // Brain.Screen.printAt(1, 200, "Done!");
}

void rightArcTurn(float angle, float arckP, float arckD, float arckI, float arcIntegralThresh, float arc_settle_error, float arc_settle_time, float arctimeout)
{
    Brain.Screen.clearScreen();
    PID arcPID(reduce_negative_180_to_180(angle - imu.heading(degrees)), arckP, arckD, arckI, arcIntegralThresh, arc_settle_error, arc_settle_time, arctimeout);

    while (arcPID.is_settled() == false)
    {
        float arcError = reduce_negative_180_to_180(angle - imu.heading(degrees));
        printf("rightarcError: %f \n", arcError);
        Brain.Screen.printAt(1, 150, "rightarcError: %f", arcError);
        float arcOutput = arcPID.calculate(arcError);

        Right.spin(reverse, arcOutput, volt);
        Left.stop(hold);
        task::sleep(10);
    }

    driving.stop(hold);
    Brain.Screen.printAt(1, 170, "Ending Theta: %f", imu.heading());
    Brain.Screen.printAt(1, 200, "Done!");
}

void leftArcTurn(float angle, float arckP, float arckD, float arckI, float arcIntegralThresh, float arc_settle_error, float arc_settle_time, float arctimeout)
{
    Brain.Screen.clearScreen();
    PID arcPID(reduce_negative_180_to_180(angle - imu.heading(degrees)), arckP, arckD, arckI, arcIntegralThresh, arc_settle_error, arc_settle_time, arctimeout);

    while (arcPID.is_settled() == false)
    {
        float arcError = reduce_negative_180_to_180(angle - imu.heading(degrees));
        printf("leftarcerror: %f \n", arcError);
        Brain.Screen.printAt(1, 150, "leftarcerror: %f", arcError);
        float arcOutput = arcPID.calculate(arcError);

        Left.spin(fwd, arcOutput, volt);
        Right.stop(hold);
        task::sleep(10);
    }

    driving.stop(hold);
    Brain.Screen.printAt(1, 170, "Ending Theta: %f", imu.heading());
    Brain.Screen.printAt(1, 200, "Done!");
}

void antiDriftD(float distance, float heading, float drivekP, float drivekD, float drivekI, float turnkP, float turnkD, float turnkI, float driveintegralThresh, float driftIntegralThresh, float drive_settle_error, float drive_settle_time, float drive_timeout)
{
    Brain.Screen.clearScreen();
    PID drivePID(distance, drivekP, drivekD, drivekI, driveintegralThresh, drive_settle_error, drive_settle_time, drive_timeout);
    PID driftPID(reduce_negative_180_to_180(heading - imu.heading(degrees)), turnkP, turnkD, turnkI, driftIntegralThresh);
    resetDriveEncoders();
    float robotPos = ((Right.position(deg) * (0.75 / 360.0 * M_PI * 2.75) + Left.position(deg) * (0.75 / 360.0 * M_PI * 2.75)) / 2);
    float desiredHeading = imu.heading();
    float saveOutPut;

    while (drivePID.is_settled() == false)
    {
        robotPos = ((Right.position(deg) * (0.75 / 360.0 * M_PI * 2.75) + Left.position(deg) * (0.75 / 360.0 * M_PI * 2.75)) / 2);
        float driveError = distance - robotPos;
        float driftError = reduce_negative_180_to_180(heading - imu.heading(deg));
        float driveOutput = drivePID.calculate(driveError);
        float driftOutput = driftPID.calculate(driftError);

        printf("driveError: %f \n", driveError);
        Brain.Screen.printAt(1, 10, "leftEncoders: %f \n", Left.position(deg) * (0.75 / 360.0 * M_PI * 2.75)); // converted into inches
        Brain.Screen.printAt(1, 30, "rawleftEncoders: %f \n", Left.position(deg));
        Brain.Screen.printAt(1, 60, "rightEncoders: %f \n", Right.position(deg) * (0.75 / 360.0 * M_PI * 2.75));
        Brain.Screen.printAt(1, 90, "rawleftEncoders: %f \n", Right.position(deg));
        Brain.Screen.printAt(1, 90, "driveerror: %f \n", driveError);

        Left.spin(fwd, driveOutput, volt);
        Right.spin(fwd, driveOutput + driftOutput, volt);

        task::sleep(10);
        saveOutPut = driftOutput;
    }

    Right.spin(fwd, saveOutPut, volt);
    driving.stop(hold);
    Brain.Screen.printAt(1, 200, "Done!");
}

void liftTo(float angle, float liftkP, float liftkD, float liftkI, float integralThresh, float lift_settle_error, float lift_settle_time, float lifttimeout)
{
    Brain.Screen.clearScreen();
    PID liftPID(reduce_negative_180_to_180(liftAngle.angle(degrees) - angle), liftkP, liftkD, liftkI, integralThresh, lift_settle_error, lift_settle_time, lifttimeout);
    while (liftPID.is_settled() == false)
    {

        float liftError = reduce_negative_180_to_180(liftAngle.angle(degrees) - angle);
        printf("liftError: %f \n", liftError);
        Brain.Screen.printAt(1, 150, "liftPID: %f", liftPID);
        float liftOutput = liftPID.calculate(liftError);

        lift.spin(fwd, liftOutput, volt);
        task::sleep(10);
    }
    lift.stop();
    Brain.Screen.printAt(1, 170, "Ending Theta: %f", liftAngle.angle(degrees));
    Brain.Screen.printAt(1, 200, "Done!");
}

// 0.75 being the gearRatio while 2.75 being the wheel diameter
vex::task odometry;

void trackPosition()
{
    while (1)
    {
        odom.updatePosition((Right.position(deg) * (0.75 / 360.0 * M_PI * 2.75) + Left.position(deg) * (0.75 / 360.0 * M_PI * 2.75)) / 2, sideWaysTracker.position(degrees) * (1.2 / 360.0 * M_PI * 2), imu.heading(degrees));
        Brain.Screen.printAt(5, 20, "X: %f", positionX());
        Brain.Screen.printAt(5, 40, "Y: %f", positionY());
    }
}

int positionTask()
{
    trackPosition();
    return (0);
}

void setCoords(float X_position, float Y_position, float orientation_deg)
{
    Brain.Screen.clearScreen();
    odom.setPosition(X_position, Y_position, orientation_deg, (Right.position(degrees) * (0.75 / 360.0 * M_PI * 2.75) + Left.position(degrees) * (0.75 / 360.0 * M_PI * 2.75)) / 2, sideWaysTracker.position(degrees) * (1.2 / 360.0 * M_PI * 2));
    imu.setHeading(orientation_deg, degrees);
    odometry = task(positionTask);
}

float positionX()
{
    return odom.X_position;
}

float positionY()
{
    return odom.Y_position;
}

void odom_test()
{
    setCoords(0, 0, 0);
    while (1)
    {
        Brain.Screen.printAt(5, 20, "X: %f", positionX());
        Brain.Screen.printAt(5, 40, "Y: %f", positionY());
        Brain.Screen.printAt(5, 60, "Heading: %f", imu.heading(degrees));
        Brain.Screen.printAt(5, 80, "ForwardTracker: %f", (Right.position(degrees) * (0.75 / 360.0 * M_PI * 2.75) + Left.position(degrees) * (0.75 / 360.0 * M_PI * 2.75)) / 2);
        Brain.Screen.printAt(5, 100, "SidewaysTracker: %f", sideWaysTracker.position(degrees) * (1.042 / 360.0 * M_PI * 2.75));
    }
}

void translate(float X_position, float Y_position, float drive_min_voltage, float drive_max_voltage, float heading_max_voltage, float drive_settle_error, float drive_settle_time, float drive_timeout, float drive_kp, float drive_ki, float drive_kd, float drive_starti, float heading_kp, float heading_ki, float heading_kd, float heading_starti)
{
    PID drivePID(hypot(X_position - positionX(), Y_position - positionY()), drive_kp, drive_ki, drive_kd, drive_starti, drive_settle_error, drive_settle_time, drive_timeout);
    float start_angle_deg = to_deg(atan2(X_position - positionX(), Y_position - positionY()));
    PID headingPID(start_angle_deg - imu.heading(degrees), heading_kp, heading_ki, heading_kd, heading_starti);
    bool line_settled = false;
    bool prev_line_settled = is_line_settled(X_position, Y_position, start_angle_deg, positionX(), positionY());
    while (!drivePID.is_settled())
    {
        line_settled = is_line_settled(X_position, Y_position, start_angle_deg, positionX(), positionY());
        if (line_settled && !prev_line_settled)
        {
            break;
        }
        prev_line_settled = line_settled;

        float drive_error = hypot(X_position - positionX(), Y_position - positionY());
        float heading_error = reduce_negative_180_to_180(to_deg(atan2(X_position - positionX(), Y_position - positionY())) - imu.heading(degrees));
        float drive_output = drivePID.calculate(drive_error);

        float heading_scale_factor = cos(to_rad(heading_error));
        drive_output *= heading_scale_factor;
        heading_error = reduce_negative_90_to_90(heading_error);
        float heading_output = headingPID.calculate(heading_error);

        if (drive_error < drive_settle_error)
        {
            heading_output = 0;
        }

        drive_output = clamp(drive_output, -fabs(heading_scale_factor) * drive_max_voltage, fabs(heading_scale_factor) * drive_max_voltage);
        heading_output = clamp(heading_output, -heading_max_voltage, heading_max_voltage);

        drive_output = clamp_min_voltage(drive_output, drive_min_voltage);

        Brain.Screen.printAt(5, 80, "X %f", positionX());
        Brain.Screen.printAt(5, 100, "Y %f", positionY());

        Left.spin(fwd, left_voltage_scaling(drive_output, heading_output), voltageUnits::volt);
        Right.spin(fwd, right_voltage_scaling(drive_output, heading_output), voltageUnits::volt);

        task::sleep(10);
    } 
    driving.stop(hold);
}

// if you want the back of your robot
//  to point at (36, 42), you would run turn_to_point(36, 42, 180).

void turntoPoint(float X_position, float Y_position, float extra_angle_deg, float turn_max_voltage, float turn_settle_error, float turn_settle_time, float turn_timeout, float turn_kp, float turn_ki, float turn_kd, float turn_starti)
{
    PID turnPID(reduce_negative_180_to_180(to_deg(atan2(X_position - positionX(), Y_position - positionY())) - imu.heading(degrees)), turn_kp, turn_ki, turn_kd, turn_starti, turn_settle_error, turn_settle_time, turn_timeout);
    while (turnPID.is_settled() == false)
    {
        float error = reduce_negative_180_to_180(to_deg(atan2(X_position - positionX(), Y_position - positionY())) - imu.heading(degrees) + extra_angle_deg);
        float output = turnPID.calculate(error);
        output = clamp(output, -turn_max_voltage, turn_max_voltage);
        Left.spin(fwd, output, volt);
        Right.spin(fwd, -output, volt);
        task::sleep(10);
    }
}

void tankDrive(float leftInput, float rightInput, float deadBand)
{

    float leftOutput = leftInput;
    float rightOutput = rightInput;

    Left.spin(fwd, leftOutput, volt);
    Right.spin(fwd, rightOutput, volt);
}

void arcadeDrive(float forward, float turn, float deadBand)
{

    float forwardout = forward * .45 / 2;
    float turnout = turn * .70 / 2;

    Left.spin(fwd, (forwardout - turnout), volt);
    Right.spin(fwd, (forwardout + turnout), volt);
}