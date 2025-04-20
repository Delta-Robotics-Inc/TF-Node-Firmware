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
#include "SerialInterface.h"
#include "SMAController.hpp"
#include "TFNode.hpp" // Include the full definition of TFNode
#include "ReadBuffer.h"
#include "WriteBuffer.h"
#include "globals.hpp"

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
    static bool isFromMaster = false;
    unsigned long now = millis();

    // Check receive heartbeat from master timeout
    if (heartbeatEnabled && (now - lastReceiveMillis) > HEARTBEAT_TIMEOUT) {
        node.CMD_disableDevice(tfnode::Device::DEVICE_ALL);
        // Reset timer so we don't repeatedly disable
        lastReceiveMillis = now;
        sendSerialString("Heartbeat timeout, disabling all devices. Use /heartbeat off to disable this feature.");
    }

    // Check send heartbeat timeout
    if (heartbeatEnabled && (now - lastSendMillis) > HEARTBEAT_INTERVAL) { // TODO add enable/disable heartbeat to DeltaLink protobuf config
        // Build and send heartbeat packet
        sendHeartbeat();
        lastSendMillis = now;
    }

    for (auto iface : interfaces) {
        Packet packet;
        if (iface->receivePacket(packet)) {
            handlePacket(packet, iface, isFromMaster);
            if(isFromMaster) lastReceiveMillis = now;  // Register heartbeat from master

        }

        // If the interface is a SerialInterface, check for ascii commands.
        if (iface->getName() == "SerialInterface") {
            SerialInterface* serialIface = static_cast<SerialInterface*>(iface);
            while (serialIface->hasAsciiCommand()) {
                // Serial.println("Ascii command received");
                String asciiCmd = serialIface->getNextAsciiCommand();
                handleAsciiCommand(asciiCmd);
                lastReceiveMillis = now;  // Also register heartbeat in ASCII
            }
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

        // Set destination ID as master (0.0.0)
        // TODO track the master address and set it here
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

        // // Debug to console the full readable contents of packet
        // Serial.print("\nSent Packet over ");
        // Serial.print(iface->getName().c_str());
        // Serial.println(": ");
        // Serial.println(packet.toString());  // Debug display outgoing packet
    } else {
        // Handle serialization error
    }
    lastSendMillis = millis();  // Reset the send timer after sending a response
}

void CommandProcessor::sendHeartbeat() {
    // Create a Packet with empty data
    Packet packet;

    // Set sender ID as this node's address
    packet.senderId = node.getAddress();
    packet.senderId.idType = node.getAddress().idType;

    // Set destination ID as master (0.0.0)
    packet.destinationId.id = {0x0, 0x0, 0x0};
    packet.destinationId.idType = NodeAddress::IDType::NodeID;

    // Empty data field
    packet.data.clear();

    // Calculate packet length and checksum
    packet.packetLength = packet.calculatePacketLength();
    packet.checksum = packet.calculateChecksum();

    // Send the packet on all interfaces
    for (auto iface : interfaces) {
        iface->sendPacket(packet);
    }
}

/// @brief Handles a packet received from a network interface, forwarding or executing commands as needed
/// @param packet Contains the data received from the network interface
/// @param sourceInterface The network interface that the packet was received from
void CommandProcessor::handlePacket(Packet& packet, NetworkInterface* sourceInterface, bool &isFromMaster) {
    if (!packet.isValid()) {
        // Invalid packet, discard or log error
        Serial.println("Invalid packet, will not execute.");
        return;
    }

    // Master address is 0.0.0
    if(packet.senderId.id[0] == 0x00 && 
       packet.senderId.id[1] == 0x00 && 
       packet.senderId.id[2] == 0x00){
        isFromMaster = true;
    }
    else{
        isFromMaster = false;
    }

    // Broadcast packets are handled by all nodes, so they are processed and forwarded
    if(packet.isBroadcast()) {
        handleCommand(packet, sourceInterface);
        forwardPacket(packet, sourceInterface);
        Serial.println("Got Broadcast!");
    }
    // Packets intended for this node are processed
    else if (packet.isForThisNode(node.getAddress())) {
        handleCommand(packet, sourceInterface);     
    } 
    // Packets intended for other nodes are forwarded
    else {
        // Packet is not for this node, forward it
        forwardPacket(packet, sourceInterface);
        //Serial.println("Packet is not for this node, forwarding...");
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
        Serial.println("Failed to parse command packet");
    }

    return command;
}

