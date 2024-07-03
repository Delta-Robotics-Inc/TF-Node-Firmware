#include<Arduino.h>
#include "GradientTracker.hpp"

//=============================================================================
// TF Node Defaults
//=============================================================================

#define CTRL_MODE_CNT 7
enum ctrl_modes { PERCENT, VOLTS, AMPS, DEGREES, OHMS, TRAIN, MANUAL };
const String ctrl_modes_str[CTRL_MODE_CNT] = { "percent", "volts", "amps", "degrees", "ohms", "train", "manual" };
#define SIGNAL_TIMEOUT 2000  // Amount of time (ms) between receiving master commands before auto-disable
const unsigned long LOG_MS = 20;  // Time between log frames (ms)

// TF Node Commands ===========================================================
#define COMMAND_CNT 8  // Number of commands
#define PARAM_MAX 20  // Command parameter array will be this size


//=============================================================================
// TF Node Configuration
//=============================================================================

#define MUSCLE_CNT 2
#define SHIELD_VERSION "0.1-DEV"
#define FIRMWARE_VERSION "0.1"
// Voltage Read Conversion Equations
#define VRD_SCALE_FACTOR 0.08962//DEV 2 -> 0.07544
#define VRD_OFFSET -3.1365//DEV 2 -> -0.48216
#define VLD_SCALE_FACTOR_M1 0.07521
#define VLD_SCALE_FACTOR_M2 0.08734 //DEV 2 -> 0.07534  // See TF Node v0.1 DEV Analog Data Spreadsheet for these calculations
#define VLD_OFFSET_M1 -1.10407
#define VLD_OFFSET_M2 -2.85265 //DEV 2 -> -0.18299  // Ideally, value of 0 would yield 0 V, but this is not currently the case

// Pinout - make sure to match with physical board
#define VRD_PIN A4
#define STATUS_SOLID_LED 8

#define M1_MOS_TRIG 3
#define M1_CURR_RD A0
#define M1_VLD_RD A2

#define M2_MOS_TRIG 6
#define M2_CURR_RD A1
#define M2_VLD_RD A3

#define MANUAL_MODE_POT A5
#define MANUAL_MODE_THRESHOLD 0.02

// DEVICE LIMITS
#define MAX_CURRENT 30.0  // Maximum current through muscle in amps
#define MIN_VSUPPLY 7.0  // Minimum battery voltage
#define IGNORE_SUPPLY 3.0 // Treat levels below this as if the battery is disconnected

//=============================================================================
// Diagnostics
//=============================================================================

// ERROR INDECES
#define ERR_LOW_VOLT 0  // Low battery error
#define ERR_CURRENT_OF 1  // Current overflow error
byte n_error = 0b11111111;  // Error byte transmitted when requested by API call

unsigned long timeout_timer;
unsigned long log_timer;  // Logs every x ms based on logMode
unsigned long log_start;  // Time of start of log
int nodeLogMode = 0;  //0=No log; 1=Device log

float n_vSupply; // Current value of measured battery voltage.
float pot_val;  // Current value of the potentiometer 

void nodeUpdate();
String devStatusFormatted();
String devStatusQuick();
String log();

// RAISE ERROR BY XORING CURRENT ERROR WITH BITSHIFTED 1s
void errRaise(int index) {
  n_error = n_error ^ (1<<index);
  digitalWrite(STATUS_SOLID_LED, HIGH);
}
void errClear() {
  n_error = 0b11111111;
  digitalWrite(STATUS_SOLID_LED, LOW);
}

//=============================================================================
// Sensor Read Functions
//=============================================================================

float getSupplyVolts() {
  float value=0.0,samples=0.0,avg_value=0.0,raw=0.0;
    
    for (int x = 0; x < 30; x++){   // Get 30 samples
      value = analogRead(VRD_PIN);  // Read voltage divider value   
      samples += value;          // Add samples together
    }
    raw=samples/30.0;  // Taking Average of Samples

    float volts = raw * VRD_SCALE_FACTOR + VRD_OFFSET;
    return volts;
    //return raw;
}

