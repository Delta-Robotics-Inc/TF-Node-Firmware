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
    unsigned long getLastReceiveMillis() const { return lastReceiveMillis; }

    // Test Command Packet Generation
    void testSendCommandPacket();
    void testCANCommandPacket();
    void testCANEnableCommandPacket();

private:
    TFNode& node;
    std::vector<NetworkInterface*> interfaces;

    // HEARTBEAT TIMING VARIABLES - CRITICAL: These track different events and must not be confused
    // =======================================================================================
    
    // General packet reception timing (any packet from any source)
    unsigned long lastReceiveMillis = millis();  // Initialize with current time
    
    // CRITICAL FIX: Separate timing variables for heartbeat send vs receive operations
    // This separation prevents the race condition where timeout resets interfere with detection
    
    // Tracks when WE last sent a heartbeat TO the master
    unsigned long lastHeartbeatSentMillis = millis();  // Initialize with current time
    
    // Tracks when we last RECEIVED a heartbeat FROM the master
    // This is the critical variable for timeout detection - must NEVER be reset during timeout handling
    unsigned long lastHeartbeatReceivedMillis = millis();  // Initialize with current time
    
    // Connection state tracking
    bool heartbeatEnabled = true;  // Default to enabled
    bool isMasterConnected = false;  // Simple boolean flag for connection status
    bool timeoutTriggered = false;  // Prevents repeated timeout actions until recovery
    
    // Heartbeat timing constants
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
