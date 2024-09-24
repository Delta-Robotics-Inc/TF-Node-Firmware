#ifndef TF_NODE_H
#define TF_NODE_H

#include "SMAController.h"
#include "CommandProcessor.h"
#include "Settings.h"

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

    String devStatusFormatted();
    String devStatusQuick();
    String log();
    void optButtonStopFunc();

    void checkErrs;
    void errRaise(NodeError err_code);
    void errClear(NodeError err_code);
    void errClear();

    float getSupplyVolts();
    float getPotVal();

private:
    // TODO make array of SMAController
    //SMAController SMAController::muscles[SMA_CONTROLLER_CNT];
    //SMAController smaController0;
    //SMAController smaController1;
    CommandProcessor commandProcessor;
    Settings settings;
};

#endif // TF_NODE_H
