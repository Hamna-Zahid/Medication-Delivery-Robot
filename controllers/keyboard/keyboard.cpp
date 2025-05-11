#include <webots/DistanceSensor.hpp>
#include <webots/Motor.hpp>
#include <webots/Robot.hpp>
#include <webots/Keyboard.hpp>
#include <webots/GPS.hpp> 
#include <webots/InertialUnit.hpp>
#include <webots/Camera.hpp>
#include <iostream>  // Required for std::cout

#define TIME_STEP 64

using namespace webots;

int main(int argc, char **argv) {
  Robot *robot = new Robot();
  Keyboard kb;
  DistanceSensor *ds;
  
  // Initialize the distance sensor
  ds = robot->getDistanceSensor("distance_sensor");
  ds->enable(TIME_STEP);
  
  // Initialize the camera
  Camera *camera;
  camera = robot->getCamera("camera");
  camera->enable(TIME_STEP);
  camera->recognitionEnable(TIME_STEP);
  
    GPS *gps;
    gps = robot->getGPS("gps");
    gps->enable(TIME_STEP);
    
    // Inertial Unit for orientation tracking
    InertialUnit *iu;
    iu = robot->getInertialUnit("iu");  // Retrieve the Inertial Unit
    iu->enable(TIME_STEP);  // Enable the Inertial Unit
  
  // Initialize motors for the wheels
  Motor *wheels[4];
  char wheels_names[4][8] = {"RWheel1", "RWheel2", "RWheel3", "RWheel4"};
  for (int i = 0; i < 4; i++) {
    wheels[i] = robot->getMotor(wheels_names[i]);
    wheels[i]->setPosition(INFINITY);
    wheels[i]->setVelocity(0.0);
  }
  
  kb.enable(TIME_STEP);

  // Speed values for movement and turning
  double forwardSpeed = 1.0;
  double turnSpeed = 2.0;
  double leftSpeed = 0.0;
  double rightSpeed = 0.0;
  
  // Previous position to track movement in XYZ space
    double prevX = 0.0, prevY = 0.0, prevZ = 0.0;
    
  while (robot->step(TIME_STEP) != -1) {
    // Continuously read the distance sensor value
    double distanceValue = ds->getValue();
    std::cout << "Distance Sensor Value: " << distanceValue << " mm" << std::endl;
    
    const double *position = gps->getValues();
        double currentX = position[0];
        double currentY = position[1];
        double currentZ = position[2];

        // Print out the current GPS position
        std::cout << "Current Position: X=" << currentX << " Y=" << currentY << " Z=" << currentZ << std::endl;

        // Calculate movement based on the change in XYZ coordinates
        double movement = std::abs(currentX - prevX) + std::abs(currentY - prevY) + std::abs(currentZ - prevZ);
        std::cout << "Movement: " << movement << std::endl;

        // Get Inertial Unit values (roll, pitch, yaw - orientation angles)
        const double *inertialUnitValues = iu->getRollPitchYaw();  // Get orientation values
        double roll = inertialUnitValues[0];
        double pitch = inertialUnitValues[1];
        double yaw = inertialUnitValues[2];

        // Print Inertial Unit values (orientation in radians)
        std::cout << "Inertial Unit Values: Roll=" << roll << " Pitch=" << pitch << " Yaw=" << yaw << std::endl;
    // Get keyboard input
    int key = kb.getKey();

    // Control logic for movement and turning
    if (key == 317) {  // Forward (UP arrow key)
      leftSpeed = forwardSpeed;
      rightSpeed = forwardSpeed;
    } else if (key == 315) {  // Backward (DOWN arrow key)
      leftSpeed = -forwardSpeed;
      rightSpeed = -forwardSpeed;
    } else if (key == 316) {  // Turn Right (RIGHT arrow key)
      leftSpeed = turnSpeed;   // Left wheels move forward
      rightSpeed = -turnSpeed; // Right wheels move backward
    } else if (key == 314) {  // Turn Left (LEFT arrow key)
      leftSpeed = -turnSpeed;  // Left wheels move backward
      rightSpeed = turnSpeed;  // Right wheels move forward
    } else {
      leftSpeed = 0.0;
      rightSpeed = 0.0;
    }

    // Apply the velocities to the wheels
    wheels[0]->setVelocity(leftSpeed);
    wheels[1]->setVelocity(rightSpeed);
    wheels[2]->setVelocity(leftSpeed);
    wheels[3]->setVelocity(rightSpeed);

    // Print current wheel speeds for debugging
    std::cout << "Left Wheel Speed: " << leftSpeed << " | Right Wheel Speed: " << rightSpeed << std::endl;
  }

  delete robot;
  return 0;  // EXIT_SUCCESS
}