// Returns a value (0.0->1.0) based on pot value
float getPotVal() {
    float value=0.0,samples=0.0,avg_value=0.0,raw=0.0;
    
    for (int x = 0; x < 3; x++){   // Get 30 samples
      value = analogRead(MANUAL_MODE_POT);  // Read potentiometer
      samples += value;          // Add samples together
    }
    raw=samples/3.0;  // Taking Average of Samples

    float percent = raw / 1024.0;
    percent = percent > MANUAL_MODE_THRESHOLD ? percent : 0.0; // Limit lowest measurable value
    return percent;
}

//=============================================================================
// Unit Conversion Functions
//=============================================================================

//CONVERT PERCENT BETWEEN 0:1 TO POSITIVE PWM
int percentToPWM(float speed) {
  int pwm = abs(speed) * 255;
  pwm = max(0, min(pwm, 255));  // Clamp between [0:255]
  return pwm;
}

//=============================================================================
// Unit Classes
//=============================================================================

//Every muscle is controlled through open-loop (percent) or closed-loop (volt, amp, degree, sensor**) mode
class TF_Muscle {
  public:
    String name = "none";
    // I/O
    uint8_t mosfet_pin;
    uint8_t curr_pin;
    uint8_t vld_pin;
    
    // Scaling Params
    float vld_scaleFactor;
    float vld_offset;

    // STORED SENSOR AND RESISTANCE VALUES (updated with update())
    // #TODO Change to Buffer Method with BufferIndex & TransmitIndex (see journal)
    float curr_val;
    float vld_val;
    float rld_val;

    // CONTROL MODE
    bool enabled = false;
    ctrl_modes mode = PERCENT;
    float setpoint[CTRL_MODE_CNT];
    int pwm_val = 0;  // PWM_VAL is the actual outputted duty cycle to the mosfets.  It's behavior will update based on the control mode

    // RESISTANCE CONTROL MODE
    enum TrainingState { MARTENSITE, PHASE_TRANSITION, POST_AF, TRAIN_FINISH };
    TrainingState trainState = MARTENSITE;
    const float timeStep_ms = 25.0f; // Time between measurement samples.  VERIFY THIS UNTIL MEASURE PERIOD IS CONSTANT!
    float Af_mohms = -1; // RESISTANCE AT POINT OF Af (Austenite finished temperature)
    float delta_mohms = 0;
    ///////////////////////// SET AS CHANGEALBE PARAMETERS IN FUTURE
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
    int gradWidth = 3;
    int gradSize = 100;
    float flatThreshold = 0.01;
    int dir_tracker; // Keep track of direction and trigger into next state after detecting low/high
    const int DIR_TRIGGER = 10; // Wait until direction is the same for x measurements (this is highly tied to the PWM percentage...)
    //#TODO determine if DIR_TRIGGER should scale by PWM setpoint
    int currentGradIndex = -1;

    // PULSE SETUP
    bool pulse_state = false; //on/off state of pulse cycle
    float pulse_on_time = 1.0f;
    float pulse_off_time = 1.0f;
    int pulse_cnt = -2; //-2 for continuous mode, -1 for infinite pulse, >0 for finite pulse
    unsigned long pulse_timer;

    // statusFormatted/LOGGING MODE
    int logMode = 0; // 0=No log; 1=Constant logging (calls statusFormatted every update); 2=Quick Logging (not formatted)

    static TF_Muscle* muscles[MUSCLE_CNT];

    void init(String _name, uint8_t _m, uint8_t _currPin, uint8_t _vLdPin, float _scaleFactor, float _offset) {
      name = _name;
      mosfet_pin = _m;
      curr_pin = _currPin;
      vld_pin = _vLdPin;
      vld_scaleFactor = _scaleFactor;
      resistTracker = new GradientTracker(gradWidth, gradSize, flatThreshold, timeStep_ms);
      vld_offset = _offset;
      pinMode(mosfet_pin, OUTPUT);
    }

