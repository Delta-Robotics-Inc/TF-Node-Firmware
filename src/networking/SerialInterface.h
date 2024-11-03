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

};

#endif // SERIAL_INTERFACE_H
