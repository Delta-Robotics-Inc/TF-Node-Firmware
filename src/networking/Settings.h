#ifndef SETTING_H
#define SETTING_H

#include <EEPROM.h>
#include <vector>
#include "Packet.h"
#include "tfnode-messages.h"
#include "WriteBuffer.h"
#include "ReadBuffer.h"


//EEPROM<-->Protobuf<-->Settings

class NodeSettingsManager {
    public:
        //save protobuf info to EEPROM
        void protobuftoEEPROM(tfnode::NodeSettings ID);

        //save EEPROM to protobuf file
        tfnode::NodeSettings EEPROMtoProtobuf();

        //load Settings from protobuf
        void loadSettingsfromProtobuf(NodeAddress& ID);

        //save Settings to protobuf
        tfnode::NodeSettings saveSettingstoProtobuf();

        //reads the EEPROM
        void readEEPROM();

        std::vector<uint8_t> getNodeID();


        // // Return a pointer to the current node settings
        // tfnode::NodeSettings* getSettings();

        // // Update the settings and save them to EEPROM
        // void setSettings(tfnode::NodeSettings newSettings);
};

// class SMASettingsManager {
// private:
//     tfnode::SMAControllerSettings* smaSettings;

// public:
//     // Constructor
//     SMASettingsManager(tfnode::SMAControllerSettings* settingsPtr);

//     // Load settings from EEPROM into the smaSettings object
//     void loadSettings();

//     // Save current settings from smaSettings to EEPROM
//     void saveCurrentSettings();

//     // Return a pointer to the current SMA settings
//     tfnode::SMAControllerSettings* getSettings();

//     // Update the settings and save them to EEPROM
//     void setSettings(tfnode::SMAControllerSettings newSettings);
// };

#endif // SETTING_H
