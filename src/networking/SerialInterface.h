// SerialInterface.h

#ifndef SERIAL_INTERFACE_H
#define SERIAL_INTERFACE_H

#include "NetworkInterface.h"
#include <vector>
#include <queue>

class SerialInterface : public NetworkInterface {
public:
    void sendPacket(const Packet& packet) override;
    bool receivePacket(Packet& packet) override;
    std::string getName() const override;

    bool isConnected() override { return true; }
    void attemptConnection() override {}

    void receiveData(); // Reads data and parses packets
    bool hasPacket();
    Packet getNextPacket();

private:
    std::vector<uint8_t> rxBuffer;
    std::queue<Packet> packetQueue;
};

#endif // SERIAL_INTERFACE_H
