#ifndef CAN_INTERFACE_H
#define CAN_INTERFACE_H

#include "NetworkInterface.h"
#include <vector>
#include <queue>
#include <Arduino_CAN.h>//CAN library
#include "tfnode-messages.h"

// Maximum number of different CAN IDs we'll handle simultaneously
#define MAX_CAN_BUFFERS 16

struct MessageBuffer {
    std::vector<uint8_t> data;
    uint32_t canId;
    uint16_t expectedLength;
    bool lengthReceived;
    bool inUse;
    ReceptionState msg_buffer_state = ReceptionState::WAIT_FOR_START_BYTE; // Used to track the incoming bytes
    
    MessageBuffer() : canId(0), expectedLength(0), lengthReceived(false), inUse(false) {}
};


class CANInterface : public NetworkInterface {
public:
    tfnode::NodeSettings CAN_Stuff;

    void parsePacket(int byte_from_packet, std::vector<uint8_t> *data, ReceptionState *msg_state, uint16_t *packetLength);

    CANInterface();
    void sendPacket(const Packet& packet) override;
    void receiveData() override; // Reads data and parses packets
    std::string getName() const override;

    bool isConnected() override { return true; }  // Placeholder, adjust based on actual connection checks
    void attemptConnection() override {}



private:
    MessageBuffer messageBuffers[MAX_CAN_BUFFERS];
    void processBuffer(int bufferIndex);
    int findOrCreateBuffer(uint32_t canId);

};

#endif // CAN_INTERFACE_H