    void update() {

      //#TODO: Handle pulse condition

      // Write to muscle if enabled
      if(enabled) {
        // Handle behavior of each control mode
        switch (mode)
        {
          case PERCENT:
            pwm_val = percentToPWM(setpoint[PERCENT]);
            break;
          case VOLTS:
            pwm_val = percentToPWM(setpoint[VOLTS] / n_vSupply);  // When controlling for volts, the ratio of setpoint/supply will be percentage of power to deliver
            break;
          //TODO: Implement other control modes
          case AMPS:     // Need to implement PID loop
            pwm_val = percentToPWM(setpoint[AMPS] / curr_val); // Simplest current control is just pulsing the peak current at a rate which matches setpoint current
            break;
          //case DEGREES:  // Need to implement equation to track/return muscle temp (temp will behave different based on 2 conditions: below/above Af)
          //case OHMS:     // Need to implement PID loop (but how to implement with hysterisis curve?)
          // Eventually add position control (requires force to be known)
          case TRAIN:
            pwm_val = updateTraining(rld_val);  // Convert to mohms
          break;
          case MANUAL:
            pwm_val = percentToPWM(pot_val); // Manual mode uses the potentiometer if connected
          break;
          default:
            pwm_val = percentToPWM(0);
            break;
        }
      }
      else {
        pwm_val = percentToPWM(0); // Disabled means write 0% power
        // RESET TRAINING STATE
        //trainState = MARTENSITE;
      }

      analogWrite(mosfet_pin, pwm_val);  // Write pwm to mosfet m
      measure(); // Update sensor values

      // CURRENT OVERFLOW ERROR CONDITION
      if(curr_val > MAX_CURRENT) {
        errRaise(ERR_CURRENT_OF);
        setEnable(false); //disable muscle
      }
    }

    void resetTraining() {
        trainState = MARTENSITE;
        resistTracker->reset();
        dir_tracker = 0;
        //Af_mohms = 0;
        dataPointsSinceAustenite = 0;
        currentGradIndex = resistTracker->oldestGradIndex;
        train_timer = millis(); //reeset timer
    }

