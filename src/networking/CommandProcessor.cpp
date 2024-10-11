// CommandProcessor.cpp

#include "CommandProcessor.hpp"
#include "SMAController.hpp"
#include "ReadBuffer.h"
#include "WriteBuffer.h"

CommandProcessor::CommandProcessor(TFNode& node)
    : node(node) {}

void CommandProcessor::addNetworkInterface(NetworkInterface* netInterface) {
    interfaces.push_back(netInterface);
}

void CommandProcessor::process() {
    for (auto iface : interfaces) {
        Packet packet;

        if (iface->receivePacket(packet)) {
            handlePacket(packet, iface);
        }
    }
}

void CommandProcessor::sendResponse(const tfnode::Response& response, NetworkInterface* iface) {
    uint8_t bufferData[256]; // Adjust size as needed
    WriteBuffer buffer(bufferData, sizeof(bufferData));

    ::EmbeddedProto::Error err = response.serialize(buffer);
    if (err == ::EmbeddedProto::Error::NO_ERRORS) {
        // Create a Packet with the serialized data
        Packet packet;

        // Set sender and destination IDs
        packet.senderId = node.getAddress(); // Node's address
        packet.senderId.idType = node.getAddress().idType;

        // Set destination ID as needed
        // For simplicity, set destination ID to zero
        packet.destinationId.id = {0x0, 0x0, 0x0};
        packet.destinationId.idType = NodeAddress::IDType::NodeID;
        packet.destinationId.id.clear();

        // Set data
        packet.data.assign(bufferData, bufferData + buffer.get_size());

        // Calculate packet length and checksum
        packet.packetLength = packet.calculatePacketLength();
        packet.checksum = packet.calculateChecksum();

        // Send the packet
        iface->sendPacket(packet);
    } else {
        // Handle serialization error
    }
}


void CommandProcessor::handlePacket(Packet& packet, NetworkInterface* sourceInterface) {
    if (!packet.isValid()) {
        // Invalid packet, discard or log error
        return;
    }

    if (packet.isForThisNode(node.getAddress())) {
        // Packet is intended for this node
        executeCommand(packet, sourceInterface);
    } else {
        // Packet is not for this node, forward it
        forwardPacket(packet, sourceInterface);
    }
}

void CommandProcessor::executeCommand(const Packet& packet, NetworkInterface* sourceInterface) {
    // Create a ReadBuffer from the packet data
    ReadBuffer buffer(packet.data.data(), packet.data.size());

    // Deserialize the NodeCommand message
    tfnode::NodeCommand command;
    ::EmbeddedProto::Error err = command.deserialize(buffer);

    if (err != ::EmbeddedProto::Error::NO_ERRORS) {
        // Failed to parse command
        return;
    }
    // Determine which command is set using the oneof field
    switch (command.get_which_command()) {
        case tfnode::NodeCommand::FieldNumber::RESET:
            node.CMD_resetDevice(command.reset().device());
            break;
        case tfnode::NodeCommand::FieldNumber::ENABLE:
            node.CMD_enableDevice(command.enable().device());
            break;
        case tfnode::NodeCommand::FieldNumber::DISABLE:
            node.CMD_enableDevice(command.disable().device());
            break;
        case tfnode::NodeCommand::FieldNumber::SET_MODE:
            // TODO implement
            switch(command.set_mode().device()) {
                case(tfnode::Device::DEVICE_PORT1):
                    node.smaControllers[0].CMD_setMode(command.set_mode().mode());
                break;
                case(tfnode::Device::DEVICE_PORT2):

                break;
                case(tfnode::Device::DEVICE_ALL):
                case(tfnode::Device::DEVICE_PORTALL):
                    for (auto& controller : node.smaControllers) {
                        controller.CMD_setMode(command.set_mode().mode());
                    }
                break;
                default:
                    // TODO Throw error
                break;
            }
            break;
        case tfnode::NodeCommand::FieldNumber::SET_SETPOINT:
            // TODO implement
            break;
        case tfnode::NodeCommand::FieldNumber::SILENCE_NODE:
            // TODO implement
            break;
        case tfnode::NodeCommand::FieldNumber::CONFIGURE_SETTINGS:
            // TODO implement
            break;
        case tfnode::NodeCommand::FieldNumber::STATUS:
            node.CMD_setStatusMode(
                command.status().device(),
                command.status().mode(),
                sourceInterface // Pass the interface
            );
            break;
        // Handle other commands similarly
        default:
            // Unknown command
            break;
    }
}

void CommandProcessor::forwardPacket(const Packet& packet, NetworkInterface* excludeInterface) {
    for (auto iface : interfaces) {
        if (iface != excludeInterface) {
            // If Network ID Type and Network ID are specified, forward only to matching interface
            // Implement logic based on your network addressing
            iface->sendPacket(packet);
        }
    }
}
