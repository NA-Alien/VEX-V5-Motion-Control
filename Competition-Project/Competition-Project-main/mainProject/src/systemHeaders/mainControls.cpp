#include "vex.h"

bool intakeBothColor = false;
bool opposingRingInSight = false;

void intakeCodeColor()
{

    if (intakeBothColor)
    {

        if (master.ButtonL1.pressing())
        {
            intake.spin(reverse, 12, volt);
        }

        else if (master.ButtonL2.pressing())
        {

            intake.spin(fwd, 12, volt);
        }

        else
        {
            intake.stop(coast);
        }
    }
    else
    {
        if (op.color() == blue)
        {
            op.objectDetected(opposingRingFound);
            opposingRingInSight = true;
        }

        if (!opposingRingInSight)
        {
            if (master.ButtonL1.pressing())
            {
                intake.spin(reverse, 3, volt);
            }

            else if (master.ButtonL2.pressing())
            {

                intake.spin(fwd, 3, volt);
            }

            else
            {
                intake.stop(coast);
            }
        }
    }
}

void intakeCode()
{

    if (master.ButtonL1.pressing())
    {
        intake.spin(reverse, 12, volt);
    }

    else if (master.ButtonL2.pressing())
    {

        intake.spin(fwd, 12, volt);
    }

    else
    {
        intake.stop(coast);
    }
}

void opposingRingFound()
{
    intake.stop(hold);
}

void doinkerOpen()
{

    if (!master.ButtonRight.pressing())
    {
        return;
    }

    if (doinker.value() == true)
    {
        return;
    }

    task::sleep(200);
    doinker.open();
}

void doinkerClose()
{
    if (!master.ButtonRight.pressing())
    {
        return; 
    }

    if (doinker.value() == false)
    {
        return;
    }

    task::sleep(200);
    doinker.close();
}

void clampClose()
{
    if (!master.ButtonY.pressing())
    {
        return;
    }

    if (clamp1.value() == false)
    {
        return;
    }

    task::sleep(200);
    clamp1.close();
}

void clampOpen()
{
    if (!master.ButtonY.pressing())
    {
        return;
    }

    if (clamp1.value() == true)
    {
        return;
    }

    task::sleep(200);
    clamp1.open();
}

// controller logic

void intakeBoth()
{
    if (!master.ButtonUp.pressing())
    {
        return;
    }

    if (intakeBothColor)
    {
        return;
    }
    intakeBothColor = true;
}

void intakeOne()
{
    if (!master.ButtonUp.pressing())
    {
        return;
    }

    if (!intakeBothColor)
    {
        return;
    }
    intakeBothColor = false;
}

bool isLiftUp = false;
void liftUp()
{

    if (isLiftUp)
    {
        return;
    }

    lift.setBrake(brake);
    lift.stop();
    task::sleep(100);
    liftTo(341, 0.37, 0, 0, 0, 1.5, 100, 500);
    isLiftUp = true;
}

void liftDown()
{

    if (!isLiftUp)
    {
        return;
    }

    lift.setBrake(brake);
    liftTo(0, 0.3, 0, 0, 0, 2, 100, 300);
    isLiftUp = false;
}
bool upPressed = false;
bool reachedLimit = false;
float liftkP = 0.01;
float target = 355;
float shouldStop = false;

void liftScore1()
{

    if (!isLiftUp)
    {
        return;
    }

    float liftErrorZeroPosition = fabs(liftAngle.angle(degrees) - target);
    float zeroPositionErrorProp = liftErrorZeroPosition * liftkP;
    float liftAnglePosition = liftAngle.angle(degrees);

    if (master.ButtonR2.pressing())
    {

        Brain.Screen.printAt(1, 180, "PRESSING");
        intake.stop(coast);
         if (liftAngle.angle(degrees) <= 225)
        {
            lift.stop(hold);
            shouldStop = true;
            upPressed = true;
        }

        else if(!shouldStop)
        {
        lift.spin(fwd, 12, volt);
        upPressed = true;
        }
        shouldStop = false;


    }

    else if (upPressed)
    {

        liftTo(0, 0.3, 0, 0, 0, 2, 100, 300);
        lift.stop(coast);
        upPressed = false;
        isLiftUp = false;
    }
}

bool isHangUp = false;

void liftHangUp()
{
    
    if (isHangUp)
    {
        return;
    }
    liftTo(220, 0.3, 0, 0, 0, 2, 100, 500);
    isHangUp = true;
}

void liftHangDown()
{
    
    if (!isHangUp)
    {
        return;
    }
    liftTo(220, 0.3, 0, 0, 0, 2, 100, 500);
    isHangUp = false;
}

float liftTarget;
float highTarget = 210;
float lowTarget;
float liftVelocity;
bool bottomTarget = true;
float liftError;
float liftVelocityProp;

void liftControl()
{
    lift.setStopping(hold);

    // Controller logic
    // Controller print logic can be added if desired

    if (master.ButtonR2.pressing())
    {
        // If R1 pressed, set target to highest spot
        liftTarget = highTarget;
        liftVelocity = 80;
    }
    else if (master.ButtonR1.pressing())
    {
        // If Y button pressed, toggle lowest target
        bottomTarget = !bottomTarget;
        if (bottomTarget)
        {
            // Lowest target
            lowTarget = 360;
        }
        else
        {
            // Middle target
            lowTarget = 339;
        }
    }
    else
    {
        // Move lift to lowest target if nothing is pressed
        liftTarget = lowTarget;
        liftVelocity = 50;
    }

    if (liftTarget == highTarget)
    {
        if (liftAngle.angle(deg) > liftTarget - 5)
        {
            lift.spin(fwd, liftVelocity, vex::percentUnits::pct);
        }
        else if (liftAngle.angle(deg) < liftTarget + 5)
        {
            lift.spin(fwd, -liftVelocity, vex::percentUnits::pct);
        }
        else
        {
            lift.spin(fwd, 0, vex::percentUnits::pct);
        }
    }
    else
    {
        // Move the lift towards the target
        liftError = fabs(liftTarget - liftAngle.angle(degrees));
        liftVelocityProp = (liftkP * liftError);
        if (liftAngle.angle(degrees) > liftTarget - 3)
        {
            lift.spin(fwd, liftVelocityProp, vex::percentUnits::pct);
        }
        else if (liftAngle.angle(degrees) < liftTarget + 3)
        {
            // Optional: print velocity for debugging
            // Brain.Screen.print("velo: %f", liftError);
            lift.spin(fwd, -liftVelocityProp, vex::percentUnits::pct);
        }
        else
        {
            lift.spin(fwd, 0, vex::percentUnits::pct);
        }
    }
}
