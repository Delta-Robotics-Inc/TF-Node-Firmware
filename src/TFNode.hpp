#ifndef TF_NODE_H
#define TF_NODE_H

//#include "SMAController.hpp"
#include "CommandProcessor.hpp"
#include "Settings.hpp"

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
    TFNode();
    void begin();
    void update();

    byte n_error = 0b11111111;  // Error byte transmitted when requested by API call

    unsigned long timeout_timer;
    unsigned long log_timer;    // Logs every x ms based on logMode
    unsigned long log_start;    // Time of start of log
    int node_log_mode = 0;        // 0=No log; 1=Device log

    float n_vSupply;    // Current value of measured battery voltage.
    float pot_val;      // Current value of the potentiometer


    // Status Logging functions
    void CMD_setStatusMode(int _mode);
    String status();
    String statusCompact();  // TODO change return type to .proto def
    String statusDump();
    String statusReadable();


    // Callbacks
    void optButtonStopFunc();


    // Error handling
    void checkErrs();
    void errRaise(NodeError err_code);
    void errClear(NodeError err_code);
    void errClear();


    // Sensor Measurements
    float getSupplyVolts();
    float getPotVal();

private:
    // TODO make array of SMAController
    SMAController* smaController0;
    SMAController* smaController1;
    CommandProcessor commandProcessor;
    NodeSettings settings;
};

#endif // TF_NODE_H