    // Will step through the state machine when in training mode
    int updateTraining(float rld_mohms) {

      // Safety auto-timeout feature
      //if(millis() - train_timer > TRAIN_TIMEOUT_MS) {
      //  setEnable(false);
      //  return 0;
      //}

      switch(trainState) {
        case MARTENSITE: {  // STATE: Heating (Martensite phase)
          // I am thinking that a rolling average would be good. Track rolling average of 10 samples and when it has been decreasing for a few cycles then go to next state
          resistTracker->addData(rld_mohms);

          if(resistTracker->oldestGradIndex != currentGradIndex) {
            currentGradIndex = resistTracker->oldestGradIndex;
            // Get the gradient direction from data tracker
            GradientDirection dir = resistTracker->getCurrentDir();
            dir_tracker += dir == POSITIVE ? 1 :
                            dir == NEGATIVE ? -1 : 0;

            // Condition for next state (resistance decreases for DIR_TRIGGER intervals)
            if(dir_tracker > 0)
              dir_tracker = 0;
            else if(dir_tracker < -DIR_TRIGGER) {
              trainState = PHASE_TRANSITION;
              dir_tracker = 0;
            }
          }
          //return setpoint pwm value
          return percentToPWM(setpoint[TRAIN]);
        }
        case PHASE_TRANSITION: {  // STATE: Heating (Pre Austenite Finished Temp)
          // The same thing with a rolling average; wait until it has been increasing for a few cycles and find bottom of dip (set bottom to Af_mohms)
          resistTracker->addData(rld_mohms);
          dataPointsSinceAustenite++;

          if(resistTracker->oldestGradIndex != currentGradIndex) {
            currentGradIndex = resistTracker->oldestGradIndex;
            // Get the gradient direction from data tracker
            GradientDirection dir = resistTracker->getCurrentDir();
            dir_tracker += dir == POSITIVE ? 1 :
                          dir == NEGATIVE ? -1 : 0;

            // Condition for next state (wait for dir_tracker to increase over threshold)
            if(dir_tracker < 0)
              dir_tracker = 0;
            else if(dir_tracker > DIR_TRIGGER) {
              trainState = POST_AF;
              // #TODO Convert this function to getLocalMinimum from current position backward.  This oldestGradIndex should get current position
              int startIndex = resistTracker->oldestGradIndex + ((dataPointsSinceAustenite / (gradWidth-1)) + 1);
              startIndex = startIndex < 0 ? 0 : startIndex; // Get oldest data if transition was lost
              Af_mohms = resistTracker->getLocalMinimum(startIndex, resistTracker->oldestGradIndex);
              dir_tracker = 0;
            }
          }
          // Return setpoint pwm value
          return percentToPWM(setpoint[TRAIN]);
        }
        case POST_AF: {  // STATE: Heating (Post Austenite finished Temp)
          // Compare change in ohms to desired delta_mohms
          float delta_mohms_real = (rld_mohms - Af_mohms); // mohms
          // When ohms exceeds (Af_ohms + delta_ohms), start training timer. Flash LED while at temp
          // Return PID (with max value) to mitigate error between desired temperature and set temperature

          // Timer Condition
          return percentToPWM(0.05); // Just cut power for now.
          // #TODO add resistance control measures at this point
        }
        // STATE: Finished
        case TRAIN_FINISH: {
          setEnable(false); // When time exceeds training time, hold LED solid, disable, and do not enable until told to do so
          // Set LED color
          return 0;
        }
      }
      return 0;
    }

    // Based on the logMode, return data to be logged
    String log() {
      if(logMode == 1)
        return statusFormatted();
      else if(logMode == 2)
        return statusQuick();
      else
        return "";
    }

    void setLogMode(int _mode) {
      logMode = _mode;
    }

    void setEnable(bool state) {
      enabled = state;

      if(mode == TRAIN && enabled)
        resetTraining();
    }

    void setMode(ctrl_modes _mode) {
      mode = _mode;

      if(mode == TRAIN) {
        trainState = MARTENSITE;
      }
    }

    void setSetpoint(ctrl_modes _mode, float val) {
        setpoint[_mode] = val;

        // Update delta milliohms for training mode
        if(_mode == TRAIN)
          delta_mohms = ((At - Af) * L_nitinol / (PI * pow(D_nitinol / 2, 2)) * resistivity_per_c_nitinol) * 0.001; //results in milliohms
    }

    void stop() {
      setEnable(false);
      setSetpoint(VOLTS, 0);
      setSetpoint(AMPS, 0);
      setSetpoint(DEGREES, 0);
    }

    void reset() {
      stop();
      setMode(PERCENT);
      // Reset timer and pulse vals*****************************************
    }

    String statusFormatted() {
      String stat_str = "MUSCLE: " + name + '\n';
      stat_str += "| mosfet: " + String(mosfet_pin) + '\n';
      stat_str += "| curr_pin: " + String(curr_pin) + '\n';
      String en_str = enabled ? "true" : "false";
      stat_str += "| enabled: " + String(en_str) + '\n';
      stat_str += "| mode: " + ctrl_modes_str[mode] + '\n';

      String setpoint_str;
      switch (mode)
        {
        case PERCENT:
            setpoint_str = String(setpoint[PERCENT]);
            break;
        case VOLTS:
            setpoint_str = String(setpoint[VOLTS]) + " V";
            break;
        case AMPS:
            setpoint_str = String(setpoint[AMPS]) + " A";
            break;
        case DEGREES:
            setpoint_str = String(setpoint[DEGREES]) + "°";
            break;
        case TRAIN:
            setpoint_str = String(setpoint[TRAIN]);
            break;
        default:
            setpoint_str = String(setpoint[mode]) + " " + String(mode);
            break;
        }
      stat_str += "| setpoint: " + setpoint_str + '\n';
      stat_str += "| pwm-val: " + String(pwm_val) + '\n';
      //stat_str += "| current: " + String(analogRead(curr_pin)) + " (native units) \n";
      stat_str += "| current: " + String(curr_val) + " A \n";
      stat_str += "| load-volts: " + String(vld_val) + " V \n";
      stat_str += "| load-resist: " + String(rld_val) + " mΩ \n";
      stat_str += "| Af_resist: " + String(Af_mohms) + " mΩ \n";
      stat_str += "| delta_ohms: " + String(delta_mohms) + " mΩ \n";
      stat_str += "| trainState: " + String(trainState) + "\n";

      return stat_str;
    }

