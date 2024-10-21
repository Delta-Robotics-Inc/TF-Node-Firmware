/**
 * @file CommandProcessor.cpp
 * @brief Implementation of the CommandProcessor class responsible for handling network commands and responses.
 *
 * This file contains the implementation of the CommandProcessor class, which is responsible for processing
 * incoming network packets, executing commands on the node or SMA controllers, and sending responses back
 * to the network interfaces. The CommandProcessor class interacts with various components such as TFNode,
 * NetworkInterface, and SMAController to perform its operations.
 *
 * The main functionalities provided by this class include:
 * - Adding network interfaces for communication.
 * - Processing incoming packets and determining if they are intended for this node.
 * - Handling packets by executing commands or forwarding them to other interfaces (route packets to other nodes).
 * - Sending responses to network interfaces after command execution.
 * - Parsing command packets and deserializing them into protobuf messages.
 * - Executing specific commands on the node or SMA controllers and returning appropriate response codes.
 *
 * The CommandProcessor class relies on the following components:
 * - TFNode: Represents the node that processes commands and maintains its state.
 * - NetworkInterface: Represents the network interface used for communication.
 * - SMAController: Represents the SMA controllers that can be controlled via commands.
 * - Packet: Represents the data packet used for communication.
 * - ReadBuffer and WriteBuffer: Used for serializing and deserializing protobuf messages.
 *
 *
 * @note This implementation assumes the use of the EmbeddedProto library for protobuf serialization and deserialization.
 */

#include "CommandProcessor.hpp"
#include "SMAController.hpp"
#include "TFNode.hpp" // Include the full definition of TFNode
#include "ReadBuffer.h"
#include "WriteBuffer.h"

CommandProcessor::CommandProcessor(TFNode& node)
    : node(node) {}

void CommandProcessor::addNetworkInterface(NetworkInterface* netInterface) {
    interfaces.push_back(netInterface);
}

NetworkInterface* CommandProcessor::getInterfaceByName(String name) {
    for (auto iface : interfaces) {
        if (iface->getName() == name.c_str()) {
            return iface;
        }
    }
    return nullptr;
}

/// @brief Accessor for the network interfaces
std::vector<NetworkInterface*> CommandProcessor::getInterfaces() {
    return interfaces;
}

void CommandProcessor::process() {
    for (auto iface : interfaces) {
        Packet packet;

        if (iface->receivePacket(packet)) {
            //#ifdef DEBUG
            //Serial.println("\nReceived Packet: ");
            //#endif
            //Serial.println(packet.toString());  // Debug display incoming packet
            handlePacket(packet, iface);
        }
    }
}

/// @brief Sends a response to the network interface
/// @param response Protobuf response message to send
/// @param iface 
void CommandProcessor::sendResponse(const tfnode::NodeResponse& response, NetworkInterface* iface) {
    //#ifdef DEBUG
    //Serial.print("\nNode Status: ");
    //#endif
    uint8_t bufferData[256]; // Adjust size as needed
    WriteBuffer buffer(bufferData, sizeof(bufferData));

    ::EmbeddedProto::Error err = response.serialize(buffer);

    // Debug
    //Serial.print(" Data: ");
    //for(int i = 0; i < buffer.get_size(); i++)
        //Serial.print((char)buffer.get_data()[i]);
    //Serial.print(" ");

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
        //packet.destinationId.id.clear();

        // Set data
        packet.data.assign(bufferData, bufferData + buffer.get_size());

        // Calculate packet length and checksum
        packet.packetLength = packet.calculatePacketLength();
        packet.checksum = packet.calculateChecksum();

        // Send the packet
        iface->sendPacket(packet);

        // Debug to console the full readable contents of packet
        //Serial.println("\nSent Packet: ");
        //Serial.println(packet.toString());  // Debug display outgoing packet
    } else {
        // Handle serialization error
    }
}

