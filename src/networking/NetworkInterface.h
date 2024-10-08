// NetworkInterface.h

#ifndef NETWORK_INTERFACE_H
#define NETWORK_INTERFACE_H

#include "Packet.h"

class NetworkInterface {
public:
    virtual ~NetworkInterface() {}
    virtual void sendPacket(const Packet& packet) = 0;
    virtual bool receivePacket(Packet& packet) = 0;
    virtual std::string getName() const = 0;
};

#endif // NETWORK_INTERFACE_H
