

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

enum class ReceptionState {
    WAIT_FOR_START_BYTE,
    READ_LENGTH,
    READ_PACKET
};

ReceptionState state = ReceptionState::WAIT_FOR_START_BYTE;
std::vector<uint8_t> packetData;
uint16_t packetLength = 0;

void SerialInterface::receiveData() {
    while (Serial.available()) {
        uint8_t byte = Serial.read();

        switch (state) {
            case ReceptionState::WAIT_FOR_START_BYTE:
                //Serial.println("Waiting for start byte");
                if (byte == Packet::startByte) {
                    packetData.clear();
                    packetData.push_back(byte);
                    state = ReceptionState::READ_LENGTH;
                }
                break;

            case ReceptionState::READ_LENGTH:
                //Serial.println("Reading length");
                packetData.push_back(byte);
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

                packetData.push_back(byte);
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
}
/*
void SerialInterface::receiveData() {

    bool sendDebug = Serial.available();  // Debug Received Bytes

    // Read available bytes and append to rxBuffer
    while (Serial.available()) {
        uint8_t byte = Serial.read();
        // TODO: Clear buffer if too large
        rxBuffer.push_back(byte);
    }

    // Debug Received Bytes
    if(sendDebug) {
        Serial.println("\nReceived Bytes: ");
        // Debug Received Bytes
        for(int i = 0; i < rxBuffer.size(); i++) {
            Serial.print(rxBuffer[i]);
            Serial.print(" ");
        }
    }

    // Attempt to parse packets from rxBuffer
    while (true) {
        if (rxBuffer.size() < 4) {
            // Not enough data for even the smallest packet
            break;
        }

        size_t index = 0;

        // Find the startByte
        while (index < rxBuffer.size() && rxBuffer[index] != Packet::startByte) {
            index++;
        }

        if (index > 0) {
            // Remove bytes before the startByte
            rxBuffer.erase(rxBuffer.begin(), rxBuffer.begin() + index);
        }

        if (rxBuffer.size() < 4) {
            // Not enough data to read packetLength
            Serial.println("Not enough data for packetLength");
            break;
        }

        // Read packetLength
        uint16_t packetLength = (rxBuffer[1] << 8) | rxBuffer[2];

        // Calculate total packet size
        size_t totalPacketSize = 1 + 2 + packetLength;

        if (rxBuffer.size() < totalPacketSize) {
            // Not enough data yet
            Serial.print("PacketLength: ");
            Serial.println(packetLength);
            Serial.println("Not enough data for full packet");
            break;
        }

        // Extract the packet bytes
        std::vector<uint8_t> packetData(rxBuffer.begin(), rxBuffer.begin() + totalPacketSize);

        // Attempt to parse the packet
        Packet packet;
        if (packet.parse(packetData)) {
            // Successfully parsed packet
            packetQueue.push(packet);
            // Remove parsed bytes
            Serial.println("Packet received");
            rxBuffer.erase(rxBuffer.begin(), rxBuffer.begin() + totalPacketSize);
        } else {
            // Invalid packet, discard the first byte and try again
            rxBuffer.erase(rxBuffer.begin());
            Serial.println("Invalid packet");
        }
    }
}*/

bool SerialInterface::hasPacket() {
    return !packetQueue.empty();
}

Packet SerialInterface::getNextPacket() {
    Packet packet = packetQueue.front();
    packetQueue.pop();
    return packet;
}

// Override method utilizing hasPacket() and getNextPacket()
bool SerialInterface::receivePacket(Packet& packet) {
    receiveData();
    // This method can be optional if using receiveData(), hasPacket(), and getNextPacket()
    if (hasPacket()) {
        packet = getNextPacket();
        return true;
    }
    return false;
}

std::string SerialInterface::getName() const {
    return "SerialInterface";
}