void CommandProcessor::handleCommand(Packet& packet, NetworkInterface* sourceInterface) {
    tfnode::NodeCommand command = parseCommandPacket(packet);
    tfnode::ResponseCode code;

    if (command.get_which_command() != tfnode::NodeCommand::FieldNumber::NOT_SET) {
        // Packet is intended for this node
        code = executeCommand(command, sourceInterface);
    }
    // Command not recognized
    else {
        code = tfnode::ResponseCode::RESPONSE_UNSUPPORTED_COMMAND;
        Serial.println("Command not recognized..");
    }

    // Create a response message.  GeneralResponse is the response message for all commands
    tfnode::NodeResponse response;
    tfnode::GeneralResponse generalResponse;
    generalResponse.set_device(tfnode::Device::DEVICE_NODE);  // Set the device to the node
    generalResponse.set_response_code(code);
    generalResponse.set_received_cmd(tfnode::FunctionCode::FUNCTION_ENABLE);  // TODO parse the received command into a function code
    response.set_general_response(generalResponse);

    Serial.println("Sending Command ACK Response...");
    sendResponse(response, sourceInterface);
}   

/// @brief Executes a commmand on the node or SMA controllers and returns a response code
/// @param command Protobuf command message
/// @param sourceInterface Network interface that sent the command
/// @return 
tfnode::ResponseCode CommandProcessor::executeCommand(tfnode::NodeCommand command, NetworkInterface* sourceInterface) {

    Serial.println("Executing command...");
    tfnode::ResponseCode responseCode = tfnode::ResponseCode::RESPONSE_UNSUPPORTED_COMMAND;

    // Debug to show that command is being executed
    node.toggleRGBStatusLED();  // Toggle the status LED on the node

    // Determine which command is set using the oneof field
    switch (command.get_which_command()) {
        case tfnode::NodeCommand::FieldNumber::RESET:
            responseCode = node.CMD_resetDevice(command.reset().device());
            //Serial.println("Reset Received");
            break;
        case tfnode::NodeCommand::FieldNumber::ENABLE:
            responseCode = node.CMD_enableDevice(command.enable().device());
            //Serial.println("Enable received");
            break;
        case tfnode::NodeCommand::FieldNumber::DISABLE:
            responseCode = node.CMD_disableDevice(command.disable().device());
            //Serial.println("Disable received");
            break;
        case tfnode::NodeCommand::FieldNumber::SET_MODE:
            //Serial.println("Set Mode Received");
            switch(command.set_mode().device()) {
                case tfnode::Device::DEVICE_PORT0:
                    node.smaController0.CMD_setMode(command.set_mode().mode());
                    responseCode = tfnode::ResponseCode::RESPONSE_SUCCESS;
                    break;
                case tfnode::Device::DEVICE_PORT1:
                    node.smaController1.CMD_setMode(command.set_mode().mode());
                    responseCode = tfnode::ResponseCode::RESPONSE_SUCCESS;
                    break;
                case tfnode::Device::DEVICE_ALL:
                case tfnode::Device::DEVICE_PORTALL:
                        node.smaController0.CMD_setMode(command.set_mode().mode());
                        node.smaController1.CMD_setMode(command.set_mode().mode());
                    responseCode = tfnode::ResponseCode::RESPONSE_SUCCESS;
                    break;
                default:
                    responseCode = tfnode::ResponseCode::RESPONSE_INVALID_DEVICE;
                    break;
            }
            break;
        case tfnode::NodeCommand::FieldNumber::SET_SETPOINT:
            //Serial.println("Set Setpoint Received");
            switch(command.set_setpoint().device()) {
                case tfnode::Device::DEVICE_PORT0:
                    node.smaController0.CMD_setSetpoint(command.set_setpoint().mode(), command.set_setpoint().setpoint());
                    responseCode = tfnode::ResponseCode::RESPONSE_SUCCESS;
                    break;
                case tfnode::Device::DEVICE_PORT1:
                    node.smaController1.CMD_setSetpoint(command.set_setpoint().mode(), command.set_setpoint().setpoint());
                    responseCode = tfnode::ResponseCode::RESPONSE_SUCCESS;
                    break;
                case tfnode::Device::DEVICE_ALL:
                case tfnode::Device::DEVICE_PORTALL:
                    node.smaController0.CMD_setSetpoint(command.set_setpoint().mode(), command.set_setpoint().setpoint());
                    node.smaController1.CMD_setSetpoint(command.set_setpoint().mode(), command.set_setpoint().setpoint());
                    responseCode = tfnode::ResponseCode::RESPONSE_SUCCESS;
                    break;
                default:
                    responseCode = tfnode::ResponseCode::RESPONSE_INVALID_DEVICE;
                    break;
            }
            break;
        case tfnode::NodeCommand::FieldNumber::SILENCE_NODE:
            //Serial.println("Silence Node Received");
            // TODO implement logic to silence the node
            responseCode = tfnode::ResponseCode::RESPONSE_SUCCESS;
            break;
        case tfnode::NodeCommand::FieldNumber::CONFIGURE_SETTINGS:
            //Serial.println("Configure Settings Received");
            // TODO implement logic to configure settings for the node
            responseCode = tfnode::ResponseCode::RESPONSE_SUCCESS;
            break;
        case tfnode::NodeCommand::FieldNumber::STATUS:
            //Serial.println("Status Received");
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
            Serial.println("Unsupported command.");
            break;
    }

    return responseCode;
}

