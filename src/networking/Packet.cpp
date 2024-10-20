// Packet.cpp

#include "Packet.h"
#include<Arduino.h>

Packet::Packet()
    : packetLength(0), checksum(0) {}

// Appears to be working
bool Packet::parse(const std::vector<uint8_t>& rawData) {
    if (rawData.size() < 13) {
        // Packet too short
        Serial.println("Error: Packet too short.");
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
        Serial.println("Error: Invalid Protocol version.");
        return false;
    }

    senderId.idType = (NodeAddress::IDType)rawData[index++];
    destinationId.idType = (NodeAddress::IDType)rawData[index++];

    size_t idLength = 3; // Assuming ID length is 3 bytes

    senderId.id.assign(rawData.begin() + index, rawData.begin() + index + idLength);
    index += idLength;

    destinationId.id.assign(rawData.begin() + index, rawData.begin() + index + idLength);
    index += idLength;

    // TODO verify this line
    size_t dataLength = packetLength - (1 + 1 + 1 + idLength + idLength + 1); // Exclude checksum
    if (index + dataLength + 1 > rawData.size()) {
        // Not enough data
        Serial.println("Error: Not enough data.");
        return false;
    }

    data.assign(rawData.begin() + index, rawData.begin() + index + dataLength);
    index += dataLength;

    checksum = rawData[index++];

    uint8_t calculated_checksum = calculateChecksum();

    // Validate checksum
    if (checksum != calculated_checksum) {
        // Checksum mismatch
        //#ifdef DEBUG
        Serial.print("Error: Invalid Checksum: ");
        Serial.print(checksum);
        Serial.print(" != ");
        Serial.println(calculated_checksum);
        //#endif
        return false;
    }

    // Ignore the rest of data

    return true;
}

std::vector<uint8_t> Packet::serialize() const {
    std::vector<uint8_t> rawData;
    String rawDataString;
    rawData.push_back(Packet::startByte);

    // // Debug rawData ==========================================================
    // rawDataString = "\nSerializing Start Byte: ";
    // for (auto byte : rawData) {
    //     rawDataString += String(byte, HEX) + " ";
    // }
    // Serial.println(rawDataString);
    // //=========================================================================

    // Placeholder for packet length
    rawData.push_back(0);
    rawData.push_back(0);

    // // Debug rawData ==========================================================
    // rawDataString = "Reserving Space For Packet Length: ";
    // for (auto byte : rawData) {
    //     rawDataString += String(byte, HEX) + " ";
    // }
    // Serial.println(rawDataString);
    // //=========================================================================

    rawData.push_back(Packet::protocolVersion);
    rawData.push_back(senderId.idType);
    rawData.push_back(destinationId.idType);

    // // Debug rawData ==========================================================
    // rawDataString = "Serializing Protocol Version, Sender ID Type, and Destination ID Type: ";
    // for (auto byte : rawData) {
    //     rawDataString += String(byte, HEX) + " ";
    // }
    // Serial.println(rawDataString);
    // //=========================================================================

    rawData.insert(rawData.end(), senderId.id.begin(), senderId.id.end());
    rawData.insert(rawData.end(), destinationId.id.begin(), destinationId.id.end());

    // // Debug rawData ==========================================================
    // rawDataString = "Serializing Sender and Destination IDs: ";
    // for (auto byte : rawData) {
    //     rawDataString += String(byte, HEX) + " ";
    // }
    // Serial.println(rawDataString);
    // //=========================================================================

    rawData.insert(rawData.end(), data.begin(), data.end());

    // // Debug rawData ==========================================================
    // rawDataString = "Serializing Packet Data/Message: ";
    // for (auto byte : rawData) {
    //     rawDataString += String(byte, HEX) + " ";
    // }
    // Serial.println(rawDataString);
    // //=========================================================================

    // Calculate packet length
    uint16_t length = packetLength; //rawData.size() - 1; //Exclude start byte
    rawData[1] = (length >> 8) & 0xFF;
    rawData[2] = length & 0xFF;

    // // Debug rawData ==========================================================
    // rawDataString = "Serializing Packet Length: ";
    // for (auto byte : rawData) {
    //     rawDataString += String(byte, HEX) + " ";
    // }
    // Serial.println(rawDataString);
    // //=========================================================================

    // Calculate checksum
    uint8_t checksum = calculateChecksum();
    rawData.push_back(checksum);

    // // Debug rawData ==========================================================
    // rawDataString = "Serializing Checksum: ";
    // for (auto byte : rawData) {
    //     rawDataString += String(byte, HEX) + " ";
    // }
    // Serial.println(rawDataString);
    // //=========================================================================

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
    sum ^= senderId.idType;
    sum ^= destinationId.idType;

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

// Returns a verbose string representation of packet and fields
String Packet::toString() const {
    String result;

    result += "Packet {\n";
    result += "  startByte: 0x" + String(startByte, HEX) + "\n";
    result += "  packetLength: " + String(packetLength) + "\n";
    result += "  protocolVersion: 0x" + String(protocolVersion, HEX) + "\n";
    result += "  senderIdType: " + String(senderId.idType == NodeAddress::NodeID ? "NodeID" : "CANID") + "\n";
    result += "  senderId: ";
    for (auto byte : senderId.id) {
        if (byte < 16) result += "0";
        result += String(byte, HEX) + " ";
    }
    result += "\n";
    result += "  destinationIdType: " + String(destinationId.idType == NodeAddress::NodeID ? "NodeID" : "CANID") + "\n";
    result += "  destinationId: ";
    for (auto byte : destinationId.id) {
        if (byte < 16) result += "0";
        result += String(byte, HEX) + " ";
    }
    result += "\n";
    result += "  data: ";
    for (auto byte : data) {
        if (byte < 16) result += "0";
        result += String(byte, HEX) + " ";
    }
    result += "\n";
    result += "  checksum: 0x";
    if (checksum < 16) result += "0";
    result += String(checksum, HEX) + "\n";
    result += "}";

    return result;
}