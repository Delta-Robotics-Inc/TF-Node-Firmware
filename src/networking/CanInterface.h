#ifndef CAN_INTERFACE_H
#define CAN_INTERFACE_H

#include "NetworkInterface.h"
#include <vector>
#include <queue>
#include <Arduino_CAN.h>  // Include the CAN library


class CANInterface : public NetworkInterface {
public:
    CANInterface();
    void sendPacket(const Packet& packet) override;
    bool receivePacket(Packet& packet) override;
    std::string getName() const override;

    bool isConnected() override { return true; }  // Placeholder, adjust based on actual connection checks
    void attemptConnection() override {}

    void receiveData(); // Reads data and parses packets
    bool hasPacket();
    Packet getNextPacket();

private:
    std::vector<uint8_t> rxBuffer;
    std::queue<Packet> packetQueue;
    
    ReceptionState state = ReceptionState::WAIT_FOR_START_BYTE;
    std::vector<uint8_t> packetData;
    uint16_t packetLength = 0;
};

#endif // CAN_INTERFACE_H
