#include<Arduino.h>
#include "GradientTracker.hpp"
#include "PWMSamplerDriver.h"
#include "ResistiveController.h"

//=============================================================================
// TF Node Defaults
//=============================================================================

#define CTRL_MODE_CNT 7
enum ctrl_modes { PERCENT, VOLTS, AMPS, DEGREES, OHMS, TRAIN, MANUAL };  // TODO add Length and Force
const String ctrl_modes_str[CTRL_MODE_CNT] = { "percent", "volts", "amps", "degrees", "ohms", "train", "manual" };
//#define SIGNAL_TIMEOUT 2000  // Amount of time (ms) between receiving master commands before auto-disable
//const unsigned long LOG_MS = 20;  // Time between log frames (ms)

// TF Node Commands
#define COMMAND_CNT 8  // Number of commands
#define PARAM_MAX 20  // Command parameter array will be this size


//=============================================================================
// TF Node Board Configuration
//=============================================================================

// #define MUSCLE_CNT 2
// #define SHIELD_VERSION "1.0"
// #define FIRMWARE_VERSION "1.0.0"

// #define VCC 5.0  // [V] Maximum readable value.  Should be 5.0 V but will vary slightly with the voltage regulator output

// Voltage Read Conversion Equations
// #define VRD_SCALE_FACTOR 11 * VCC/1023.0  // Derived from resistor divider circuit with R1=10k and R2=1k
// #define VRD_OFFSET 0
// #define VLD_SCALE_FACTOR_M1 11 * VCC/1023.0
// #define VLD_SCALE_FACTOR_M2 11 * VCC/1023.0
// #define VLD_OFFSET_M1 0
// #define VLD_OFFSET_M2 0

// Current Sense Amplifier -> TI INA301
// #define R_SNS 0.0005  // [ohms]
// #define AMP_GAIN 100  // [V/V] Vin/Vout gain of amplifier

// Pinout - make sure to match with physical board
// #define VRD_PIN A4
// #define STATUS_SOLID_LED 8
// #define STATUS_RGB_RED 9
// #define STATUS_RGB_GREEN 10
// #define STATUS_RGB_BLUE 11

// #define M1_MOS_TRIG 3
// #define M1_CURR_RD A0
// #define M1_VLD_RD A2
// #define M1_ALERT 12

// #define M2_MOS_TRIG 6
// #define M2_CURR_RD A1
// #define M2_VLD_RD A3
// #define M2_ALERT 13

// #define AUX_BUTTON 7
// #define MANUAL_MODE_POT A5
// #define MANUAL_MODE_THRESHOLD 0.02

// Device Limits
// #define MAX_CURRENT 80.0  // [A] Maximum current through muscle in amps
// #define MIN_VSUPPLY 7.0  // [V] Minimum battery voltage
// #define IGNORE_SUPPLY 3.0 // [V] Treat levels below this as if the battery is disconnected

//=============================================================================
// Diagnostics
//=============================================================================

// ERROR INDECES
// #define ERR_LOW_VOLT 0  // Low battery error
// #define ERR_CURRENT_OF 1  // Current overflow error
// #define ERR_EXTERNAL_INTERRUPT 2
// byte n_error = 0b11111111;  // Error byte transmitted when requested by API call

// unsigned long timeout_timer;
// unsigned long log_timer;  // Logs every x ms based on logMode
// unsigned long log_start;  // Time of start of log
// int nodeLogMode = 0;  //0=No log; 1=Device log

// float n_vSupply; // Current value of measured battery voltage.
// float pot_val;  // Current value of the potentiometer 

// void nodeUpdate();
// String devStatusFormatted();
// String devStatusQuick();
// String log();
// void optButtonStopFunc();

