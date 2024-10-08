// SerialInterface.cpp

#include "SerialInterface.h"

SerialInterface::SerialInterface(HardwareSerial& serialPort)
    : serial(serialPort) {}

void SerialInterface::sendPacket(const Packet& packet) {
    std::vector<uint8_t> rawData = packet.serialize();
    serial.write(rawData.data(), rawData.size());
}

bool SerialInterface::receivePacket(Packet& packet) {
    if (serial.available()) {
        // Read data from serial and attempt to parse a packet
        // Implement buffering and parsing logic as needed
        // For simplicity, assume we read a complete packet into rawData
        std::vector<uint8_t> rawData;
        while (serial.available()) {
            rawData.push_back(serial.read());
        }
        return packet.parse(rawData);
    }
    return false;
}
