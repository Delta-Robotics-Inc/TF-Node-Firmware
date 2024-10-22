#ifndef SETTING_H
#define SETTING_H

#include <EEPROM.h>
#include "tfnode-messages.h"

class NodeSettingsManager {
private:
    tfnode::NodeSettings* nodeSettings;
    uint8_t nodeIDAddress;

public:
    // Constructor
    NodeSettingsManager(tfnode::NodeSettings* settingsPtr, int address);

    // Load settings from EEPROM into the nodeSettings object
    void loadSettings();

    // Save current settings from nodeSettings to EEPROM
    void saveCurrentSettings();

    // Return a pointer to the current node settings
    tfnode::NodeSettings* getSettings();

    // Update the settings and save them to EEPROM
    void setSettings(tfnode::NodeSettings newSettings);
};

class SMASettingsManager {
private:
    tfnode::SMAControllerSettings* smaSettings;

public:
    // Constructor
    SMASettingsManager(tfnode::SMAControllerSettings* settingsPtr);

    // Load settings from EEPROM into the smaSettings object
    void loadSettings();

    // Save current settings from smaSettings to EEPROM
    void saveCurrentSettings();

    // Return a pointer to the current SMA settings
    tfnode::SMAControllerSettings* getSettings();

    // Update the settings and save them to EEPROM
    void setSettings(tfnode::SMAControllerSettings newSettings);
};

#endif // SETTING_H
