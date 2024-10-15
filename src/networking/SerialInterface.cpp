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

void SerialInterface::receiveData() {
    // Read available bytes and append to rxBuffer
    while (Serial.available()) {
        uint8_t byte = Serial.read();
        rxBuffer.push_back(byte);
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
            break;
        }

        // Read packetLength
        uint16_t packetLength = (rxBuffer[1] << 8) | rxBuffer[2];

        // Calculate total packet size
        size_t totalPacketSize = 1 + 2 + packetLength;

        if (rxBuffer.size() < totalPacketSize) {
            // Not enough data yet
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
            //Serial.println("Packet received");
            rxBuffer.erase(rxBuffer.begin(), rxBuffer.begin() + totalPacketSize);
        } else {
            // Invalid packet, discard the first byte and try again
            rxBuffer.erase(rxBuffer.begin());
        }
    }
}

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
