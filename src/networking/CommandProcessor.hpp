// CommandProcessor.h

#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

#ifdef Response
#error "'Response' is defined as a macro"
#endif


#include <vector>
#include <Arduino.h>
#include "NetworkInterface.h"
#include "Packet.h"
#include "tfnode-messages.h" // Protobuf generated header

class TFNode;

class CommandProcessor {
public:
    CommandProcessor(TFNode& node);

    void addNetworkInterface(NetworkInterface* netInterface);
    NetworkInterface* getInterfaceByName(String name);
    std::vector<NetworkInterface *> getInterfaces();
    void process();
    void sendResponse(const tfnode::NodeResponse& response, NetworkInterface* iface);
    void sendHeartbeat();
    void sendSerialString(String message);

    bool isHeartbeatEnabled() const;
    bool isConnected() const;

    // Test Command Packet Generation
    void testSendCommandPacket();
    void testCANCommandPacket();
    void testCANEnableCommandPacket();

private:
    TFNode& node;
    std::vector<NetworkInterface*> interfaces;

    // For tracking Heartbeat timeouts. Packets will be sent and received at these intervals or else the node will be disabled.
    unsigned long lastReceiveMillis = 0;
    unsigned long lastSendMillis = 0;
    bool heartbeatEnabled = true;  // Default to enabled
    static const unsigned long HEARTBEAT_INTERVAL = 2000;  // 2 seconds between sent packets (send heartbeat if exceeded)
    static const unsigned long HEARTBEAT_TIMEOUT = 2500;  // 2.5 seconds between received packets from master (disable node if exceeded)

    void handlePacket(Packet& packet, NetworkInterface* sourceInterface, bool &isFromMaster);
    tfnode::NodeCommand parseCommandPacket(const Packet& packet);
    void handleCommand(Packet &packet, NetworkInterface* sourceInterface);
    tfnode::ResponseCode executeCommand(tfnode::NodeCommand command, NetworkInterface *sourceInterface);
    void forwardPacket(const Packet& packet, NetworkInterface* excludeInterface = nullptr);

    void handleAsciiCommand(String commandStr);  // TODO add handing of ascii command received over serial
};

#endif // COMMAND_PROCESSOR_H
