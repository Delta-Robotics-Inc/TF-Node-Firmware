#include "CANInterface.h"
#include "tfnode-messages.h"

void CANInterface::sendPacket(const Packet& packet) {
    tfnode::NodeSettings* nodeSettings;
    std::vector<uint8_t> rawData = packet.serialize();
    //construct CAN message
    //uint32_t const can_id = packet.senderId.id[packet.senderId.CANID]; this might be wrong
    CanMsg msg(CanStandardId(nodeSettings->can_id()), rawData.size(), rawData.data());
    //Begin sending a CAN packet
    CAN.write(msg);
}

void CANInterface::receiveData() {
    CanMsg msg;
    // Check if a CAN packet is available and append to rxBuffer
        while (CAN.available()) {
            msg = CAN.read();
            //the msg is an array so we must process it
            for (int i = 0; i < msg.data_length; i++) {
                uint8_t byte = msg.data[i];  // Access each byte individually
                rxBuffer.push_back(byte);    // Add each byte to the rxBuffer
            }
        }

        // Attempt to parse packets from rxBuffer (same as original)
        while (true) {
            if (rxBuffer.size() < 4) {
                // Not enough data for even the smallest packet
                break;
            }

            size_t index = 0;
            while (index < rxBuffer.size() && rxBuffer[index] != Packet::startByte) {
                index++;
            }

            if (index > 0) {
                rxBuffer.erase(rxBuffer.begin(), rxBuffer.begin() + index);
            }

            if (rxBuffer.size() < 4) {
                break;
            }

            uint16_t packetLength = (rxBuffer[1] << 8) | rxBuffer[2];
            size_t totalPacketSize = 1 + 2 + packetLength;

            if (rxBuffer.size() < totalPacketSize) {
                break;
            }

            std::vector<uint8_t> packetData(rxBuffer.begin(), rxBuffer.begin() + totalPacketSize);
            Packet packet;
            if (packet.parse(packetData)) {
                packetQueue.push(packet);
                rxBuffer.erase(rxBuffer.begin(), rxBuffer.begin() + totalPacketSize);
            } else {
                rxBuffer.erase(rxBuffer.begin());
            }
        }
    }


bool CANInterface::hasPacket() {
    return !packetQueue.empty();
}

Packet CANInterface::getNextPacket() {
    Packet packet = packetQueue.front();
    packetQueue.pop();
    return packet;
}

// Override method utilizing hasPacket() and getNextPacket()
bool CANInterface::receivePacket(Packet& packet) {
    // This method can be optional if using receiveData(), hasPacket(), and getNextPacket()
    if (hasPacket()) {
        packet = getNextPacket();
        return true;
    }
    return false;
}

std::string CANInterface::getName() const {
    return "CANInterface";
}