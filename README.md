# VEX V5 Motion Library (C++)

A high-performance, modular C++ library developed for VEX V5 robotics competition. This system features advanced position tracking (Odometry), custom PID control loops, and non-linear motion algorithms designed for high-precision autonomous routines.

## Technical Overview

This library provides the foundational "brain" for a 6-motor 600RPM drivetrain. It bridges the gap between raw sensor data and complex autonomous navigation by implementing coordinate geometry and closed-loop control theory.

## Core Engineering Features

### 1. Arc-Based Odometry System
Uses a 2-wheel encoder setup paired with a 3-axis V5 Inertial Sensor to maintain a global (X, Y) coordinate and orientation (Theta).
* **Non-Linear Arc Model:** Calculates position updates based on curved paths rather than linear approximations, maintaining sub-inch accuracy during high-speed turns.
* **Local-to-Global Transformation:** Utilizes trigonometric mapping to translate local encoder deltas into field-relative coordinates.

### 2. Advanced PID Control Logic
A generalized PID controller optimized for the physical constraints of competitive robotics:
* **Integral Thresholding:** Mitigates "Integral Windup" by only accumulating error when the robot is within a specific range of the target.
* **Zero-Crossing Reset:** Instantly clears the integral sum when the robot crosses the target point to prevent oscillating overshoot.
* **Settle Logic:** Custom algorithms ensure the robot is physically stationary and on-target for a defined duration before executing the next command.

### 3. Motion Algorithms
* **Translate to Point:** Moves to specific (X, Y) coordinates while dynamically adjusting heading.
* **Anti-Drift Drive:** Synchronizes dual PID loops (Distance + Heading) to maintain a perfectly straight path despite motor variance.
* **Exponential Drive (EXPO):** Implements a mathematical scaling curve for joystick inputs, providing finer precision at low speeds for driver control.

## Code Implementation Examples

### Autonomous Coordinate Navigation
```cpp
// Navigate to coordinate (24, 24) on the field
// Parameters: X, Y, min_volt, max_volt, turn_max, settle_err, settle_time, timeout, gains...
translate(24, 24, 0, 12, 10, 1.5, 50, 4000, 
          drivekP, drivekI, drivekD, 0, 
          turnkP, turnkI, turnkD,
```
### Precision Arc Turn
```cpp
// Right-handed curve used in the Red 4-Ring autonomous routine
// Parameters: angle, kP, kD, kI, int_thresh, settle_err, settle_time, timeout
rightArcTurn(30, 0.55, 3.3, 0.016, 15, 2, 150, 500);
```
### Repository Structure
odom.cpp / .h: Coordinate geometry and position tracking engine.

PID.cpp / .h: Closed-loop controller with integral management and settling logic.

drive.cpp / .h: High-level motion functions (Translate, Turn, Arc).

exponen.cpp: Mathematical scaling for non-linear joystick input.

util.cpp: Unit conversions (Degrees/Radians) and voltage clamping utility.

### Hardware Integration
Drivetrain: 6-motor 11W (600 RPM) configuration.

Sensors: V5 Inertial Sensor, V5 Optical Sensor, V5 Rotation Sensors 

Actuators: Pneumatic mobile goal clamp and auxiliary "doinker" mechanism.