/// @brief Handles a packet received from a network interface, forwarding or executing commands as needed
/// @param packet Contains the data received from the network interface
/// @param sourceInterface The network interface that the packet was received from
void CommandProcessor::handlePacket(Packet& packet, NetworkInterface* sourceInterface) {
    if (!packet.isValid()) {
        // Invalid packet, discard or log error
        return;
    }

    if (packet.isForThisNode(node.getAddress())) {

        tfnode::NodeCommand command = parseCommandPacket(packet);
        tfnode::ResponseCode code;

        if (command.get_which_command() != tfnode::NodeCommand::FieldNumber::NOT_SET) {
            // Packet is intended for this node
            code = executeCommand(command, sourceInterface);
        }
        // Command not recognized
        else {
            code = tfnode::ResponseCode::RESPONSE_UNSUPPORTED_COMMAND;
        }

        // Create a response message.  GeneralResponse is the response message for all commands
        tfnode::NodeResponse response;
        tfnode::GeneralResponse generalResponse;
        generalResponse.set_response_code(code);
        generalResponse.set_received_cmd(tfnode::FunctionCode::FUNCTION_ENABLE);  // TODO parse the received command into a function code
        response.set_general_response(generalResponse);

        sendResponse(response, sourceInterface);
    } else {
        // Packet is not for this node, forward it
        forwardPacket(packet, sourceInterface);
    }
}

/// @brief Parses a command packet and returns the command protobuf message
tfnode::NodeCommand CommandProcessor::parseCommandPacket(const Packet& packet) {
    // Create a ReadBuffer from the packet data
    ReadBuffer buffer(packet.data.data(), packet.data.size());

    // Deserialize the NodeCommand message
    tfnode::NodeCommand command;
    ::EmbeddedProto::Error err = command.deserialize(buffer);

    if (err != ::EmbeddedProto::Error::NO_ERRORS) {
        // Failed to parse command
        // Handle error
    }

    return command;
}

/// @brief Executes a commmand on the node or SMA controllers and returns a response code
/// @param command Protobuf command message
/// @param sourceInterface Network interface that sent the command
/// @return 
tfnode::ResponseCode CommandProcessor::executeCommand(tfnode::NodeCommand command, NetworkInterface* sourceInterface) {

    tfnode::ResponseCode responseCode = tfnode::ResponseCode::RESPONSE_UNSUPPORTED_COMMAND;

    // Determine which command is set using the oneof field
    switch (command.get_which_command()) {
        case tfnode::NodeCommand::FieldNumber::RESET:
            responseCode = node.CMD_resetDevice(command.reset().device());
            break;
        case tfnode::NodeCommand::FieldNumber::ENABLE:
            responseCode = node.CMD_enableDevice(command.enable().device());
            break;
        case tfnode::NodeCommand::FieldNumber::DISABLE:
            responseCode = node.CMD_enableDevice(command.disable().device());
            break;
        case tfnode::NodeCommand::FieldNumber::SET_MODE:
            switch(command.set_mode().device()) {
                case tfnode::Device::DEVICE_PORT1:
                    node.smaControllers[0].CMD_setMode(command.set_mode().mode());
                    responseCode = tfnode::ResponseCode::RESPONSE_SUCCESS;
                    break;
                case tfnode::Device::DEVICE_PORT2:
                    node.smaControllers[1].CMD_setMode(command.set_mode().mode());
                    responseCode = tfnode::ResponseCode::RESPONSE_SUCCESS;
                    break;
                case tfnode::Device::DEVICE_ALL:
                case tfnode::Device::DEVICE_PORTALL:
                    for (auto& controller : node.smaControllers) {
                        controller.CMD_setMode(command.set_mode().mode());
                    }
                    responseCode = tfnode::ResponseCode::RESPONSE_SUCCESS;
                    break;
                default:
                    responseCode = tfnode::ResponseCode::RESPONSE_INVALID_DEVICE;
                    break;
            }
            break;
        case tfnode::NodeCommand::FieldNumber::SET_SETPOINT:
            switch(command.set_setpoint().device()) {
                case tfnode::Device::DEVICE_PORT1:
                    node.smaControllers[0].CMD_setSetpoint(command.set_setpoint().mode(), command.set_setpoint().setpoint());
                    responseCode = tfnode::ResponseCode::RESPONSE_SUCCESS;
                    break;
                case tfnode::Device::DEVICE_PORT2:
                    node.smaControllers[1].CMD_setSetpoint(command.set_setpoint().mode(), command.set_setpoint().setpoint());
                    responseCode = tfnode::ResponseCode::RESPONSE_SUCCESS;
                    break;
                case tfnode::Device::DEVICE_ALL:
                case tfnode::Device::DEVICE_PORTALL:
                    for (auto& controller : node.smaControllers) {
                        controller.CMD_setSetpoint(command.set_setpoint().mode(), command.set_setpoint().setpoint());
                    }
                    responseCode = tfnode::ResponseCode::RESPONSE_SUCCESS;
                    break;
                default:
                    responseCode = tfnode::ResponseCode::RESPONSE_INVALID_DEVICE;
                    break;
            }
            break;
        case tfnode::NodeCommand::FieldNumber::SILENCE_NODE:
            // TODO implement logic to silence the node
            responseCode = tfnode::ResponseCode::RESPONSE_SUCCESS;
            break;
        case tfnode::NodeCommand::FieldNumber::CONFIGURE_SETTINGS:
            // TODO implement logic to configure settings for the node
            responseCode = tfnode::ResponseCode::RESPONSE_SUCCESS;
            break;
        case tfnode::NodeCommand::FieldNumber::STATUS:
            responseCode = node.CMD_setStatusMode(
                command.status().device(),
                command.status().mode(),
                command.status().repeating(),
                sourceInterface // Pass the interface
            );
            break;
        // Handle other commands similarly
        default:
            responseCode = tfnode::ResponseCode::RESPONSE_UNSUPPORTED_COMMAND;
            break;
    }

    return responseCode;
}

