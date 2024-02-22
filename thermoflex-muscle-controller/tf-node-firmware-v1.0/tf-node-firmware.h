//place all functions from firmware here
#include<Arduino.h>

//=============================================================================
// TF Node Defaults
//=============================================================================

#define CTRL_MODE_CNT 4
enum ctrl_modes { PERCENT, VOLTS, AMPS, DEGREES };
const String ctrl_modes_str[CTRL_MODE_CNT] = { "percent", "volts", "amps", "degrees" };
#define SIGNAL_TIMEOUT 2.0  // Amount of time between receiving master commands before auto-disable

// TF Node Commands ===========================================================
#define COMMAND_CNT 6  // number of commands
#define PARAM_MAX 20  // parameter array will be this size


//=============================================================================
// TF Node Configuration
//=============================================================================

#define MUSCLE_CNT 3
#define SHIELD_VERSION "1.0"
#define FIRMWARE_VERSION "1.0-dev"
#define VRD_SCALE_FACTOR 1 //TO-DO
//pinout
#define VRD_PIN A4

void initMuscles();


//=============================================================================
// Diagnostic Variables
//=============================================================================

const int ERR_LOW_VOLT = 0; //low battery error
const int ERR_CURRENT_OF = 1; //current overflow error
byte error = 0b00000000; //error byte transmitted when requested by API call

unsigned long timeout_timer;


//=============================================================================
// Muscle Control/Config Functions
//=============================================================================

//enable/disable all muscles
void setEnableAll(bool state);
void resetAll();
void updateMuscles();


//=============================================================================
// Sensor Read Functions
//=============================================================================

float getBatteryVolts();


//=============================================================================
// Unit Conversion Functions
//=============================================================================

int percentToPWM(float speed);


//=============================================================================
// Unit Classes
//=============================================================================

//Every muscle is controlled through open-loop (percent) or closed-loop (volt, amp, degree, sensor**) mode
struct TF_Muscle {
    //I/O
    int mosfet_pin;
    int curr_pin;

    //CONTROL MODE
    bool enabled = false;
    ctrl_modes mode = PERCENT;
    float setpoint_percent;
    float setpoint_volts;
    float setpoint_amps;
    float setpoint_degrees;

    //PULSE SETUP
    bool pulse_state = false; //on/off state of pulse cycle
    float pulse_on_time = 1.0f;
    float pulse_off_time = 1.0f;
    int pulse_cnt = -2; //-2 for continuous mode, -1 for infinite pulse, >0 for finite pulse
    unsigned long pulse_timer;

    static TF_Muscle muscles[MUSCLE_CNT];

    void init() {
        pinMode(mosfet_pin, OUTPUT);
    }

    void update() {
      //write to muscle if enabled
      if(enabled) {
        if(mode == PERCENT) {
          analogWrite(mosfet_pin, percentToPWM(setpoint_percent)); //write pwm to mosfet m
        }
  
        //note: CURRENT and TEMP modes are unimplemented
      }
      else {
        analogWrite(mosfet_pin, 0); //disabled means write 0% power
      }
    }

    void setEnable(bool state) {
       enabled = state;
    }

    void enable() {
      enabled = true;
    }

    void disable() {
       enabled = false;
    }

    void setMode(ctrl_modes _mode) {
       mode = _mode;
    }

    void setSetpoint(ctrl_modes _mode, float setpoint) {
        switch (_mode)
        {
        case VOLTS:
            setpoint_volts = setpoint;
            break;
        case AMPS:
            setpoint_amps = setpoint;
            break;
        case DEGREES:
            setpoint_degrees = setpoint;
            break;
        default:
            break;
        }
    }

