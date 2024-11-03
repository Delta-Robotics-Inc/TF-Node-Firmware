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
    void sendSerialString(String message);

    // Test Command Packet Generation
    void testSendCommandPacket();
    void testCANCommandPacket();
    void testCANEnableCommandPacket();

private:
    TFNode& node;
    std::vector<NetworkInterface*> interfaces;

    void handlePacket(Packet& packet, NetworkInterface* sourceInterface);
    tfnode::NodeCommand parseCommandPacket(const Packet& packet);
    void handleCommand(Packet &packet, NetworkInterface* sourceInterface);
    tfnode::ResponseCode executeCommand(tfnode::NodeCommand command, NetworkInterface *sourceInterface);
    void forwardPacket(const Packet& packet, NetworkInterface* excludeInterface = nullptr);
};

#endif // COMMAND_PROCESSOR_H
