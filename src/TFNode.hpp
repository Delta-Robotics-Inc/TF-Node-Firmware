#ifndef TF_NODE_H
#define TF_NODE_H

#include "SMAController.hpp"  // Ensure this header file contains the full definition of SMAController
#include "networking/CommandProcessor.hpp"
#include "networking/tfnode-messages.h"

//class SMAController;  // Forward Declaration TODO remove the need for this...

//=============================================================================
// Diagnostics
//=============================================================================

// ERROR INDECES
enum NodeError {
    ERR_LOW_VOLT,           // Low supply voltage error
    ERR_CURRENT_OF,         // Current overflow error
    ERR_EXTERNAL_INTERRUPT
};

enum StatusLEDColorState {
    COLOR_OFF,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_BLUE,
    COLOR_ORANGE,
    COLOR_WHITE
};


class TFNode {
public:
    TFNode(const NodeAddress& address);
    void begin();
    void update();

    void init_finished();

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

    // Use smart pointers to manage memory automatically
    //std::array<std::unique_ptr<SMAController>, 2> smaControllers;
    SMAController smaController0;
    SMAController smaController1;


    // Command handlers
    // Status Logging functions
    tfnode::ResponseCode CMD_setStatusMode(tfnode::Device device, tfnode::DeviceStatusMode mode, bool repeating, NetworkInterface* iface);
    tfnode::ResponseCode CMD_resetDevice(tfnode::Device device);
    tfnode::ResponseCode CMD_enableDevice(tfnode::Device device);
    tfnode::ResponseCode CMD_disableDevice(tfnode::Device device);
    // Add other command handlers

    void sendStatusResponse(tfnode::DeviceStatusMode mode);  // Sends status response based on status mode
    tfnode::NodeStatusCompact getStatusCompact();  // TODO change return type to .proto def
    tfnode::NodeStatusDump getStatusDump();
    String getStatusReadable();

    // Callbacks
    void optButtonStopFunc();
    void toggleRGBStatusLED(StatusLEDColorState colorA = COLOR_RED,
                            StatusLEDColorState colorB = COLOR_GREEN);
    void setRGBStatusLED(StatusLEDColorState color);
    void updateStatusLED();
    void signalPacketReceived();
    void updatePacketLED();
    void updateActiveLED();
    StatusLEDColorState ledState = COLOR_RED;

    // Error handling
    // TODO implement better error processing
    void checkErrs();
    void errRaise(NodeError err_code);
    void errClear(NodeError err_code);
    void errClear();

    // Sensor Measurements
    float getSupplyVolts();
    float getPotVal();

    NetworkInterface* statusInterface;


private:
    // TODO make array of SMAController
    tfnode::DeviceStatusMode statusMode;
    NodeAddress address;
    //CommandProcessor* commandProcessor; // Reference to CommandProcessor
    tfnode::NodeSettings settings;

    unsigned long ledBlinkTimer = 0;
    bool ledBlinkState = false;
    unsigned long packetLedTimer = 0;
    unsigned long activityLedTimer = 0;
    uint8_t activityLedStep = 0;
    uint8_t activityLedPulses = 0;
    bool activityLedPause = false;
};

#endif // TF_NODE_H