    //********************************************************************************************************************
    //https://www.engineersgarage.com/acs712-current-sensor-with-arduino/
    float getMuscleAmps() {
      float AcsValue=0.0,Samples=0.0,AvgAcs=0.0,raw=0.0;
    
      for (int x = 0; x < 30; x++){ //Get 150 samples
        AcsValue = analogRead(curr_pin);     //Read current sensor values   
        Samples = Samples + AcsValue;  //Add samples together
      }
      raw=Samples/30.0;//Taking Average of Samples
    
      float amps = (2.5 - (raw * (5.0 / 1024.0)))/0.100; //5.0/1024 is conversion ratio of volts/native unit
      return amps;
    }

//init muscles
} m_1, m_2, m_3; //update parameter MUSCLE_CNT when new muscle is added


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
    
    // adds parameter to command's parameter "list"
    void addParameter(String _param) {
        params[param_cnt] = _param; 
        param_cnt++; 
        Serial.println("Found param: " + _param);
    }

    // displays the given name and parameters passed into command
    void serialDisplayInput() {
      Serial.print("Name: ");
      Serial.print(name);
      Serial.print(" Params: ");
      for(int i = 0; i < param_cnt; i++) {
        Serial.print(params[i] + ' ');
      }
    }
    
    // must be overriden by inherited members of this command
    void execute() { 
      serialDisplayInput();  // for now, only execution will be displaying back what was passed in
    }

    static Command c_commands[COMMAND_CNT];  // array of all commands to iterate over
    // make sure to update COMMAND_CNT parameter when adding a new command
    // NOTE: would be nice to create a "Command Queue" for backlogging commands -> Need to create state machine for Node system

    // attempts to locate command by name from list of commands
    static bool getCommandByName(String _name, Command* c) {
      _name.toLowerCase();
      for(int i=0; i<COMMAND_CNT; i++) {
        if(c_commands[i].name == _name) {
          c = &c_commands[i];
          return true;
        }
      }
      // command not found
      return false;
    }

    static bool parseCommandStr(String consoleIn, Command* cmd) {
        // converts an input string to the corresponding command and parameters
        // input string should be "name par1 par2 par3 .. parn"
        int param_index = consoleIn.indexOf(' ');  // start of parameters
        param_index = param_index==-1 ? consoleIn.length() : param_index;  // if no spaces were found, parse entire input as name

        String _name = consoleIn.substring(0, param_index);
        Serial.println(_name);
        bool found = getCommandByName(_name, cmd);  // whether command was found
        cmd->param_cnt = 0;  // effectively clears params

        // continue if command was found and has parameters to parse
        if(found && param_index != consoleIn.length()) {
          String param_str = consoleIn.substring(param_index+1, consoleIn.length());
          Serial.println(param_str);
          int start_index = 0;  // index from start to param_index
          param_index = param_str.indexOf(' ');  // index of next param start
          
          // iterate through parameters, splitting at ' ' characters, and add to list of params
          for(int i = 0; i < PARAM_MAX && param_index != -1; i++) {
            String param = param_str.substring(start_index, param_index);
            Serial.println(param);
            param_str = param_str.substring(param_index+1, param_str.length());
            Serial.println(param_str);
            cmd->addParameter(param);  // add parameter to array of parameters
            start_index = param_index+1;
            param_index = param_str.indexOf(' ');  // index of next param start
          }

          if(cmd->param_cnt < PARAM_MAX)
            cmd->addParameter(param_str);  // add last parameter since another space will not be located
        }
        return found;
    }
};

class Reset : public Command {
  public:
    explicit Reset() : Command("reset", 0xFF) {
      // additional init
    }
    void execute() {
       Command::execute();
       resetAll();
    }
} c_reset;

class SetEnable : public Command {
  public:
    explicit SetEnable() : Command("enable", 0x01) {
      //additional init
    }
    void execute() {
      Command::execute();
      //additional execute
      String state_str = params[1];
      state_str.toLowerCase();
      bool state = state_str == "true" ? true : false;
      if(params[0] == "all") {
         setEnableAll(state);
      }
      else {
         int m = params[0].toInt();
         TF_Muscle::muscles[m].setEnable(state);
      }
    }
} c_setEnable;

class SetMode : public Command {
  public:
    explicit SetMode() : Command("set-mode", 0x02) {
      //additional init
    }
    void execute() {
      Command::execute();
      //additional execute
      int m = params[0].toInt();
      String mode_str = params[1];
      mode_str.toLowerCase();

      ctrl_modes mode = mode_str == ctrl_modes_str[PERCENT] ? PERCENT :
                        mode_str == ctrl_modes_str[VOLTS] ? VOLTS :
                        mode_str == ctrl_modes_str[AMPS] ? AMPS :
                        mode_str == ctrl_modes_str[DEGREES] ? DEGREES : PERCENT; //default is percent
      TF_Muscle::muscles[m].setMode(mode);
    }
} c_setMode;

class SetSetpoint : public Command {
  public:
    explicit SetSetpoint() : Command("set-setpoint", 0x03) {
      //additional init
    }
    void execute() {
      Command::execute();
      //additional execute
    }
} c_setSetpoint;

class Status : public Command {
  public:
    explicit Status() : Command("status", 0x04) {
      //additional init
    }
    void execute() {
      Command::execute();
      //additional execute
      Serial.print("Battery Volts: ");
      Serial.println(getBatteryVolts());
      Serial.print("Error Code: ");
      Serial.println(bit(error));
    }
} c_status;

class Stop : public Command {
  public:
    explicit Stop() : Command("stop", 0x05) {
      //additional init
    }
    void execute() {
      Command::execute();
      //additional execute
    }
} c_stop;