void CommandProcessor::forwardPacket(const Packet& packet, NetworkInterface* excludeInterface) {
    Serial.println("Forwarding packet to other interfaces...");
    for (auto iface : interfaces) {
        if (iface != excludeInterface) {
            // If Network ID Type and Network ID are specified, forward only to matching interface
            iface->sendPacket(packet);

            // Debug to console the full readable contents of packet
            Serial.print("\nForwarded Packet over ");
            Serial.print(iface->getName().c_str());
        //  Serial.println(": ");
        //     Serial.println(packet.toString());  // Debug display outgoing packet   
        }
    }
}

void CommandProcessor::handleAsciiCommand(String commandStr)
{
    // Lambda to get node address prefix.
    auto addrPrefix = [&]() -> String {
        return "\n(" + String(node.getAddress().id[0]) + "." + String(node.getAddress().id[1]) + "." + String(node.getAddress().id[2]) + ") ";
    };

    // Ensure the command starts with '/'.
    if (!commandStr.startsWith("/"))
    {
        sendSerialString(addrPrefix() + "Error parsing ASCII: Command must start with '/'");
        return;
    }
    commandStr.remove(0, 1);

    // Split the command string by spaces into tokens.
    std::vector<String> tokens;
    {
        int startIndex = 0;
        while (true)
        {
            int spaceIndex = commandStr.indexOf(' ', startIndex);
            if (spaceIndex == -1)
            {
                String token = commandStr.substring(startIndex);
                if (token.length() > 0)
                    tokens.push_back(token);
                break;
            }
            else
            {
                String token = commandStr.substring(startIndex, spaceIndex);
                if (token.length() > 0)
                    tokens.push_back(token);
                startIndex = spaceIndex + 1;
            }
        }
    }
    if (tokens.empty())
    {
        sendSerialString(addrPrefix() + "Error: Empty command string");
        return;
    }

    // Helper lambdas to parse device and mode strings.
    auto parseDevice = [&](const String &devStr) -> tfnode::Device {
        // Valid device identifiers:
        if (devStr == "all")      return tfnode::Device::DEVICE_ALL;
        if (devStr == "node")     return tfnode::Device::DEVICE_NODE;
        if (devStr == "portall")  return tfnode::Device::DEVICE_PORTALL;
        // Sometimes referred to as "M1" or "port0"
        if (devStr == "m1" || devStr == "port0") return tfnode::Device::DEVICE_PORT0;
        // Sometimes referred to as "M2" or "port1"
        if (devStr == "m2" || devStr == "port1") return tfnode::Device::DEVICE_PORT1;
        // Not recognized
        return (tfnode::Device)(-1);
    };
    auto parseMode = [&](const String &modeStr) -> tfnode::SMAControlMode {
        if (modeStr == "percent") return tfnode::SMAControlMode::MODE_PERCENT;
        if (modeStr == "amps")    return tfnode::SMAControlMode::MODE_AMPS;
        if (modeStr == "volts")   return tfnode::SMAControlMode::MODE_VOLTS;
        if (modeStr == "ohms")    return tfnode::SMAControlMode::MODE_OHMS;
        return tfnode::SMAControlMode::MODE_PERCENT; // default
    };

    tfnode::NodeCommand nodeCmd;
    tfnode::ResponseCode result = tfnode::ResponseCode::RESPONSE_UNSUPPORTED_COMMAND;
    String cmd = tokens[0];


    // Next, translate the ASCII command into a DeltaLink protobuf message and execute with executeCommand()

    // -------------------- about --------------------
    if (cmd == "about")
    {
        // Send a string response with the firmware version, node ID, and timestamp
        String response = addrPrefix() + 
                         "Firmware: " + String(CFG_FIRMWARE_VERSION) + 
                         "\nNode ID: " + String(node.getAddress().id[0]) + "." + String(node.getAddress().id[1]) + "." + String(node.getAddress().id[2]) + 
                         "\nTimestamp: NONE";// TODO add timestamp
        sendSerialString(response);
    }
    // -------------------- heartbeat --------------------
    else if (cmd == "heartbeat")
    {
        if (tokens.size() > 1) {
            if (tokens[1] == "on") {
                heartbeatEnabled = true;
                sendSerialString(addrPrefix() + "Heartbeat enabled\m");
            } else if (tokens[1] == "off") {
                heartbeatEnabled = false;
                sendSerialString(addrPrefix() + "Heartbeat disabled\n");
            } else {
                sendSerialString(addrPrefix() + "Error: Invalid heartbeat command. Use 'on' or 'off'");
            }
        } else {
            // Just send a heartbeat immediately
            sendHeartbeat();
            sendSerialString(addrPrefix() + "Heartbeat sent");
        }
    }
    // -------------------- reset --------------------
    else if (cmd == "reset")
    {
        tfnode::ResetCommand resetCmd;
        if (tokens.size() > 1)
        {
            auto dev = parseDevice(tokens[1]);
            if (dev == (tfnode::Device)(-1)) // Undefined device
            {
                sendSerialString(addrPrefix() + "Error: Unknown device for reset command");
                return;
            }
            resetCmd.set_device(dev);
        }
        else
        {
            resetCmd.set_device(tfnode::Device::DEVICE_ALL);
        }
        nodeCmd.set_reset(resetCmd);
    }
    // -------------------- set-enable --------------------
    else if (cmd == "set-enable")
    {
        if (tokens.size() <= 2)
        {
            sendSerialString(addrPrefix() + "Error: set-enable command requires device and state parameters");
            return;
        }
        auto dev = parseDevice(tokens[1]);
        if (dev == (tfnode::Device)(-1)) // Undefined device
        {
            sendSerialString(addrPrefix() + "Error: Unknown device in set-enable command");
            return;
        }
        if (!(tokens[2] == "true" || tokens[2] == "false"))
        {
            sendSerialString(addrPrefix() + "Error: set-enable command requires 'true' or 'false' for the state");
            return;
        }
        if (tokens[2] == "true")
        {
            sendSerialString(addrPrefix() + "Enabling " + tokens[1] + '\n');
            tfnode::EnableCommand enableCmd;
            enableCmd.set_device(dev);
            nodeCmd.set_enable(enableCmd);
        }
        else
        {
            sendSerialString(addrPrefix() + "Disabling " + tokens[1] + '\n');
            tfnode::DisableCommand disableCmd;
            disableCmd.set_device(dev);
            nodeCmd.set_disable(disableCmd);
        }
    }
    // -------------------- set-mode --------------------
    else if (cmd == "set-mode")
    {
        if (tokens.size() <= 2)
        {
            sendSerialString(addrPrefix() + "Error: set-mode command requires device and mode parameters");
            return;
        }
        auto dev = parseDevice(tokens[1]);
        if (dev == (tfnode::Device)(-1))
        {
            sendSerialString(addrPrefix() + "Error: Unknown device in set-mode command");
            return;
        }
        if (tokens[2] != "percent" && tokens[2] != "amps" &&
            tokens[2] != "volts"   && tokens[2] != "ohms")
        {
            sendSerialString(addrPrefix() + "Error: Unknown mode in set-mode command");
            return;
        }
        tfnode::SetModeCommand modeCmd;
        modeCmd.set_device(dev);
        modeCmd.set_mode(parseMode(tokens[2]));
        nodeCmd.set_set_mode(modeCmd);
    }
    // -------------------- set-setpoint --------------------
    else if (cmd == "set-setpoint")
    {
        if (tokens.size() <= 3)
        {
            sendSerialString(addrPrefix() + "Error: set-setpoint command requires device, mode, and setpoint parameters");
            return;
        }
        auto dev = parseDevice(tokens[1]);
        if (dev == (tfnode::Device)(-1))
        {
            sendSerialString(addrPrefix() + "Error: Unknown device in set-setpoint command");
            return;
        }
        if (tokens[2] != "percent" && tokens[2] != "amps" &&
            tokens[2] != "volts"   && tokens[2] != "ohms")
        {
            sendSerialString(addrPrefix() + "Error: Unknown mode in set-setpoint command");
            return;
        }
        float val = tokens[3].toFloat();
        if (val == 0.0f && tokens[3] != "0" && tokens[3] != "0.0")
        {
            sendSerialString(addrPrefix() + "Error: Invalid setpoint value in set-setpoint command");
            return;
        }
        tfnode::SetSetpointCommand setpointCmd;
        setpointCmd.set_device(dev);
        setpointCmd.set_mode(parseMode(tokens[2]));
        setpointCmd.set_setpoint(val);
        nodeCmd.set_set_setpoint(setpointCmd);
    }
    // -------------------- status --------------------
    else if (cmd == "status")
    {
        tfnode::GetStatusCommand statusCmd;
        if (tokens.size() > 1)
        {
            auto dev = parseDevice(tokens[1]);
            if (dev == (tfnode::Device)(-1))
            {
                sendSerialString(addrPrefix() + "Error: Unknown device in status command");
                return;
            }
            statusCmd.set_device(dev);
        }
        else
        {
            statusCmd.set_device(tfnode::Device::DEVICE_ALL);
        }
        // Ascii command for status will always be in readable mode.
        statusCmd.set_mode(tfnode::DeviceStatusMode::STATUS_DUMP_READABLE);
        nodeCmd.set_status(statusCmd);
    }
    else if (cmd == "help")
    {
        const char* helpLines[] = {
            "addrPrefix()\n",
            "========================================",
            "Available Commands:",
            "/help                - Display this help message",
            "/about               - Display information about the node (firmware version, node ID, timestamp)",
            "/heartbeat [on|off]  - Send a heartbeat or enable/disable automatic heartbeats",
            "/reset [device]      - Reset the specified device (default: all)",
            "/set-enable <device> <true|false> - Enable or disable a device",
            "/set-mode <device> <percent|amps|volts|ohms> - Set the mode for a device",
            "/set-setpoint <device> <percent|amps|volts|ohms> <value> - Set the setpoint for a device",
            "/status [device]     - Get the status of a device (default: all)"
        };
        const int numLines = sizeof(helpLines) / sizeof(helpLines[0]);
        String helpMsg;
        for (int i = 0; i < numLines; i++) {
            helpMsg += helpLines[i];
            helpMsg += "\n";
        }
        sendSerialString(helpMsg);
        return;
    }
    else
    {
        sendSerialString(addrPrefix() + "Error: Unsupported command '" + cmd + "'");
        return;
    }

    // If a valid command was built, execute it.
    if (nodeCmd.get_which_command() != tfnode::NodeCommand::FieldNumber::NOT_SET)
    {
        result = executeCommand(nodeCmd, getInterfaceByName("SerialInterface"));

        // Build and send response.
        tfnode::NodeResponse resp;
        tfnode::GeneralResponse genResp;
        genResp.set_device(tfnode::Device::DEVICE_NODE);
        genResp.set_response_code(result);
        genResp.set_received_cmd(tfnode::FunctionCode::FUNCTION_ENABLE);
        resp.set_general_response(genResp);

        // Convert response to a string, adding a new line before the response.
        String responseString = addrPrefix() +
                                "Received: Device:" + String((int)genResp.get_device()) +
                                ", Code:" + String((int)genResp.get_response_code()) +
                                ", Cmd:" + String((int)genResp.get_received_cmd());
        sendSerialString(responseString);
    }
}