    // TODO: Switch this function to binary protocol
    // TODO: Make logMode a binary key that outputs only certain variables
    String statusQuick() {
      String stat_str = name + " "; //4
      stat_str += String(mosfet_pin) +  " ";
      stat_str += String(curr_pin) + " ";
      stat_str += String(enabled) + " ";
      stat_str += String(mode) + " ";
      stat_str += String(setpoint[mode]) + " ";
      stat_str += String(pwm_val) + " ";
      stat_str += String(curr_val) + " ";
      stat_str += String(vld_val) + " ";
      stat_str += String(rld_val) + " ";
      stat_str += String(Af_mohms) + " ";
      stat_str += String(delta_mohms) + " ";
      stat_str += String(trainState) + " ";
      return stat_str;
    }

    //********************************************************************************************************************
    //https://www.engineersgarage.com/acs712-current-sensor-with-arduino/
    float getMuscleAmps() {
      float AcsValue=0.0,Samples=0.0,AvgAcs=0.0,raw=0.0;
    
      for (int x = 0; x < 3; x++){       // Get 10 samples
        waitForPWMHigh();
        AcsValue = analogRead(curr_pin);  // Read current sensor values   
        Samples = Samples + AcsValue;     // Add samples together
      }
      raw = Samples / 3.0;  // Taking Average of Samples
    
      float amps = ((raw * (5.0 / 1024.0)) - 2.5)/0.100; //5.0/1024 is conversion ratio of volts/native unit. 2.5 v is 0 A due to positive and negative ability
      
      if(amps < 0.0 && amps > -1.0f)
        return 0.0f;  // Return 0 if close to 0 (prevent negative resistance)
      return amps;
    }

    // Returns the voltage (VLD) after the load (muscle) -> used with getMuscleAmps() and getBatteryVolts() to calculate resistance
    float getLoadVoltage() {
      float value=0.0,samples=0.0,avg_value=0.0,raw=0.0;
    
      for (int x = 0; x < 3; x++){   // Get 10 samples
        waitForPWMHigh();
        value = analogRead(vld_pin);  // Read current sensor values   
        samples += value;          // Add samples together
      }
      raw = samples / 3.0;  // Taking Average of Samples

      float volts = raw * vld_scaleFactor + vld_offset; // Values are attained experimentally with known input voltages
      if(volts < 0.0 && volts > -2.0)
        return 0.0f; // Return 0 volts for values close to 0
      return volts;
      //return raw;
    }

    // V=IR -> R=V/I
    static float calcResistMohms(float V1, float V2, float I) {
      if(I == 0.0f)
        return 9999999999.0f; // Really high number
      return 1000 * abs((V1-V2) / I);
    }

    void measure() {
      // ONLY MEASURE ON HIGH VALUE OF PWM
      // If pwm is low, wait a max of 3 ms (This assumes that PWM frequency is 450 Hz)
      // If pwm does not switch during this time, take the measurement anyways
      vld_val = getLoadVoltage();    
      curr_val = getMuscleAmps();
      rld_val = calcResistMohms(n_vSupply, vld_val, curr_val);
    }

