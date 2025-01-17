#include "CanInterface.h"

CANInterface::CANInterface()
{
    // Serial.println("Starting CAN...");
    if (!CAN.begin(CanBitRate::BR_250k))
    {
        // Serial.println("Failed to start CAN");
    }
}


int CANInterface::findOrCreateBuffer(uint32_t canId) {
    // First, try to find an existing buffer for this CAN ID
    for (int i = 0; i < MAX_CAN_BUFFERS; i++) {
        if (messageBuffers[i].inUse && messageBuffers[i].canId == canId) {
            return i;
        }
    }
    
    // If not found, look for an unused buffer
    for (int i = 0; i < MAX_CAN_BUFFERS; i++) {
        if (!messageBuffers[i].inUse) {
            messageBuffers[i].inUse = true;
            messageBuffers[i].canId = canId;
            messageBuffers[i].data.clear();
            messageBuffers[i].expectedLength = 0;
            messageBuffers[i].lengthReceived = false;
            return i;
        }
    }
    
    // If no buffer is available, return -1 (error condition)
    return -1;
}


void CANInterface::receiveData()
{
    CanMsg msg;
    // Check if a CAN packet is available
    while (CAN.available())
    {
        msg = CAN.read();
        // Debug================================================================================================
        Serial.print("Received CAN Message: ");
        Serial.print(msg.id);
        Serial.print(" ");
        for (int i = 0; i < msg.data_length; i++)
        {
            Serial.print(msg.data[i]);
            Serial.print(" ");
        }
        Serial.println();
        //======================================================================================================
        // Find or create a buffer for this CAN ID
        int bufferIndex = findOrCreateBuffer(msg.id);

        if (bufferIndex < 0) {
            // No buffer available, skip this message
            Serial.println("No buffer available for CAN");
            continue;
        }
        
        MessageBuffer& buffer = messageBuffers[bufferIndex];

        for (int i = 0; i < msg.data_length; i++)
        {
            //  uint8_t byte = msg.data[i]; // Access each byte individually
            //  parsePacket(byte);
            uint8_t byte = msg.data[i];//Access each byte individually
            parsePacket(byte, &buffer.data, &buffer.msg_buffer_state, &buffer.expectedLength);
        }
    }
}


std::string CANInterface::getName() const
{
    return "CANInterface";
}

void CANInterface::parsePacket(int byte_from_packet, std::vector<uint8_t> *data, ReceptionState *msg_state, uint16_t *packetLength) {
    switch (*msg_state) {
        case ReceptionState::WAIT_FOR_START_BYTE:
            //Serial.println("Waiting for start byte");
            if (byte_from_packet == Packet::startByte) {
                data->clear();
                data->push_back(byte_from_packet);
                *msg_state = ReceptionState::READ_LENGTH;
                Serial.println("Reading length...");
            }
            break;

        case ReceptionState::READ_LENGTH:
            //Serial.println("Reading length");
            data->push_back(byte_from_packet);
            if (data->size() == 3) { // Start byte + 2 length bytes
                *packetLength = (data->at(1) << 8) | data->at(2);
                *msg_state = ReceptionState::READ_PACKET;
                Serial.println("Reading Packet...");
            }
            break;

        case ReceptionState::READ_PACKET:
            // Serial.print("Reading packet: [");
            // Serial.print(packetData.size() - 3);
            // Serial.print('/');
            // Serial.print(packetLength);
            // Serial.println(']');

            data->push_back(byte_from_packet);
            if (data->size() == 3 + *packetLength) { // Start byte + 2 length bytes + packet length
                Packet packet;
                if (packet.parse(*data)) {
                    packetQueue.push(packet);
                    Serial.println("Packet received");
                } else {
                    Serial.println("Invalid packet");
                }
                *msg_state = ReceptionState::WAIT_FOR_START_BYTE;
                Serial.println("Packet Parsed...");
            }
            else if (data->size() > 3 + *packetLength) {
                Serial.println("Error: Packet size mismatch. Resetting...");
                data->clear();
                *msg_state = ReceptionState::WAIT_FOR_START_BYTE;
            }
            break;
    }

}

void CANInterface::sendPacket(const Packet &packet)
{
    // Serial.println("Serializing packet for CAN");
    std::vector<uint8_t> rawData = packet.serialize();
    //static uint32_t const CAN_ID = CAN_Stuff.get_can_id();
    static uint32_t const CAN_ID = 0x11;

    // Send the rawData in chunks of 8 bytes
    for (size_t i = 0; i < rawData.size(); i += 8)
    {
        uint8_t chunkSize = std::min(static_cast<size_t>(8), rawData.size() - i);

        // Debug Sent CANMsg Data======================================
         Serial.println("Sending CAN message: ");
         for(int j = 0; j < chunkSize; j++) {
             Serial.print(rawData[i + j]);
             Serial.print(" ");
         }
         Serial.println();
        //============================================================

        CanMsg msg(CanStandardId(CAN_ID), chunkSize, rawData.data() + i);
        if (int const rc = CAN.write(msg); rc < 0)
        {
            // TODO implement error
            //Serial.print  ("CAN.write(...) failed with error code ");
            //Serial.println(rc);
        }
        //TODO determine better solution then delay
       delay(5);
    }
}