#ifndef CONFIG_H
#define CONFIG_H

// Select the shield version
#include "config-shield-v1.0.hpp"
#include <Arduino.h>
#include <vector>

// Common configuration parameters
const char CFG_FIRMWARE_VERSION[] = "1.1.0";  // Increment before pull request to main branch
//no longer a need to hard code in a NODE_ID
//const std::vector<uint8_t> NODE_ID = {0x01, 0x02, 0x03};
const int NODE_ID_LOC = 4000; // EEPROM location where the node ID is stored (read-only)

const int SIGNAL_TIMEOUT =              2000;  // Amount of time (ms) between receiving master commands before auto-disable
const int RECONNECTION_INTERVAL_MS =    5000;  // Attempt to reconnect to enabled NetworkInterfaces on this interval
const unsigned long LOG_MS =            200;   // Time between log frames (ms)

// TODO pull these from .proto file
//#define CTRL_MODE_CNT 7
//enum ctrl_modes { PERCENT, VOLTS, AMPS, DEGREES, OHMS, TRAIN, MANUAL };  // TODO add Length and Force
//const String ctrl_modes_str[CTRL_MODE_CNT] = { "percent", "volts", "amps", "degrees", "ohms", "train", "manual" };

#endif // CONFIG_H
