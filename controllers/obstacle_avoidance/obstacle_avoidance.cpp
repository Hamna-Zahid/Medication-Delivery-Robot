#include <webots/DistanceSensor.hpp>
#include <webots/Motor.hpp>
#include <webots/Robot.hpp>
#include <iostream>

#define TIME_STEP 64

using namespace webots;

int main(int argc, char **argv) {
    Robot *robot = new Robot();

    // Single Distance Sensor
    DistanceSensor *ds;
    ds = robot->getDistanceSensor("distance_sensor");
    ds->enable(TIME_STEP);

    // Motors
    Motor *wheels[4];
    char wheels_names[4][10] = {"RWheel1", "RWheel2", "RWheel3", "RWheel4"};
    for (int i = 0; i < 4; i++) {
        wheels[i] = robot->getMotor(wheels_names[i]);
        wheels[i]->setPosition(INFINITY); // Allow infinite rotation
        wheels[i]->setVelocity(0.0);      // Initially, set velocity to 0
    }

    const double collisionThreshold = 100.0;  // Sensitivity for actual collisions in mm (0.1 meters)
    bool avoidingCollision = false;             // Flag to check if the robot is avoiding a collision
    int reverseDuration = 20;                   // Duration for reversing

    while (robot->step(TIME_STEP) != -1) {
        double speed = 1.0;  // Default speed for moving forward
        double leftSpeed = speed;   // Speed for left wheels
        double rightSpeed = speed;  // Speed for right wheels

        // Read distance sensor value (assumed to be in mm)
        double distanceValue = ds->getValue();
        std::cout << "Distance Sensor Value: " << distanceValue << " mm" << std::endl;

        // If the distance sensor detects something close (less than 0.1 meters)
        if (distanceValue < collisionThreshold && !avoidingCollision) {  
            avoidingCollision = true;  // Set the flag to avoid collision
            std::cout << "Collision detected! Initiating avoidance." << std::endl;
        }

        // Collision reaction logic
        if (avoidingCollision) {
            // Move backward
            leftSpeed = -1.0;    // Reverse both left wheels
            rightSpeed = -1.0;   // Reverse both right wheels

            std::cout << "Reversing to avoid obstacle." << std::endl;

            // Countdown to reset the avoiding collision state
            reverseDuration--;
            if (reverseDuration <= 0) {
                avoidingCollision = false;  // Reset the avoiding collision flag
                reverseDuration = 20;        // Reset the countdown for the next possible collision
            }
        } else {
            // Continue moving forward if not avoiding
            leftSpeed = speed;
            rightSpeed = speed;
        }

        // Set the speeds for the wheels
        wheels[0]->setVelocity(leftSpeed);
        wheels[1]->setVelocity(leftSpeed);  // Set same speed for front left and back left
        wheels[2]->setVelocity(rightSpeed);
        wheels[3]->setVelocity(rightSpeed); // Set same speed for front right and back right

        // Print out the current speed values for both left and right wheels
        std::cout << "Left Wheel Speed: " << leftSpeed << " | Right Wheel Speed: " << rightSpeed << std::endl;
    }

    delete robot;
    return 0; // EXIT_SUCCESS
}
