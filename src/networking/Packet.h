// Packet.h

#ifndef PACKET_H
#define PACKET_H

#include <vector>
#include <cstdint>
#include <string>
#include "tfnode-messages.h" // Generated Protobuf header

struct NodeAddress {
    uint8_t idType;
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
    uint8_t startByte;
    uint16_t packetLength;
    uint8_t protocolVersion;
    uint8_t senderIdType;
    uint8_t destinationIdType;
    NodeAddress senderId;
    NodeAddress destinationId;
    std::vector<uint8_t> data; // Serialized Protobuf message
    uint8_t checksum;
    uint8_t endByte;

private:
    // Helper methods
    uint8_t calculateChecksum() const;
};

#endif // PACKET_H
