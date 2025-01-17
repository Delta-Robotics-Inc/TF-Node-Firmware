// SerialInterface.h

#ifndef SERIAL_INTERFACE_H
#define SERIAL_INTERFACE_H

#include "NetworkInterface.h"
#include <vector>
#include <queue>

class SerialInterface : public NetworkInterface {
public:
    void sendPacket(const Packet& packet) override;
    void receiveData(); // Reads data and parses packets
    std::string getName() const override;

    bool isConnected() override { return true; }
    void attemptConnection() override {}

    // Serial interfaces has one of the following, CAN has one per device on the network (up to max devices)
    std::vector<uint8_t> packetData;
    uint16_t packetLength;
    ReceptionState state = ReceptionState::WAIT_FOR_START_BYTE;

    void parsePacket(int byte_from_packet);
};

#endif // SERIAL_INTERFACE_H