void CommandProcessor::forwardPacket(const Packet& packet, NetworkInterface* excludeInterface) {
    Serial.println("Forwarding packet to other interfaces...");
    for (auto iface : interfaces) {
        if (iface != excludeInterface) {
            // If Network ID Type and Network ID are specified, forward only to matching interface
            // Implement logic based on your network addressing
            iface->sendPacket(packet);
        }
    }
}

// Simply print message over serial
void CommandProcessor::sendSerialString(String message) {
    Serial.println(message);
}


/// @brief Test function to send a command to the PC.  Not used in production.
///        Use this function to determine the format of the command packet.
void CommandProcessor::testSendCommandPacket() {

    // Specify the command being sent
    Serial.println("Sending Serialized StatusCommand: ");

    // Create a NodeCommand message
    tfnode::NodeCommand command;
    tfnode::GetStatusCommand statusCommand;  // Or other command type

    // Set the command fields as needed
    statusCommand.set_device(tfnode::Device::DEVICE_ALL);
    statusCommand.set_mode(tfnode::DeviceStatusMode::STATUS_COMPACT);
    statusCommand.set_repeating(false);
    command.set_status(statusCommand);

    uint8_t bufferData[256]; // Adjust size as needed
    WriteBuffer buffer(bufferData, sizeof(bufferData));

    ::EmbeddedProto::Error err = command.serialize(buffer);

    // Debug
    //Serial.print(" Data: ");
    //for(int i = 0; i < buffer.get_size(); i++)
        //Serial.print((char)buffer.get_data()[i]);
    //Serial.print(" ");

    if (err == ::EmbeddedProto::Error::NO_ERRORS) {
        // Create a Packet with the serialized data
        Packet packet;

        // Set sender and destination IDs
        packet.senderId.id = {0x0, 0x0, 0x0}; // Set sender ID to zero for test
        packet.senderId.idType = NodeAddress::IDType::NodeID;

        // Set destination ID as needed
        // For simplicity, set destination ID to zero
        packet.destinationId.id = {0x01, 0x02, 0x03};  // Destination is this device for the test
        packet.destinationId.idType = NodeAddress::IDType::NodeID;
        // Set data
        packet.data.assign(bufferData, bufferData + buffer.get_size());

        // Calculate packet length and checksum
        packet.packetLength = packet.calculatePacketLength();
        packet.checksum = packet.calculateChecksum();

        // Debugging output
        Serial.print("Calculated Packet Length: ");
        Serial.println(packet.packetLength);

        // Send the packet over Serial Interface
        getInterfaceByName("SerialInterface")->sendPacket(packet);

        // Debug to console the full readable contents of packet
        Serial.println("\nSent Packet: ");
        Serial.println(packet.toString());  // Debug display outgoing packet

        delay(5000);

        // Now, handle the same packet that was constructed to test packet handling
        Serial.println("Handling packet...\n");
        handlePacket(packet, getInterfaceByName("SerialInterface"));
    } else {
        // Handle serialization error
        Serial.println("Error: Failed to serialize command");
    }
}