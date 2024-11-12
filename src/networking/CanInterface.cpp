#include "CANInterface.h"

CANInterface::CANInterface()
{
    // Serial.println("Starting CAN...");
    if (!CAN.begin(CanBitRate::BR_250k))
    {
        // Serial.println("Failed to start CAN");
    }
}

void CANInterface::sendPacket(const Packet &packet)
{
    // Serial.println("Serializing packet for CAN");
    std::vector<uint8_t> rawData = packet.serialize();
    static uint32_t const CAN_ID = 0x53;

    // Send the rawData in chunks of 8 bytes
    for (size_t i = 0; i < rawData.size(); i += 8)
    {
        uint8_t chunkSize = std::min(static_cast<size_t>(8), rawData.size() - i);

        // Debug Sent CANMsg Data======================================
        //  Serial.println("Sending CAN message: ");
        //  for(int j = 0; j < chunkSize; j++) {
        //      Serial.print(rawData[i + j]);
        //      Serial.print(" ");
        //  }
        //  Serial.println();
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
        // Serial.print("Received CAN Message: ");
        // for (int i = 0; i < msg.data_length; i++)
        // {
        //     Serial.print(msg.data[i]);
        //     Serial.print(" ");
        // }
        // Serial.println();
        //======================================================================================================
        // Find or create a buffer for this CAN ID
        int bufferIndex = findOrCreateBuffer(msg.id);

        if (bufferIndex < 0) {
            // No buffer available, skip this message
            continue;
        }
        
        MessageBuffer& buffer = messageBuffers[bufferIndex];

        for (int i = 0; i < msg.data_length; i++)
        {
            
          //  uint8_t byte = msg.data[i]; // Access each byte individually
          //  parsePacket(byte);

            uint8_t byte = msg.data[i];//Access each byte individually
            if (state == ReceptionState::WAIT_FOR_START_BYTE)
            {
                parsePacket(byte);
                continue;;
            }
            
            if (state == ReceptionState::READ_LENGTH) {
                buffer.expectedLength = byte;
                buffer.lengthReceived = true;
                parsePacket(byte);
                continue;
            }


            // Add the byte to the buffer
            buffer.data.push_back(byte);
            
            // Check if we've received all expected bytes
            if (buffer.lengthReceived && buffer.data.size() >= buffer.expectedLength)
            {
                processBuffer(bufferIndex);
            }

    }
}
}

void CANInterface::processBuffer(int bufferIndex)
{
    MessageBuffer& buffer = messageBuffers[bufferIndex];
    
    // Process the complete message
    for (size_t i = 0; i < buffer.expectedLength; i++)
    {
        parsePacket(buffer.data[i]);
    }
    
    // Reset the buffer
    buffer.inUse = false;
    buffer.data.clear();
    buffer.lengthReceived = false;
    buffer.expectedLength = 0;
}


std::string CANInterface::getName() const
{
    return "CANInterface";
}