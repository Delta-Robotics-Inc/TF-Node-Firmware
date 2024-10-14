#ifndef TF_NODE_H
#define TF_NODE_H

//#include "SMAController.hpp"
#include "networking/CommandProcessor.hpp"
#include "Settings.hpp"
#include "networking/tfnode-messages.h"

class SMAController;  // Forward Declaration TODO remove the need for this...

//=============================================================================
// Diagnostics
//=============================================================================

// ERROR INDECES
enum NodeError {
    ERR_LOW_VOLT,           // Low supply voltage error
    ERR_CURRENT_OF,         // Current overflow error
    ERR_EXTERNAL_INTERRUPT
};


class TFNode {
public:
    TFNode(const NodeAddress& address);
    void begin();
    void update();

    // Setter for CommandProcessor
    void setCommandProcessor(CommandProcessor* cp);

    NodeAddress getAddress() const;

    byte n_error = 0b11111111;  // Error byte transmitted when requested by API call

    unsigned long timeout_timer;
    unsigned long log_timer;    // Logs every x ms based on logMode
    unsigned long log_start;    // Time of start of log
    int node_log_mode = 0;        // 0=No log; 1=Device log

    float n_vSupply;    // Current value of measured battery voltage.
    float pot_val;      // Current value of the potentiometer

    std::vector<SMAController> smaControllers;


    // Command handlers
    // Status Logging functions
    tfnode::ResponseCode CMD_setStatusMode(tfnode::Device device, tfnode::DeviceStatusMode mode, NetworkInterface* iface);
    tfnode::ResponseCode CMD_resetDevice(tfnode::Device device);
    tfnode::ResponseCode CMD_enableDevice(tfnode::Device device);
    // Add other command handlers

    void sendStatusResponse();  // Sends status response based on status mode
    tfnode::NodeStatusCompact getStatusCompact();  // TODO change return type to .proto def
    tfnode::NodeStatusDump getStatusDump();
    String getStatusReadable();

    // Callbacks
    void optButtonStopFunc();

    // Error handling
    // TODO implement better error processing
    void checkErrs();
    void errRaise(NodeError err_code);
    void errClear(NodeError err_code);
    void errClear();

    // Sensor Measurements
    float getSupplyVolts();
    float getPotVal();

        


private:
    // TODO make array of SMAController
    tfnode::DeviceStatusMode statusMode;
    NodeAddress address;
    CommandProcessor* commandProcessor; // Reference to CommandProcessor
    NodeSettings settings;
    NetworkInterface* statusInterface;
};

#endif // TF_NODE_H
