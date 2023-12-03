#pragma once
// Ensures that the header is included only once during compilation

#include "rclcpp/rclcpp.hpp" // Main header for ROS 2 C++ functionality

// Include the header files for the Navigation, Perception, and Manipulation classes
#include "./Manipulation.hpp"
#include "./Navigation.hpp"
#include "./Perception.hpp"

// Definition of the RRR class
class RRR {
 public:
    RRR();
    // Constructor for the RRR class

    Navigation nav;
    // Member variable of type Navigation for navigation-related tasks

    Perception perc;
    // Member variable of type Perception for perception-related tasks

    Manipulation manip;
    // Member variable of type Manipulation for manipulation-related tasks
};