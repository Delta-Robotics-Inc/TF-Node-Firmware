#include "NetworkInterface.h"

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