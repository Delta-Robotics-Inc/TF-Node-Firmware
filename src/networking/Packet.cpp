// Packet.cpp

#include "Packet.h"

Packet::Packet()
    : packetLength(0), senderIdType(0),
      destinationIdType(0), checksum(0) {}

bool Packet::parse(const std::vector<uint8_t>& rawData) {
    if (rawData.size() < 13) {
        // Packet too short
        return false;
    }

    size_t index = 0;

    uint8_t incoming_startByte = rawData[index++];
    if (incoming_startByte != startByte) {
        // Invalid start byte
        return false;
    }

    packetLength = (rawData[index++] << 8);
    packetLength |= rawData[index++];

    uint8_t incoming_protocolVersion = rawData[index++];
    if(incoming_protocolVersion != protocolVersion) {
        // Throw error for incompatible version
        // FUTURE implement all acceptable versions
        return false;
    }

    senderIdType = rawData[index++];
    destinationIdType = rawData[index++];

    size_t idLength = 3; // Assuming ID length is 3 bytes

    senderId.idType = senderIdType;
    senderId.id.assign(rawData.begin() + index, rawData.begin() + index + idLength);
    index += idLength;

    destinationId.idType = destinationIdType;
    destinationId.id.assign(rawData.begin() + index, rawData.begin() + index + idLength);
    index += idLength;

    size_t dataLength = packetLength - (1 + 1 + 1 + idLength + idLength + 1); // Exclude checksum
    if (index + dataLength + 1 > rawData.size()) {
        // Not enough data
        return false;
    }

    data.assign(rawData.begin() + index, rawData.begin() + index + dataLength);
    index += dataLength;

    checksum = rawData[index++];

    // Validate checksum
    if (checksum != calculateChecksum()) {
        // Checksum mismatch
        return false;
    }

    // Ignore the rest of data

    return true;
}

std::vector<uint8_t> Packet::serialize() const {
    std::vector<uint8_t> rawData;
    rawData.push_back(startByte);

    // Placeholder for packet length
    rawData.push_back(0);
    rawData.push_back(0);

    rawData.push_back(protocolVersion);
    rawData.push_back(senderIdType);
    rawData.push_back(destinationIdType);

    rawData.insert(rawData.end(), senderId.id.begin(), senderId.id.end());
    rawData.insert(rawData.end(), destinationId.id.begin(), destinationId.id.end());

    rawData.insert(rawData.end(), data.begin(), data.end());

    // Calculate packet length
    uint16_t length = rawData.size() - 1; // Exclude start byte
    rawData[1] = (length >> 8) & 0xFF;
    rawData[2] = length & 0xFF;

    // Calculate checksum
    uint8_t checksum = calculateChecksum();
    rawData.push_back(checksum);

    rawData.push_back(endByte);

    return rawData;
}

bool Packet::isValid() const {
    // Additional validation if needed
    return true;
}

bool Packet::isForThisNode(const NodeAddress& nodeAddress) const {
    return destinationId == nodeAddress;
}

uint8_t Packet::calculateChecksum() const {
    // Simple checksum calculation (e.g., XOR of all bytes except start and end bytes)
    uint8_t sum = 0;
    sum ^= protocolVersion;
    sum ^= senderIdType;
    sum ^= destinationIdType;

    for (auto byte : senderId.id) {
        sum ^= byte;
    }

    for (auto byte : destinationId.id) {
        sum ^= byte;
    }

    for (auto byte : data) {
        sum ^= byte;
    }

    return sum;
}

uint16_t Packet::calculatePacketLength() const {
    // Packet length includes:
    // - protocolVersion (1 byte)
    // - senderIdType (1 byte)
    // - destinationIdType (1 byte)
    // - senderId.id (idLength bytes)
    // - destinationId.id (idLength bytes)
    // - data (variable length)
    // - checksum (1 byte)

    size_t protocolVersionSize = 1;
    size_t senderIdTypeSize = 1;
    size_t destinationIdTypeSize = 1;
    size_t idLength = senderId.id.size(); // Assuming sender and destination IDs are the same length
    size_t checksumSize = 1;

    size_t packetLength = protocolVersionSize
                        + senderIdTypeSize
                        + destinationIdTypeSize
                        + idLength // senderId.id
                        + idLength // destinationId.id
                        + data.size()
                        + checksumSize;

    return static_cast<uint16_t>(packetLength);
}
