/*=============================================================================
*
* ThermoFlex Node Muscle Controller Firmware
* Author: Mark Dannemiller
*
*=============================================================================*/

#define DEBUG  // Define if wanting serial debug statements

#include <Arduino.h>
#include "config.hpp"
#include "TFNode.hpp"
#include "networking/SerialInterface.h"
#include "networking/CanInterface.h"
#include "SMAController.hpp"


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
    //Serial.println("Serial begin at 115200");
    serialInterface = new SerialInterface;
    canInterface = new CANInterface();

    // Initialize node with its address
    // TODO this should be different for every device
    // TODO add a unique command for setting address and attach it to Settings
    NodeAddress nodeAddress;
    nodeAddress.idType = NodeAddress::IDType::NodeID;  // Default NodeID ID type
    nodeAddress.id = NODE_ID;  // Node's unique ID specified in Config

    master_tfNode = new TFNode(nodeAddress);
    commandProcessor = new CommandProcessor(*master_tfNode);
    master_tfNode->setCommandProcessor(commandProcessor);

    // Add network interfaces to the command processor
    commandProcessor->addNetworkInterface(serialInterface);
    commandProcessor->addNetworkInterface(canInterface);
    // commandProcessor.addNetworkInterface(&spiInterface);

    master_tfNode->begin();


    // Add debug actions below
    master_tfNode->smaControllers[0].CMD_setMode(tfnode::SMAControlMode::MODE_PERCENT);
    master_tfNode->smaControllers[1].CMD_setMode(tfnode::SMAControlMode::MODE_PERCENT);
    master_tfNode->smaControllers[0].CMD_setSetpoint(tfnode::SMAControlMode::MODE_PERCENT, 1.0);
    master_tfNode->smaControllers[1].CMD_setSetpoint(tfnode::SMAControlMode::MODE_PERCENT, 1.0);
    master_tfNode->CMD_enableDevice(tfnode::Device::DEVICE_PORT1);
    // master_tfNode->CMD_setStatusMode(tfnode::Device::DEVICE_NODE, tfnode::DeviceStatusMode::STATUS_COMPACT, &serialInterface);
    //commandProcessor->testCANCommandPacket();
    commandProcessor->testCANEnableCommandPacket();
    //Serial.println("Setup complete");
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