// Simply print message over serial
void CommandProcessor::sendSerialString(String message) {
    Serial.print(message);
}






//=============================================================================
// Internal Test Functions
//=============================================================================

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
        bool temp = false;
        handlePacket(packet, getInterfaceByName("SerialInterface"), temp);
    } else {
        // Handle serialization error
        Serial.println("Error: Failed to serialize command");
    }
}

/// @brief Test function to send a command to the PC.  Not used in production.
///        Use this function to determine the format of the command packet.
void CommandProcessor::testCANEnableCommandPacket() {

    // Specify the command being sent
    Serial.println("Sending CAN EnableCommand: ");

    // Create a NodeCommand message
    tfnode::NodeCommand command;
    tfnode::EnableCommand enableCommand;  // Or other command type

    // Set the command fields as needed
    enableCommand.set_device(tfnode::Device::DEVICE_PORT1);
    command.set_enable(enableCommand);

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
        getInterfaceByName("CANInterface")->sendPacket(packet);

        // Debug to console the full readable contents of packet
        Serial.println("\nSent Packet: ");
        Serial.println(packet.toString());  // Debug display outgoing packet

        delay(5000);

        // Now, handle the same packet that was constructed to test packet handling
        //Serial.println("Handling packet...\n");
        //handlePacket(packet, getInterfaceByName("SerialInterface"));
    } else {
        // Handle serialization error
        Serial.println("Error: Failed to serialize command");
    }
}

/// @brief Test function to send a command to the PC.  Not used in production.
///        Use this function to determine the format of the command packet.
void CommandProcessor::testCANCommandPacket() {
    

    // Specify the command being sent
    Serial.println("Sending CAN StatusCommand: ");

    // Create a NodeCommand message
    tfnode::NodeCommand command;
    tfnode::GetStatusCommand statusCommand;  // Or other command type

    // Set the command fields as needed
    statusCommand.set_device(tfnode::Device::DEVICE_ALL);
    statusCommand.set_mode(tfnode::DeviceStatusMode::STATUS_COMPACT);
    statusCommand.set_repeating(true);
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
        packet.destinationId.id = {0x00, 0x00, 0x03};  // Destination is this device for the test
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
        getInterfaceByName("CANInterface")->sendPacket(packet);

        // Debug to console the full readable contents of packet
        Serial.println("\nSent Packet: ");
        Serial.println(packet.toString());  // Debug display outgoing packet

        delay(5000);

        // Now, handle the same packet that was constructed to test packet handling
        //Serial.println("Handling packet...\n");
        //handlePacket(packet, getInterfaceByName("SerialInterface"));
    } else {
        // Handle serialization error
        Serial.println("Error: Failed to serialize command");
    }
}