// Raise Error by anding current error val with 0 bitshifted by index
void errRaise(int index) {
  n_error = n_error & (0b11111111 ^ (1<<index));
  digitalWrite(STATUS_SOLID_LED, HIGH);
}
void errClear() {
  n_error = 0b11111111;
  digitalWrite(STATUS_SOLID_LED, LOW);
}
void errClear(int index) {
  n_error = n_error | (1<<index);
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
    //float temp_sensor_val = -0.0499 * (raw)*(raw) + 97.084 * (raw) - 47088;
    //return temp_sensor_val;
    //return raw; //TODO Switch back to percent after running tests with this value
}

//=============================================================================
// Unit Conversion Functions
//=============================================================================

//CONVERT PERCENT BETWEEN 0:1 TO POSITIVE PWM
/*int percentToPWM(float speed) {
  int pwm = abs(speed) * 255;
  pwm = max(0, min(pwm, 255));  // Clamp between [0:255]
  return pwm;
}*/

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

    // PWM and Measurement Delay during PWM while High
    const float PWM_FREQUENCY = 100;  // PWM Frequency
    const float PWM_MEASURE_DELAY_US = 300;  // PWMSamplerDriver will wait this long before taking a measurement after PWM goes high
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
    ResistiveController* resController;
    const float KP_rc = 0.05, KI_rc = 0.0, KD_rc = 0.0;

    // CONTROL MODE
    bool enabled = false;
    ctrl_modes mode = PERCENT;
    float setpoint[CTRL_MODE_CNT];
    float pwm_duty_percent = 0;  // From 0.0->0.1

    // RESISTANCE TRAINING CONTROL MODE
    enum TrainingState { MARTENSITE, PHASE_TRANSITION, POST_AF, TRAIN_FINISH };
    TrainingState trainState = MARTENSITE;
    const float timeStep_ms = 50.0f; // Time between measurement samples.  VERIFY THIS UNTIL MEASURE PERIOD IS CONSTANT!
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
    int gradWidth = 5;
    int gradSize = 100;
    float flatThreshold = 0.001;
    int dir_tracker; // Keep track of direction and trigger into next state after detecting low/high
    const int DIR_TRIGGER = 3; // Wait until direction is the same for x measurements (this is highly tied to the PWM percentage...)
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
      //pinMode(mosfet_pin, OUTPUT);

      // Initialize resistive PID controller
      resController = new ResistiveController(0.0, KP_rc, KI_rc, KD_rc);

      // Initialize pwm driver which will handle pwm on the mosfet pin and sampling the sensors
      driver = new PWMSamplerDriver(PWM_FREQUENCY, 0.0f, mosfet_pin, PWM_MEASURE_DELAY_US, PWM_MEASURE_CYCLE_THRESH, static_measure, this);
    }

    void update() {

      //#TODO: Handle pulse condition

      // Write to muscle if enabled
      if(enabled) {
        // Handle behavior of each control mode
        switch (mode)
        {
          case PERCENT:
            pwm_duty_percent = setpoint[PERCENT];
            break;
          case VOLTS:
            pwm_duty_percent = setpoint[VOLTS] / n_vSupply;  // When controlling for volts, the ratio of setpoint/supply will be percentage of power to deliver
            break;
          //TODO: Implement other control modes
          case AMPS:     // Need to implement PID loop
            pwm_duty_percent = setpoint[AMPS] / curr_val; // Simplest current control is just pulsing the peak current at a rate which matches setpoint current
            break;
          //case DEGREES:  // Need to implement equation to track/return muscle temp (temp will behave different based on 2 conditions: below/above Af)
          case OHMS:     // Need to implement PID loop (but how to implement with hysterisis curve?)
          {
            resController->setSetpoint(setpoint[OHMS]);  // Update setpoint of pid to setpoint of this device for OHMS mode
            resController->update(rld_val); // Update pid controller
            pwm_duty_percent = resController->getOutput();
          }
          break;
          // Eventually add position control (requires force to be known)
          case TRAIN:
            pwm_duty_percent = updateTraining(rld_val);  // Convert to mohms
          break;
          case MANUAL:
            pwm_duty_percent = pot_val; // Manual mode uses the potentiometer if connected
          break;
          default:
            pwm_duty_percent = 0;
            break;
        }
      }
      else {
        pwm_duty_percent = 0; // Disabled means write 0% power
        // RESET TRAINING STATE
        //trainState = MARTENSITE;
      }

      //analogWrite(mosfet_pin, pwm_val);  // Write pwm to mosfet m
      //measure(); // Update sensor values
      driver->setDutyCyclePercent(pwm_duty_percent, enabled);  // The PWMDriver will hande analogWrite() and measuring sensors
      //Serial.println(driver->min_duty_percent);
      //Serial.println(driver->cycle_threshold);
      //Serial.println(driver->pulse_cycle_count);

      // CURRENT OVERFLOW ERROR CONDITION
      if(curr_val > MAX_CURRENT) {
        errRaise(ERR_CURRENT_OF);
        //setEnable(false); //disable muscle
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
    float updateTraining(float rld_mohms) {

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
          return setpoint[TRAIN];
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
          return setpoint[TRAIN];
        }
        case POST_AF: {  // STATE: Heating (Post Austenite finished Temp)
          // Compare change in ohms to desired delta_mohms
          float delta_mohms_real = (rld_mohms - Af_mohms); // mohms
          // When ohms exceeds (Af_ohms + delta_ohms), start training timer. Flash LED while at temp
          // Return PID (with max value) to mitigate error between desired temperature and set temperature

          // Timer Condition
          return 0.01; // Just cut power for now.
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

      // Clear the external disable error
      if(enabled)
        errClear(ERR_EXTERNAL_INTERRUPT);

      if((mode == TRAIN || mode == OHMS) && enabled)
        resetTraining();
        resController->Reset();
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
      setSetpoint(PERCENT, 0);
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
            setpoint_str = String(setpoint[PERCENT], 6);
            break;
        case VOLTS:
            setpoint_str = String(setpoint[VOLTS], 6) + " V";
            break;
        case AMPS:
            setpoint_str = String(setpoint[AMPS], 6) + " A";
            break;
        case DEGREES:
            setpoint_str = String(setpoint[DEGREES], 6) + "°";
            break;
        case TRAIN:
            setpoint_str = String(setpoint[TRAIN], 6);
            break;
        case OHMS:
            setpoint_str = String(setpoint[OHMS], 6) + " mΩ";
            break;
        default:
            setpoint_str = String(setpoint[mode], 6) + " " + String(mode);
            break;
        }
      stat_str += "| setpoint: " + setpoint_str + '\n';
      stat_str += "| pwm-val: " + String(pwm_duty_percent) + '\n';
      //stat_str += "| current: " + String(analogRead(curr_pin)) + " (native units) \n";
      stat_str += "| current: " + String(curr_val, 6) + " A \n";
      stat_str += "| load-volts: " + String(vld_val, 6) + " V \n";
      stat_str += "| load-resist: " + String(rld_val, 6) + " mΩ \n";
      stat_str += "| Af_resist: " + String(Af_mohms, 6) + " mΩ \n";
      stat_str += "| delta_ohms: " + String(delta_mohms, 6) + " mΩ \n";
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
      stat_str += String(setpoint[mode], 6) + " ";
      stat_str += String(pwm_duty_percent) + " ";
      stat_str += String(curr_val, 6) + " ";
      stat_str += String(vld_val, 6) + " ";
      stat_str += String(rld_val, 6) + " ";
      stat_str += String(Af_mohms, 6) + " ";
      stat_str += String(delta_mohms, 6) + " ";
      stat_str += String(trainState) + " ";
      return stat_str;
    }

    // Using current amplifier: https://www.ti.com/product/INA301?bm-verify=AAQAAAAJ_____3vvQVqhDs6dN-q2F7TfcduLcXGPYHN_yemLpkjLOSFozABq8zjBc6aZ1bHQMXuQcVRel2S374cevoKQ14rUg6LyRHkRgLS507wvwpDNZhvE-ZWK9hVspPwTC71ayCc3-WGCwn-CquFEWjRJKDJvsJXJVh4eel_qHn8Kcueux4PnQ39dUIMN51MZvpS9lZ7o7K9nCUaPUoq0s-TIxAvmatQPs4K61R7LI2pVSV_YWOOVyAT6TcwQ1i3h7ZRzdiO2DlOvNC6KJ8hoXFBhaxkZCcEV-xI63CAnv-xaEy2seyI
    float getMuscleAmps() {
      float AcsValue=0.0,Samples=0.0,AvgAcs=0.0,raw=0.0;
    
      for (int x = 0; x < 10; x++){       // Get 10 samples
        //waitForPWMHigh();
        AcsValue = analogRead(curr_pin);  // Read current amp sensor values   
        Samples = Samples + AcsValue;     // Add samples together
      }
      raw = Samples / 10.0;  // Taking Average of Samples

      // For some unknown reason, there is an offset current of 5 amps...
      float amps = raw * VCC / (1023 * AMP_GAIN * R_SNS); //- 5; // Formula derived from voltage drop across sense resistor amplified and read from 0-1023

      if(amps < 0.0 && amps > -6.0f)
        return 0.0f;  // Return 0 if close to 0 (prevent negative resistance)
      return amps;
    }

    // Returns the voltage (VLD) after the load (muscle) -> used with getMuscleAmps() and getBatteryVolts() to calculate resistance
    float getLoadVoltage() {
      float value=0.0,samples=0.0,avg_value=0.0,raw=0.0;
    
      for (int x = 0; x < 10; x++){   // Get 10 samples
        //waitForPWMHigh();
        //delayMicroseconds((1.0/490.0*1000000) * pwm_val/255.0 - 10); // Delay until almost the end of the pulse (10 microseconds before)
        value = analogRead(vld_pin);  // Read voltage divider values   
        samples += value;          // Add samples together
      }
      raw = samples / 10.0;  // Taking Average of Samples

      float volts = raw * vld_scaleFactor + vld_offset; // Values are attained experimentally with known input voltages
      if(volts < 0.0 && volts > -2.0)
        return 0.0f; // Return 0 volts for values close to 0
      return volts;
      //return raw;
    }

    // V=IR -> R=V/I
    static float calcResistance(float V1, float V2, float I) {
      if(I == 0.0f)
        return 9999999999.0f; // Really high number
      return 1000 * abs((V1-V2) / I);
    }

    // Callable by PWMSamplerDriver with a reference to a TF_Muscle
    static void static_measure(TF_Muscle* m) {
      //Serial.println("measure");
      m->measure();
    }


    void measure() {
      // ONLY MEASURE ON HIGH VALUE OF PWM
      // If pwm is low, wait a max of 3 ms (This assumes that PWM frequency is 450 Hz)
      // If pwm does not switch during this time, take the measurement anyways
      //Serial.print("Measuring ");
      //Serial.println(name);
      vld_val = getLoadVoltage();    
      curr_val = getMuscleAmps();
      rld_val = calcResistance(n_vSupply, vld_val, curr_val);

      // TODO Not a good way of doing this btw
      //if(mode == TRAIN && enabled) {
        //pwm_duty_percent = updateTraining(rld_val);
      //}
    }

    /*void waitForPWMHigh() {
      if(enabled && pwm_duty_percent > 0 && !digitalRead(mosfet_pin)) {
        unsigned long measure_start = millis();
        unsigned long measure_timeout = 1000; //timeout after 1000 ms
        while((millis() - measure_start < measure_timeout) && !digitalRead(mosfet_pin)) 
        {  }
      }
      return;
    }*/

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