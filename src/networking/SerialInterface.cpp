

/**
 * @file SerialInterface.cpp
 * @brief Implementation of the SerialInterface class for handling serial communication.
 *
 * This file contains the implementation of the SerialInterface class, which provides
 * methods for sending and receiving packets over a serial interface using the Arduino framework.
 * The class handles serialization and deserialization of packets, as well as buffering and parsing
 * incoming data.
 *
 * The main functionalities include:
 * - Sending packets via the `sendPacket` method.
 * - Receiving and buffering data via the `receiveData` method.
 * - Parsing buffered data into packets and managing a queue of received packets.
 * - Checking for available packets and retrieving them using `hasPacket` and `getNextPacket` methods.
 * - An optional method `receivePacket` that combines receiving data and retrieving the next packet.
 *
 * The class also provides a method to get the name of the interface.
 *
 * Dependencies:
 * - Arduino.h: For serial communication functions.
 * - SerialInterface.h: Header file for the SerialInterface class.
 *
 * @note This implementation assumes the existence of a Packet class with methods for serialization
 *       and deserialization.
 */
#include "SerialInterface.h"
#include "Arduino.h"

void SerialInterface::sendPacket(const Packet& packet) {
    std::vector<uint8_t> rawData = packet.serialize();
    Serial.write(rawData.data(), rawData.size());

    // Debug
    /*for(int i = 0; i < rawData.size(); i++) {
        Serial.print(rawData[i]);
        Serial.print(" ");
    }*/
}

// TODO remove boilerplate code and add repeat functions to NetworkInterface
void SerialInterface::receiveData() {
    while (Serial.available()) {
        uint8_t byte = Serial.read();
        parsePacket(byte);

    }
}

std::string SerialInterface::getName() const {
    return "SerialInterface";
}
