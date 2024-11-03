// Packet.h

#ifndef PACKET_H
#define PACKET_H

#include <vector>
#include <cstdint>
#include <string>
#include <Arduino.h>
#include "tfnode-messages.h" // Generated Protobuf header

// [Start Byte][Packet Length][Protocol Version][Sender ID Type][Destination ID Type][Sender ID][Destination ID][Data][Checksum]

struct NodeAddress {

    enum IDType { NodeID, CANID };

    IDType idType;
    std::vector<uint8_t> id;

    bool operator==(const NodeAddress& other) const {
        return idType == other.idType && id == other.id;
    }
};

class Packet {
public:
    // Constructor
    Packet();

    bool parse(const std::vector<uint8_t>& rawData);  // Parse raw data into a packet
    std::vector<uint8_t> serialize() const;  // Serialize the packet into raw data
    String toString() const;  // Convert packet to string representation
    bool isValid() const;   // Check if the packet is valid
    bool isBroadcast() const;
    bool isForThisNode(const NodeAddress &nodeAddress) const; // Check if the packet is intended for this node

    // Packet fields
    static constexpr uint8_t startByte = 0x7E;
    uint16_t packetLength;
    static constexpr uint8_t protocolVersion = 0x01;
    NodeAddress senderId;
    NodeAddress destinationId;
    std::vector<uint8_t> data; // Serialized Protobuf message
    uint8_t checksum;

    // Helper methods
    uint8_t calculateChecksum() const;
    uint16_t calculatePacketLength() const;

};

#endif // PACKET_H
