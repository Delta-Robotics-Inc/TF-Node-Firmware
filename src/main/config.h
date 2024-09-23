// config.h
#ifndef CONFIG_H
#define CONFIG_H

// Select the shield version
#include "config-shield-v1.0.h"

// Common configuration parameters
const char FIRMWARE_VERSION[] = "1.0.0";
constexpr uint8_t NODE_ID = 1;

const int SIGNAL_TIMEOUT = 2000;    // Amount of time (ms) between receiving master commands before auto-disable
const unsigned long LOG_MS = 20;    // Time between log frames (ms)



#endif // CONFIG_H
