#include <webots/Supervisor.hpp>
#include <iostream>

using namespace webots;

int main(int argc, char **argv) {
  Supervisor *supervisor = new Supervisor();
  
  // Get the root node of the robot
  Node *robotNode = supervisor->getSelf();

  // Get the robot's total mass (this method retrieves the entire robot's mass)
  double robotMass = robotNode->getMass();
  
  std::cout << "Robot Mass: " << robotMass << " kg" << std::endl;

  delete supervisor;
  return 0;
}
