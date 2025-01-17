//globals.hpp
#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include "networking/SerialInterface.h" // Include the header file for SerialInterface
#include "networking/CanInterface.h"    // Include the header file for CanInterface
#include "TFNode.hpp"                     // Include the header file for TFNode
#include "networking/CommandProcessor.hpp"   

#define DEBUG  // Define if wanting serial debug statements

// Global objects are defined in main.cpp
extern SerialInterface* serialInterface;
// Assume CANInterface and SPIInterface are implemented similarly
extern CANInterface* canInterface;
// SPIInterface spiInterface;

extern TFNode* master_tfNode;
extern CommandProcessor* commandProcessor;

#endif // GLOBALS_HPP