    void waitForPWMHigh() {
      if(enabled && pwm_val > 0 && !digitalRead(mosfet_pin)) {
        unsigned long measure_start = millis();
        unsigned long measure_timeout = 1000; //timeout after 1000 ms
        while((millis() - measure_start < measure_timeout) && !digitalRead(mosfet_pin)) 
        { /* Do nothing */ }
      }
      return;
    }

    //=============================================================================
    // Muscle Control/Config Functions
    //=============================================================================

    static void setLogModeAll(int _mode) {
      for(int m=0; m < MUSCLE_CNT; m++) { muscles[m]->setLogMode(_mode); };
    }

    //enable/disable all muscles
    static void setEnableAll(bool state) {
      for(int m=0; m < MUSCLE_CNT; m++) { muscles[m]->setEnable(state); };
    }

    static void setModeAll(ctrl_modes _mode) {
      for(int m=0; m < MUSCLE_CNT; m++) { muscles[m]->setMode(_mode); };
    }

    static void setSetpointAll(ctrl_modes _mode, float setpoint) {
      for(int m=0; m < MUSCLE_CNT; m++) { muscles[m]->setSetpoint(_mode, setpoint); };
    }

    static String statusFormattedAll() {
      String stat_str = "";
      for(int m=0; m < MUSCLE_CNT; m++) { stat_str += muscles[m]->statusFormatted(); };
      return stat_str;
    }

    static void resetAll() {
      for(int m=0; m < MUSCLE_CNT; m++) { muscles[m]->reset(); };
    }

    static void stopAll() {
      for(int m=0; m < MUSCLE_CNT; m++) { muscles[m]->stop(); };
    }

    //loop through updating all muscles
    static void updateMuscles() {
      for(int m=0; m < MUSCLE_CNT; m++) { muscles[m]->update(); }
    }

// Init muscles
} m_1, m_2;  // Update parameter MUSCLE_CNT when new muscle is added/removed


// Command is the primitive object sent from a master device to this Node
// Note, commands are sent top-down from (Master Device -> Master Node -> Slave Nodes)
// Inherit from this class to create a new command (see other command classes below)
class Command {
  public:
    String name = "default";   // Used to identify command being run
    byte code = 0x00;          // Every command will be assigned an 8-bit code.  No code should repeat.
    String params[PARAM_MAX];  // Parameters are passed into the referenced execute() function
    int param_cnt;             // Incremented when a new command is added

    Command() {}  // Empty constructor

    explicit Command(String _name, byte _code) {
      name = _name;
      code = _code;
    }
    
    // Adds parameter to command's parameter "list"
    void addParameter(String _param) {
        params[param_cnt] = _param; 
        param_cnt++; 
        //Serial.println("Found param: " + _param);
    }

    // Displays the given name and parameters passed into command
    void serialDisplayInput() {
      Serial.print("Name: ");
      Serial.print(name);
      Serial.print(" Params: ");
      for(int i = 0; i < param_cnt; i++) {
        Serial.print(params[i] + ' ');
      }
      Serial.println(' ');
    }
    
    // Must be overriden by inherited members of this command
    virtual void execute() { 
      serialDisplayInput();  // For now, only execution will be displaying back what was passed in
    }

    static Command* c_commands[COMMAND_CNT];  // Array of all commands to iterate over
    // Make sure to update COMMAND_CNT parameter when adding a new command
    // NOTE: would be nice to create a "Command Queue" for backlogging commands -> Need to create state machine for Node system

    // Attempts to locate command by name from list of commands
    static Command* getCommandByName(String _name, bool &success) {
      _name.toLowerCase();
      for(int i=0; i<COMMAND_CNT; i++) {
        //Serial.println(c_commands[i].name);
        if(c_commands[i]->name == _name) {
          success = true;
          return c_commands[i];
        }
      }
      // Command not found, return default
      success = false;
      return c_commands[0];
    }

