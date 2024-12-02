/*=============================================================================
*
* ThermoFlex Node Muscle Controller Firmware
* Author: Mark Dannemiller
*
*=============================================================================*/

#include <Arduino.h>
#include "config.hpp"
#include "networking/Settings.h"
#include "globals.hpp"

// Create instances of network interfaces
SerialInterface* serialInterface;
// Assume CANInterface and SPIInterface are implemented similarly
CANInterface* canInterface;
// SPIInterface spiInterface;

TFNode* master_tfNode;
CommandProcessor* commandProcessor;

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("Setup starting");
    serialInterface = new SerialInterface;
    canInterface = new CANInterface();

    //addresses global
    NodeSettingsManager IDs;
    // Serial.println("Interfaces created");

    // Initialize node with its address
    // TODO this should be different for every device
    
    // TODO add a unique command for setting address and attach it to Settings
    NodeAddress nodeAddress;
    

    nodeAddress.idType = NodeAddress::IDType::NodeID;  // Default NodeID ID type
    nodeAddress.id = IDs.getNodeID();  // Node's unique ID specified in Config
    //Serial.println(nodeAddress.idType);
    for(int i = 0; i <3; i++) {
        Serial.print(nodeAddress.id[i]);
    }

    Serial.println();

    master_tfNode = new TFNode(nodeAddress);
    commandProcessor = new CommandProcessor(*master_tfNode);

    // Serial.println("Node and CommandProcessor created");

    // Add network interfaces to the command processor
    commandProcessor->addNetworkInterface(serialInterface);
    commandProcessor->addNetworkInterface(canInterface);
    // commandProcessor.addNetworkInterface(&spiInterface);

    master_tfNode->begin();

    //Serial.println(IDs.settings.get_can_id());

    // // Add debug actions below
    // master_tfNode->smaController0.CMD_setMode(tfnode::SMAControlMode::MODE_PERCENT);
    // master_tfNode->smaController1.CMD_setMode(tfnode::SMAControlMode::MODE_PERCENT);
    // master_tfNode->smaController0.CMD_setSetpoint(tfnode::SMAControlMode::MODE_PERCENT, 1.0);
    // master_tfNode->smaController1.CMD_setSetpoint(tfnode::SMAControlMode::MODE_PERCENT, 0.2);
    // master_tfNode->CMD_enableDevice(tfnode::Device::DEVICE_ALL);

    //master_tfNode->CMD_setStatusMode(tfnode::Device::DEVICE_ALL, tfnode::DeviceStatusMode::STATUS_DUMP_READABLE, false, serialInterface);
    //commandProcessor->testCANCommandPacket();
    // commandProcessor->testCANEnableCommandPacket();
    //IDs.readEEPROM();

    Serial.println("Setup complete");
    master_tfNode->init_finished(); // Turn on LED to indicate setup complete
}

void loop() {
    // Process incoming packets and commands
    commandProcessor->process();
    master_tfNode->update();

    // Periodically attempt reconnections
    /*static unsigned long lastAttemptTime = 0;
    if (millis() - lastAttemptTime > RECONNECTION_INTERVAL_MS) {
        for (auto iface : commandProcessor->getInterfaces()) {
            if (!iface->isConnected()) {
                iface->attemptConnection();  // TODO implement attemptConnection() and isConnected()
            }
        }
        lastAttemptTime = millis();
    }*/
}
