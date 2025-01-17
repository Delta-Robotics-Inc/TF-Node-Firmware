// NetworkInterface.h

#ifndef NETWORK_INTERFACE_H
#define NETWORK_INTERFACE_H

#include "Packet.h"
#include <vector>
#include <queue>

enum class ReceptionState
{
    WAIT_FOR_START_BYTE,
    READ_LENGTH,
    READ_PACKET
};

class NetworkInterface {
public:
    virtual ~NetworkInterface() {}
    virtual void sendPacket(const Packet& packet) = 0;
    virtual void receiveData() = 0; // Reads data and parses packets
    virtual std::string getName() const = 0;

    // TODO implement these on Serial and CAN interfaces
    virtual bool isConnected() = 0;
    virtual void attemptConnection() = 0;

    void parsePacket(int byte_from_packet, std::vector<uint8_t> *data, ReceptionState *msg_state, uint16_t *packetLength);
    bool receivePacket(Packet& packet);
    bool hasPacket();
    Packet getNextPacket();

    std::queue<Packet> packetQueue;
};

#endif // NETWORK_INTERFACE_H