    // Parses a user input device into the integer equivalent
    static int parseDeviceStr(String device) {
      int d = (device == "m1" || device == "0") ? 0 :
              (device == "m2" || device == "1") ? 1 :
              (device == "m3" || device == "2") ? 2 : 0;
      return d;
    }

    static Command* parseCommandStr(String consoleIn, bool &success) {
        // Converts an input string to the corresponding command and parameters
        // Input string should be "name par1 par2 par3 .. parn"
        Serial.println("========================================");
        Serial.print("<serial-in> ");
        Serial.println(consoleIn);
        int param_index = consoleIn.indexOf(' ');  // Start of parameters
        param_index = param_index==-1 ? consoleIn.length() : param_index;  // If no spaces were found, parse entire input as name

        String _name = consoleIn.substring(0, param_index);
        //Serial.print("Found name: ");
        //Serial.println(_name);
        Command* cmd = getCommandByName(_name, success);  // Whether command was found
        cmd->param_cnt = 0;  // Effectively clears params

        // Continue if command was found and has parameters to parse
        if(success && param_index != consoleIn.length()) {
          String param_str = consoleIn.substring(param_index+1, consoleIn.length());
          param_index = param_str.indexOf(' ');  // index of next param start
          
          // Iterate through parameters, splitting at ' ' characters, and add to list of params
          for(int i = 0; i < PARAM_MAX && param_index != -1; i++) {
            String param = param_str.substring(0, param_index);
            param_str = param_str.substring(param_index+1, param_str.length());
            cmd->addParameter(param);  // Add parameter to array of parameters
            param_index = param_str.indexOf(' ');  // Index of next param start
          }

          if(cmd->param_cnt < PARAM_MAX)
            cmd->addParameter(param_str);  // Add last parameter since another space will not be located
        }
        return cmd;
    }
} c_default;


//=============================================================================
// Implemented Commands
//=============================================================================

class Reset : public Command {
  public:
    explicit Reset() : Command("reset", 0xFF) {
      // Additional init
    }
    void execute() override {
       Command::execute();
       Serial.println("executing reset...");
      if(param_cnt == 0) {
        TF_Muscle::resetAll();
        return;
      }

      if(params[0] == "all") {
         TF_Muscle::resetAll();
      }
      else {
         int m = Command::parseDeviceStr(params[0]);
         TF_Muscle::muscles[m]->reset();
      }
    }
} c_reset;

class SetEnable : public Command {
  public:
    explicit SetEnable() : Command("set-enable", 0x01) {
      // Additional init
    }
    void execute() override {
      Command::execute();
      Serial.println("executing set-enable...");
      // Additional execute
      String device = params[0];
      String state_str = params[1];
      state_str.toLowerCase();
      bool state = state_str == "true" ? true : false;

      if(device == "all") {
         TF_Muscle::setEnableAll(state);
      }
      else {
         int m = Command::parseDeviceStr(device);
         TF_Muscle::muscles[m]->setEnable(state);
      }
    }
} c_setEnable;

class SetMode : public Command {
  public:
    explicit SetMode() : Command("set-mode", 0x02) {
      // Additional init
    }
    void execute() override {
      Command::execute();
      Serial.println("executing set-mode...");
      // Additional execute
      String device = params[0];
      String mode_str = params[1];
      mode_str.toLowerCase();

      // Either wrap into parseMode() func or deprecate with new serial comms
      ctrl_modes mode = mode_str == ctrl_modes_str[PERCENT] ? PERCENT :
                        mode_str == ctrl_modes_str[VOLTS] ? VOLTS :
                        mode_str == ctrl_modes_str[AMPS] ? AMPS :
                        mode_str == ctrl_modes_str[DEGREES] ? DEGREES :
                        mode_str == ctrl_modes_str[OHMS] ? OHMS : 
                        mode_str == ctrl_modes_str[TRAIN] ? TRAIN :
                        mode_str == ctrl_modes_str[MANUAL] ? MANUAL : PERCENT;  // Default is percent

      if(device == "all") {
         TF_Muscle::setModeAll(mode);
      }
      else {
         int m = Command::parseDeviceStr(device);
         TF_Muscle::muscles[m]->setMode(mode);
      }
    }
} c_setMode;

