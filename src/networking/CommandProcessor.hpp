// CommandProcessor.h

#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

#include <vector>
#include "NetworkInterface.h"
#include "TFNode.hpp"
#include "Packet.h"
#include "tfnode-messages.h" // Protobuf generated header

class CommandProcessor {
public:
    CommandProcessor(TFNode& node);

    void addNetworkInterface(NetworkInterface* netInterface);
    void process();

private:
    TFNode& node;
    std::vector<NetworkInterface*> interfaces;

    void handlePacket(Packet& packet, NetworkInterface* sourceInterface);
    void executeCommand(const Packet& packet);
    void forwardPacket(const Packet& packet, NetworkInterface* excludeInterface = nullptr);

    void sendResponse(const tfnode::Response response, NetworkInterface* iface);
};

#endif // COMMAND_PROCESSOR_H
