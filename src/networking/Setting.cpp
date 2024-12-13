#include "Settings.h"
#include "config.hpp"

// TODO clean up this functionality

//save protobuf info to EEPROM
void protobuftoEEPROM(tfnode::NodeSettings IDs){
    uint8_t DataBuffer[1000];// temporarily large
    WriteBuffer loadData(DataBuffer, sizeof(DataBuffer));

    ::EmbeddedProto::Error load_err =  IDs.serialize(loadData);

    // Set the loaded settings Object for the Node to the one that was just serialized
    //Look at what errors is occuring in this statment
    if(load_err == ::EmbeddedProto::Error::NO_ERRORS) {

        for (int i = 5; i < sizeof(DataBuffer); i++){
            EEPROM.update(i, DataBuffer[i]);
        }
    }
    else{
        Serial.println("Error saving settings to EEPROM");
    }

}

//save EEPROM to protobuf file
tfnode::NodeSettings EEPROMtoProtobuf(){
    uint8_t DataBuffer[1000];// temporarily large

    for (int i = 5; i < sizeof(DataBuffer); i++){
        DataBuffer[i] = EEPROM.read(i);
        Serial.println(EEPROM.read(i));
    }
    
    ReadBuffer saveData(DataBuffer,sizeof(DataBuffer));
    tfnode::NodeSettings settings;
    ::EmbeddedProto::Error save_err = settings.deserialize(saveData);

    if(save_err == ::EmbeddedProto::Error::NO_ERRORS) {
        //failed to move EEPROM to protobuf
        //Handle error
    }
    else{
        Serial.println("Error loading settings from EEPROM");
    }

    return settings;
}

//load Settings from protobuf
void loadSettingsfromProtobuf(NodeAddress& ID){
    uint8_t DataBuffer[1000];// temporarily large
    WriteBuffer loadData(DataBuffer, sizeof(DataBuffer));

    tfnode::NodeSettings settings;
    ::EmbeddedProto::Error load_err =  settings.serialize(loadData);


    if(load_err == ::EmbeddedProto::Error::NO_ERRORS) {

    }
    else{
        Serial.println("Error loading settings from protobuf");
    }

}

//save Settings to protobuf
tfnode::NodeSettings saveSettingstoProtobuf(){
    uint8_t DataBuffer[1000];// temporarily large
    
    ReadBuffer saveData(DataBuffer,sizeof(DataBuffer));
    tfnode::NodeSettings settings;
    ::EmbeddedProto::Error save_err = settings.deserialize(saveData);

    if(save_err == ::EmbeddedProto::Error::NO_ERRORS) {
        //failed to move EEPROM to protobuf
        //Handle error
    }
    else{
        //================================================
        Serial.println("Error saving settings to protobuf");
        //================================================
    }

    return settings;

}

std::vector<uint8_t> NodeSettingsManager::getNodeID(){
std::vector<uint8_t> NODE_ID;
NODE_ID.clear();
for (int i = NODE_ID_LOC; i < NODE_ID_LOC+3; i++)
{
    NODE_ID.push_back(EEPROM.read(i));

}
return NODE_ID;
}


// class SMASettingsManagers {
// private:
//     tfnode::SMAControllerSettings* smaSettings;
 
// public:
//     // Constructor
//     SMASettingsManagers(tfnode::SMAControllerSettings* settingsPtr) 
//         : smaSettings(settingsPtr) {}

        
//     // Load settings from EEPROM into the nodeSettings object
//     void loadSettings() {
//         // Byte array containing the representation of the float
//         uint8_t bytes[4];
//         // Reinterpret the bytes as a float by taking 4 bytes
//         float value = *reinterpret_cast<float*>(bytes);
//         for (int i = 0; i < 4; i++)
//             bytes[i] = EEPROM.read(2+i);///pidKpAddress
//         smaSettings->set_rctrl_kp(value);
//         for (int i = 0; i < 4; i++)
//             bytes[i] = EEPROM.read(7+i);//pidKiAddress
//         smaSettings->set_rctrl_ki(value);
//         for (int i = 0; i < 4; i++)
//             bytes[i] = EEPROM.read(12+i);//pidKdAddress
//         smaSettings->set_rctrl_kd(value);
//         for (int i = 0; i < 4; i++)
//             bytes[i] = EEPROM.read(17+i);
//         smaSettings->set_default_setpoint(value);//defaultSetPoint    
//     }

    // Save current settings from nodeSettings to EEPROM
//     void saveCurrentSettings() {
//         const uint8_t* pptr = reinterpret_cast<const uint8_t*>(&smaSettings->get_rctrl_kp());
//         const uint8_t* iptr = reinterpret_cast<const uint8_t*>(&smaSettings->get_rctrl_ki());
//         const uint8_t* dptr = reinterpret_cast<const uint8_t*>(&smaSettings->get_rctrl_kd());
//         const uint8_t* defaultptr = reinterpret_cast<const uint8_t*>(&smaSettings->get_rctrl_kd());

//         for (int i = 0; i < 4; i++) {
//             EEPROM.update((2+i),pptr[i]);
//         }
//         for (int i = 0; i < 4; i++) {
//             EEPROM.update((7+i),iptr[i]);
//         }
//         for (int i = 0; i < 4; i++) {
//             EEPROM.update((12+i),dptr[i]);
//         }
//         for (int i = 0; i < 4; i++)
//         {
//             EEPROM.update((17+i),defaultptr[i]);
//         }
//     }

//     // Return a pointer to the current node settings
//     tfnode::SMAControllerSettings* getSettings() {
//         return smaSettings;
//     }

//     // Update the settings and save them to EEPROM
//     void setSettings(tfnode::SMAControllerSettings newSettings) {
//         *smaSettings = newSettings;
//         saveCurrentSettings();  // Automatically save to EEPROM after change
//     }
// };