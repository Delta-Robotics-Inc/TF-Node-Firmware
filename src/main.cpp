/*=============================================================================
*
* ThermoFlex Node Muscle Controller Firmware
* Author: Mark Dannemiller
*
*=============================================================================*/

#include <Arduino.h>
#include "config.hpp"
#include "TFNode.hpp"
#include "networking/SerialInterface.h"
#include "networking/CommandProcessor.hpp"

// Create instances of network interfaces
SerialInterface serialInterface();

// Assume CANInterface and SPIInterface are implemented similarly
// CANInterface canInterface;
// SPIInterface spiInterface;

TFNode* master_tfNode;
CommandProcessor* commandProcessor;

void setup() {
    Serial.begin(115200);
    

    // Initialize node with its address
    // TODO this should be different for every device
    // TODO add a unique command for setting address and attach it to Settings
    NodeAddress nodeAddress;
    nodeAddress.idType = NodeAddress::IDType::NodeID;  // Default NodeID ID type
    nodeAddress.id = {0x01, 0x02, 0x03};               // Node's unique ID

    master_tfNode = new TFNode(nodeAddress);
    commandProcessor = new CommandProcessor(master_tfNode);
    master_tfNode->setCommandProcessor(commandProcessor);

    // Add network interfaces to the command processor
    commandProcessor->addNetworkInterface(&serialInterface);
    // commandProcessor.addNetworkInterface(&canInterface);
    // commandProcessor.addNetworkInterface(&spiInterface);

    master_tfNode->begin();
}

void loop() {
    // Process incoming packets and commands
    commandProcessor->process();
    master_tfNode->update();

    // Periodically attempt reconnections
    static unsigned long lastAttemptTime = 0;
    if (millis() - lastAttemptTime > RECONNECTION_INTERVAL_MS) {
        for (auto iface : commandProcessor->getInterfaces()) {
            if (!iface->isConnected()) {
                iface->attemptConnection();  // TODO implement attemptConnection() and isConnected()
            }
        }
        lastAttemptTime = millis();
    }
}
