// CommandProcessor.cpp

#include "CommandProcessor.hpp"
#include "tfnode-messages.h" // Protobuf generated header

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

void CommandProcessor::handlePacket(Packet& packet, NetworkInterface* sourceInterface) {
    if (!packet.isValid()) {
        // Invalid packet, discard or log error
        return;
    }

    if (packet.isForThisNode(node.getAddress())) {
        // Packet is intended for this node
        executeCommand(packet);
    } else {
        // Packet is not for this node, forward it
        forwardPacket(packet, sourceInterface);
    }
}

void CommandProcessor::executeCommand(const Packet& packet) {
    // Deserialize the Data field using Protobuf
    tfnode::NodeCommand command;
    if (!command.ParseFromArray(packet.data.data(), packet.data.size())) {
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
