VEX V5 Motion Library
A modular C++ library for VEX V5 robots featuring advanced position tracking (Odometry), custom PID control, and automated motion algorithms. This system is designed for high-precision autonomous routines and responsive driver control.

Core Features
1. Arc-Based Odometry
The tracking system uses a 2-wheel encoder setup combined with a 3-axis Inertial Sensor. It implements a non-linear arc model to calculate position updates, which remains accurate even during high-speed turns.

Coordinate System: Maintains a global (X, Y) position and orientation (Theta).

Local to Global Transformation: Uses trigonometry to translate local encoder deltas into field-relative coordinates.

2. Custom PID Controller
The PID implementation includes specialized logic for competitive robotics:

Integral Thresholding: Only accumulates error when near the target to prevent massive overshoot (Integral Windup).

Zero-Crossing Reset: Instantly clears the integral sum if the robot crosses the target point.

Settle Logic: Ensures the robot is physically still and on-target for a set duration before moving to the next command.

3. Motion Algorithms
Translate to Point: Moves to an (X, Y) coordinate while dynamically adjusting heading.

Arc Turns: Executes curved paths by pivoting on a specific radius for smoother transitions.

Anti-Drift Drive: Uses two simultaneous PID loops (one for distance, one for heading) to keep the robot driving perfectly straight.

Exponential Drive (EXPO): Applies a mathematical curve to joystick inputs for better precision at low speeds.

Code Examples
Moving to a Target Coordinate
Using the translate function to navigate to a specific spot on the field:

C++

// Move to coordinate (24, 24)
// Parameters: X, Y, min_volt, max_volt, turn_max, settle_err, settle_time, timeout, gains...
translate(24, 24, 0, 12, 10, 1.5, 50, 4000, 
          drivekP, drivekI, drivekD, 0, 
          turnkP, turnkI, turnkD, 0);
Executing an Arc Turn
A right-handed curve used in the Red 4-Ring autonomous routine:

C++

// Right Arc Turn to 30 degrees
// Parameters: angle, kP, kD, kI, int_thresh, settle_err, settle_time, timeout
rightArcTurn(30, 0.55, 3.3, 0.016, 15, 2, 150, 500);
Turning to Face a Point
Rotating the chassis to face a specific object or coordinate:

C++

// Face the point (103, 103)
turntoPoint(103, 103, 0, 10, 1.0, 150, 2500, 
            0.2, 0.07, 1.05, 0);
Project Structure
odom.cpp / odom.h: The position tracking class and coordinate math.

PID.cpp / PID.h: The generalized PID controller with settling logic.

drive.cpp / drive.h: High-level autonomous movement functions.

exponen.cpp: Mathematical scaling for driver-controlled joystick input.

mainControls.cpp: Automated intake and lift logic.

util.cpp: Unit conversions (degrees to radians) and voltage clamping.

main.cpp: Competition entry point and touchscreen GUI selection.

Hardware Configuration
Drivetrain: 6-motor 600RPM 11W drivetrain.

Sensors: V5 Inertial Sensor, V5 Optical Sensor, High-Res Rotation Sensors.

Actuators: Pneumatic mobile goal clamp and doinker mechanism.
