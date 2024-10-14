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
    std::vector<NetworkInterface*> getInterfaces();
    void process();
    void sendResponse(const (tfnode::Response)& response, NetworkInterface* iface);
    void sendSerialString(String message);

private:
    TFNode& node;
    std::vector<NetworkInterface*> interfaces;

    void handlePacket(Packet& packet, NetworkInterface* sourceInterface);
    void executeCommand(const Packet& packet, NetworkInterface* sourceInterface);
    void forwardPacket(const Packet& packet, NetworkInterface* excludeInterface = nullptr);
};

#endif // COMMAND_PROCESSOR_H