class SetSetpoint : public Command {
  public:
    explicit SetSetpoint() : Command("set-setpoint", 0x03) {
      // Additional init
    }
    void execute() override {
      Command::execute();
      Serial.println("executing set-setpoint...");
      // Additional execute
      String device = params[0];
      String mode_str = params[1];
      mode_str.toLowerCase();
      float setpoint = params[2].toFloat();

      // Either wrap into parseMode() func or deprecate with new serial comms
      ctrl_modes mode = mode_str == ctrl_modes_str[PERCENT] ? PERCENT :
                        mode_str == ctrl_modes_str[VOLTS] ? VOLTS :
                        mode_str == ctrl_modes_str[AMPS] ? AMPS :
                        mode_str == ctrl_modes_str[DEGREES] ? DEGREES :
                        mode_str == ctrl_modes_str[OHMS] ? OHMS : 
                        mode_str == ctrl_modes_str[TRAIN] ? TRAIN : // #TODO Actually implement setpoint of "manual" mode
                        mode_str == ctrl_modes_str[MANUAL] ? MANUAL : PERCENT; // Default is percent
      if(device == "all") {
         TF_Muscle::setSetpointAll(mode, setpoint);
      }
      else {
         int m = Command::parseDeviceStr(device);
         TF_Muscle::muscles[m]->setSetpoint(mode, setpoint);
      }
    }
} c_setSetpoint;

class Status : public Command {
  public:
    explicit Status() : Command("status", 0x04) {
      // Additional init
    }
    void execute() override {
      Command::execute();
      Serial.println("executing status...");

      // Check for optional constant logging mode
      if(param_cnt > 1) {
        int logMode = params[1].toInt();
      }

      // Display Status of all devices
      if(param_cnt == 0 || params[0] == "all") {
        Serial.print(devStatusFormatted());
        Serial.println(TF_Muscle::statusFormattedAll());
        return;
      }
      if(params[0] == "node") {
        Serial.println(devStatusFormatted());
        return;
      }

      // Display Status of specified device
      int m = Command::parseDeviceStr(params[0]);
      Serial.print(TF_Muscle::muscles[m]->statusFormatted());
      Serial.println();
    }
} c_status;

class Stop : public Command {
  public:
    explicit Stop() : Command("stop", 0x05) {
      // Additional init
    }
    void execute() override {
      Command::execute();
      Serial.println("executing stop...");
      if(param_cnt == 0) {
        TF_Muscle::stopAll();
        return;
      }

      // Additional execute
      if(params[0] == "all") {
         TF_Muscle::stopAll();
      }
      else {
         int m = Command::parseDeviceStr(params[0]);
         TF_Muscle::muscles[m]->stop();
      }
    }
} c_stop;

class LogMode : public Command {
  public:
    explicit LogMode() : Command("log-mode", 0x06) {
      // Additional init
    }
    void execute() override {
      Command::execute();
      Serial.println("executing log-mode...");

      int logMode = 0;  // Default set to 0

      // Set log mode if user inputted
      if(param_cnt > 1) {
        logMode = params[1].toInt();
      }

      log_start = millis();  // Changing log mode will reset log start time

      // Set log mode of all devices
      if(param_cnt == 0 || params[0] == "all") {
        nodeLogMode = logMode;
        TF_Muscle::setLogModeAll(logMode);
        return;
      }
      if(params[0] == "node") {
        nodeLogMode = logMode;
        return;
      }

      // Set log mode of specific muscle
      int m = Command::parseDeviceStr(params[0]);
      TF_Muscle::muscles[m]->setLogMode(logMode);
    }
} c_logMode;

//#TODO: Handle pulse condition in command