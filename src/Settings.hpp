// Settings.h
#ifndef SETTINGS_H
#define SETTINGS_H

#include <EEPROM.h>
#include <iostream>

struct NodeSettings {
    uint8_t nodeId;
    // Add other settings as needed
};

struct SMASettings {
    // Controller-specific
    float pidKp;
    float pidKi;
    float pidKd;
    uint8_t controlMode;
};


#endif // SETTINGS_H
