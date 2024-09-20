// Settings.h
#ifndef SETTINGS_H
#define SETTINGS_H

#include <EEPROM.h>

struct Settings {
    uint8_t nodeId;
    float pidKp;
    float pidKi;
    float pidKd;
    uint8_t controlMode;
    // Add other settings as needed

    // Method to load settings from EEPROM
    void load() {
        EEPROM.get(0, *this);
    }

    // Method to save settings to EEPROM
    void save() const {
        EEPROM.put(0, *this);
    }
};

#endif // SETTINGS_H
