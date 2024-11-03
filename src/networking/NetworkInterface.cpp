#include "NetworkInterface.h"

void NetworkInterface::parsePacket(int byte_from_packet){
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

bool NetworkInterface::hasPacket() {
    return !packetQueue.empty();
}

Packet NetworkInterface::getNextPacket() {
    Packet packet = packetQueue.front();
    packetQueue.pop();
    return packet;
}

// Override method utilizing hasPacket() and getNextPacket()
bool NetworkInterface::receivePacket(Packet& packet) {
    receiveData();
    // This method can be optional if using receiveData(), 
    //hasPacket(), and getNextPacket()
    if (hasPacket()) {
        packet = getNextPacket();
        return true;
    }
    return false;
}