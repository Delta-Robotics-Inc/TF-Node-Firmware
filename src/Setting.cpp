#include <EEPROM.h>
#include "Settings.hpp"

class NodeSettingsManager {
private:
    NodeSettings* nodeSettings;
    uint8_t nodeIDAddress = 0;//address of the node ID

public:
    // Constructor
    NodeSettingsManager(NodeSettings* settingsPtr, int address) 
        : nodeSettings(settingsPtr), nodeIDAddress(address) {}

    // Load settings from EEPROM into the nodeSettings object
    void loadSettings() {
        nodeSettings -> nodeId = EEPROM.read(nodeIDAddress);
    }

    // Save current settings from nodeSettings to EEPROM
    void saveCurrentSettings() {
        EEPROM.update(nodeIDAddress, nodeSettings -> nodeId);
    }

    // Return a pointer to the current node settings
    NodeSettings* getSettings() {
        return nodeSettings;
    }

    // Update the settings and save them to EEPROM
    void setSettings(NodeSettings newSettings) {
        *nodeSettings = newSettings;
        saveCurrentSettings();  // Automatically save to EEPROM after change
    }
};



class SMASettingsManager {
private:
    SMASettings* smaSettings;
    // uint8_t pidKpAddress = 1;
    // uint8_t pidKiAddress = 2;
    // uint8_t pidKdAddress = 3;
    // uint8_t controlMode = 4;
    uint8_t addressArray[4] = {1,2,3,4};//pidKpAddress, pidKiAddress, pidKdAddress, controlModeAddress
    
public:
    // Constructor
    SMASettingsManager(SMASettings* settingsPtr, const uint8_t addresses[4]) 
        : smaSettings(settingsPtr) {
            for (int i = 0; i < 4; i++) {
                addressArray[i] = addresses[i];
            }
        }
        
    // Load settings from EEPROM into the nodeSettings object
    void loadSettings() {
        smaSettings ->pidKd  = EEPROM.read(addressArray[0]);
        smaSettings ->pidKi  = EEPROM.read(addressArray[1]);
        smaSettings ->pidKp  = EEPROM.read(addressArray[2]);
        smaSettings ->controlMode  = EEPROM.read(addressArray[3]);                
    }

    // Save current settings from nodeSettings to EEPROM
    void saveCurrentSettings() {
        EEPROM.update(addressArray[0], smaSettings ->pidKp);
        EEPROM.update(addressArray[1], smaSettings ->pidKp);
        EEPROM.update(addressArray[2], smaSettings ->pidKp);
        EEPROM.update(addressArray[3], smaSettings ->pidKp);
    }

    // Return a pointer to the current node settings
    SMASettings* getSettings() {
        return smaSettings;
    }

    // Update the settings and save them to EEPROM
    void setSettings(SMASettings newSettings) {
        *smaSettings = newSettings;
        saveCurrentSettings();  // Automatically save to EEPROM after change
    }
};