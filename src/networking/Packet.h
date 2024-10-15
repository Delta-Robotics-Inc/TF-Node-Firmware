// Packet.h

#ifndef PACKET_H
#define PACKET_H

#include <vector>
#include <cstdint>
#include <string>
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

    // Parse raw data into a packet
    bool parse(const std::vector<uint8_t>& rawData);

    // Serialize the packet into raw data
    std::vector<uint8_t> serialize() const;

    // Check if the packet is valid
    bool isValid() const;

    // Check if the packet is intended for this node
    bool isForThisNode(const NodeAddress& nodeAddress) const;

    // Packet fields
    static constexpr uint8_t startByte = 0x7E;
    static constexpr uint8_t protocolVersion = 0x01;
    uint16_t packetLength;
    NodeAddress senderId;
    NodeAddress destinationId;
    std::vector<uint8_t> data; // Serialized Protobuf message
    uint8_t checksum;

    // Helper methods
    uint8_t calculateChecksum() const;
    uint16_t calculatePacketLength() const;

};

#endif // PACKET_H
