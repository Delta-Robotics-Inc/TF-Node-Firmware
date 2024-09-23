#include "CommandProcessor.h"
#include "tf-node.h" // For access to controllers

void CommandProcessor::begin() {
    Serial.begin(115200);
    // Initialize CAN, SPI, etc.
}

void CommandProcessor::update() {
    processSerial();
    // processCAN();
    // processSPI();
}

void CommandProcessor::processSerial() {
    if (Serial.available()) {
        // Read and parse command
    }
}

void CommandProcessor::dispatchCommand(uint8_t deviceId, uint8_t functionCode, const uint8_t* params, size_t paramLength) {
    // Dispatch command to the appropriate device
    if (deviceId == 0) {
        // Send to SMA Controller 0
        // Assuming tfNode is accessible or singleton
        tfNode.smaController0.executeCommand(functionCode, params, paramLength);
    } else if (deviceId == 1) {
        // Send to SMA Controller 1
        tfNode.smaController1.executeCommand(functionCode, params, paramLength);
    } else {
        // Unknown device
        sendAcknowledgement(deviceId, 0xFF); // Error code
    }
}

void CommandProcessor::sendAcknowledgement(uint8_t deviceId, uint8_t statusCode) {
    // Send acknowledgement back over the same interface
}

