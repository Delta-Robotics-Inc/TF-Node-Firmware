#ifndef SMA_CONTROLLER_H
#define SMA_CONTROLLER_H

#include "config.hpp"
#include "networking/tfnode-messages.h"
#include "networking/NetworkInterface.h"
//#include "TFNode.hpp"
#include "networking/CommandProcessor.hpp"

#include "drivers/ResistiveController.h"
#include "drivers/GradientTracker.hpp"

class PWMSamplerDriver;  // Forward Declaration

class SMAController {
public:
    SMAController(tfnode::Device _devicePort, String _name, uint8_t _m, uint8_t _currPin, uint8_t _vLdPin, float _scaleFactor, float _offset);

    String name = "none";
    // I/O
    uint8_t mosfet_pin;
    uint8_t curr_pin;
    uint8_t vld_pin;

    void begin();
    void update();
    
    // For demo to get average resistance
    float voltage_history[10];
    float current_history[10];
    int sample_index;
    
    // SMAController Commands
    void CMD_setEnable(bool state);
    void CMD_setMode(tfnode::SMAControlMode _mode);
    void CMD_setSetpoint(tfnode::SMAControlMode _mode, float val);
    void CMD_reset();
    void CMD_setStatusMode(tfnode::DeviceStatusMode _mode, bool repeating, NetworkInterface* iface);

    void sendStatusResponse(tfnode::DeviceStatusMode mode);

    // Status Logging functions
    tfnode::SMAStatusCompact getSMAStatusCompact();  // TODO change return type to .proto def
    tfnode::SMAStatusDump getSMAStatusDump();
    String getSMAStatusReadable();
    void sendSMAStatusResponse(tfnode::DeviceStatusMode mode);

    // Getters
    float getBatteryVolts();
    float getMuscleAmps();
    float getLoadVolts();
    float getResistance();

    tfnode::SMAControlMode getMode();
    bool getEnabled() { return outputEnabled; }
    float getOutput() { return pwm_duty_percent; }
    

    float setpoint[(int)tfnode::SMAControlMode::MODE_CNT];
    ResistiveController* resController;

    // Sensor Measurements
    float readMuscleAmps();
    float readLoadVoltage();
    static float calcResistance(float V1, float V2, float I);
    //float calcResistance(float V1);
    void measure();
    static void static_measure(SMAController* m);  // Measures with SMAController handle

    // Do I put executeCommand here or implement in a separate class/interface?
    //void executeCommand(uint8_t functionCode, const uint8_t* params, size_t paramLength);

private:
    tfnode::Device devicePort;
    tfnode::SMAControllerSettings settings;  // TODO place modifiable fields within this settings handle

    // Device Status
    tfnode::DeviceStatusMode statusMode = tfnode::DeviceStatusMode::STATUS_NONE;

    // PWM and Measurement Delay during PWM while High
    const float PWM_FREQUENCY = 100;            // PWM Frequency
    const float PWM_MEASURE_DELAY_US = 400; //300     // PWMSamplerDriver will wait this long before taking a measurement after PWM goes high
    const float PWM_MEASURE_CYCLE_THRESH = 50;  // During low PWM while enabled, drive signal high after this many cycles to take current measurement
    PWMSamplerDriver* driver;
    
    // Scaling Params
    float vld_scaleFactor;
    float vld_offset;

    // STORED SENSOR AND RESISTANCE VALUES (updated with update())
    // #TODO Change to Buffer Method with BufferIndex & TransmitIndex (see journal)
    float curr_val;
    float vld_val;
    float rld_val;

    // For OHMS control mode, a PID controller will be used to control output PWM to minimize error to a setpoint resistance
    const float KP_rc = 0.05, KI_rc = 0.0, KD_rc = 0.0;

    // Control Settings
    bool outputEnabled = false;
    tfnode::SMAControlMode currentMode;
    float pwm_duty_percent = 0;  // From 0.0->0.1

    // Resistance Training Settings
    enum TrainingState { MARTENSITE, PHASE_TRANSITION, POST_AF, TRAIN_FINISH };
    TrainingState trainState = MARTENSITE;
    const float timeStep_ms = 50.0f; // Time between measurement samples.  VERIFY THIS UNTIL MEASURE PERIOD IS CONSTANT!
    float Af_mohms = -1; // Resistance at point of Af (Austenite finished temperature)
    float delta_mohms = 0;
    ///////////////////////// SET AS CHANGEABLE PARAMETERS IN FUTURE
    const float At = 500;
    const float Af = 89;
    const float L_nitinol = (18.91f + 2.0f) * 2.54f; //inches to cm
    const float D_nitinol = 0.2; //in cm
    const float resistivity_per_c_nitinol = 0.05; //micro-ohms * cm / C
    unsigned long TRAIN_TIMEOUT_MS = 4000;
    unsigned long train_timer;
    int dataPointsSinceAustenite = 0;
    /////////////////////////
    GradientTracker* resistTracker;
    int gradWidth = 5;
    int gradSize = 100;
    float flatThreshold = 0.001;
    int dir_tracker; // Keep track of direction and trigger into next state after detecting low/high
    const int DIR_TRIGGER = 3; // Wait until direction is the same for x measurements (this is highly tied to the PWM percentage...)
    //#TODO determine if DIR_TRIGGER should scale by PWM setpoint
    int currentGradIndex = -1;


    void resetTraining();
    float updateTraining(float rld_mohms);  // Will step through the state machine when in training mode


    // Utility Functions
    String getModeString();
    String getPortString();
};

#endif // SMA_CONTROLLER_H
