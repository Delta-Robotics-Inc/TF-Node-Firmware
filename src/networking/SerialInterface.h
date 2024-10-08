// SerialInterface.h

#ifndef SERIAL_INTERFACE_H
#define SERIAL_INTERFACE_H

#include "NetworkInterface.h"
#include <HardwareSerial.h>

class SerialInterface : public NetworkInterface {
public:
    SerialInterface(HardwareSerial& serialPort);

    void sendPacket(const Packet& packet) override;
    bool receivePacket(Packet& packet) override;
    std::string getName() const override { return "Serial"; }

private:
    HardwareSerial& serial;
};

#endif // SERIAL_INTERFACE_H
