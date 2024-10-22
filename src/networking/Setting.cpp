#include "Settings.h"

class NodeSettingsManagers {
private:
    tfnode::NodeSettings* nodeSettings;
    uint8_t nodeIDAddress = 0;

public:
    // Constructor
    NodeSettingsManagers(tfnode::NodeSettings* settingsPtr) 
        : nodeSettings(settingsPtr) {}

    // Load settings from EEPROM into the nodeSettings object
    void loadSettings() {
        nodeSettings->set_can_id(EEPROM.read(0));//address of the node ID
    }

    // Save current settings from nodeSettings to EEPROM
    void saveCurrentSettings() {
        EEPROM.update(nodeIDAddress, nodeSettings->get_can_id());
    }

    // Return a pointer to the current node settings
    tfnode::NodeSettings* getSettings() {
        return nodeSettings;
    }

    // Update the settings and save them to EEPROM
    void setSettings(tfnode::NodeSettings newSettings) {
        *nodeSettings = newSettings;
        saveCurrentSettings();  // Automatically save to EEPROM after change
    }
};


class SMASettingsManagers {
private:
    tfnode::SMAControllerSettings* smaSettings;
 
public:
    // Constructor
    SMASettingsManagers(tfnode::SMAControllerSettings* settingsPtr) 
        : smaSettings(settingsPtr) {}

        
    // Load settings from EEPROM into the nodeSettings object
    void loadSettings() {
        // Byte array containing the representation of the float
        uint8_t bytes[4];
        // Reinterpret the bytes as a float by taking 4 bytes
        float value = *reinterpret_cast<float*>(bytes);
        for (int i = 0; i < 4; i++)
            bytes[i] = EEPROM.read(2+i);///pidKpAddress
        smaSettings->set_rctrl_kp(value);
        for (int i = 0; i < 4; i++)
            bytes[i] = EEPROM.read(7+i);//pidKiAddress
        smaSettings->set_rctrl_ki(value);
        for (int i = 0; i < 4; i++)
            bytes[i] = EEPROM.read(12+i);//pidKdAddress
        smaSettings->set_rctrl_kd(value);
        for (int i = 0; i < 4; i++)
            bytes[i] = EEPROM.read(17+i);
        smaSettings->set_default_setpoint(value);//defaultSetPoint    
    }

    // Save current settings from nodeSettings to EEPROM
    void saveCurrentSettings() {
        const uint8_t* pptr = reinterpret_cast<const uint8_t*>(&smaSettings->get_rctrl_kp());
        const uint8_t* iptr = reinterpret_cast<const uint8_t*>(&smaSettings->get_rctrl_ki());
        const uint8_t* dptr = reinterpret_cast<const uint8_t*>(&smaSettings->get_rctrl_kd());
        const uint8_t* defaultptr = reinterpret_cast<const uint8_t*>(&smaSettings->get_rctrl_kd());

        for (int i = 0; i < 4; i++) {
            EEPROM.update((2+i),pptr[i]);
        }
        for (int i = 0; i < 4; i++) {
            EEPROM.update((7+i),iptr[i]);
        }
        for (int i = 0; i < 4; i++) {
            EEPROM.update((12+i),dptr[i]);
        }
        for (int i = 0; i < 4; i++)
        {
            EEPROM.update((17+i),defaultptr[i]);
        }
    }

    // Return a pointer to the current node settings
    tfnode::SMAControllerSettings* getSettings() {
        return smaSettings;
    }

    // Update the settings and save them to EEPROM
    void setSettings(tfnode::SMAControllerSettings newSettings) {
        *smaSettings = newSettings;
        saveCurrentSettings();  // Automatically save to EEPROM after change
    }
};