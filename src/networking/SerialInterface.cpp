

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

void SerialInterface::parsePacket(int byte_from_packet){
    switch (state) {
        case ReceptionState::WAIT_FOR_START_BYTE:
            //Serial.println("Waiting for start byte");
            if (byte_from_packet == Packet::startByte) {
                packetData.clear();
                packetData.push_back(byte_from_packet);
                state = ReceptionState::READ_LENGTH;
            }
            break;

        case ReceptionState::READ_LENGTH:
            //Serial.println("Reading length");
            packetData.push_back(byte_from_packet);
            if (packetData.size() == 3) { // Start byte + 2 length bytes
                packetLength = (packetData[1] << 8) | packetData[2];
                state = ReceptionState::READ_PACKET;
            }
            break;

        case ReceptionState::READ_PACKET:
            // Serial.print("Reading packet: [");
            // Serial.print(packetData.size() - 3);
            // Serial.print('/');
            // Serial.print(packetLength);
            // Serial.println(']');

            packetData.push_back(byte_from_packet);
            if (packetData.size() == 3 + packetLength) { // Start byte + 2 length bytes + packet length
                Packet packet;
                if (packet.parse(packetData)) {
                    packetQueue.push(packet);
                    // Serial.println("Packet received");
                } else {
                    // Serial.println("Invalid packet");
                }
                state = ReceptionState::WAIT_FOR_START_BYTE;
            }
            break;
    }

}