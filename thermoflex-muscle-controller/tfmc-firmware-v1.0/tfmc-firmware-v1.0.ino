/*=============================================================================
*
* ThermoFlex Node Muscle Controller Firmware
* Author: Mark Dannemiller
*
*=============================================================================*/



//=============================================================================
// TF Node Defaults
//=============================================================================

#define CTRL_MODE_CNT 4
enum ctrl_modes { PERCENT, VOLTS, AMPS, DEGREES };
const String ctrl_modes_str[CTRL_MODE_CNT] = { "percent", "volts", "amps", "degrees" };
#define SIGNAL_TIMEOUT 2.0 //amount of time between receiving master commands before auto-disable

// TF Node Commands ===========================================================
#define COMMAND_CNT 6 //number of commands
#define PARAM_MAX 20 //parameter array will be this size

//Command is the primitive object sent from a master device to this Node
//Note, commands are sent top-down from (Master Device -> Master Node -> Slave Nodes)
//Inherit from this class to create a new command (see other command classes below)
class Command {
  public:
    String name = "default";  //used to identify command being run
    byte code = 0x00;       //every command will be assigned an 8-bit code.  No code should repeat.
    String params[PARAM_MAX]; //parameters are passed into the referenced execute() function
    int param_cnt;            //incremented when a new command is added
    
    Command() {} //empty constructor

    Command(String _name, byte _code) {
      name = _name;
      code = _code;
    }
    
    //adds parameter to command's parameter "list"
    void addParameter(String _param) {
        params[param_cnt] = _param; 
        param_cnt++; 
        Serial.println("Found param: " + _param);
    }
    
    //must be overriden by inherited members of this command
    void execute() {  }

    static Command c_commands[COMMAND_CNT]; //array of all commands to iterate over
    //make sure to update COMMAND_CNT parameter when adding a new command
    //NOTE: would be nice to create a "Command Queue" for backlogging commands -> Need to create state machine for Node system

    //attempts to locate command by name from list of commands
    static bool getCommandByName(String _name, Command* c) {
      _name.toLowerCase();
      for(int i=0; i<COMMAND_CNT; i++) {
        if(c_commands[i].name == _name) {
          c = &c_commands[i];
          return true;
        }
      }
      //command not found
      return false;
    }

    static bool parseCommandStr(String consoleIn, Command* cmd) {
        //converts an input string to the corresponding command and parameters
        //input string should be "name par1 par2 par3 .. parn"
        int param_index = consoleIn.indexOf(' '); //start of parameters
        param_index = param_index==-1 ? consoleIn.length() : param_index; //if no spaces were found, parse entire input as name

        String _name = consoleIn.substring(0, param_index);
        bool found = getCommandByName(_name, cmd); //whether command was found
        cmd->param_cnt = 0; //effectively clears params

        //continue if command was found and has parameters to parse
        if(found && param_index != consoleIn.length()) {
          String param_str = consoleIn.substring(param_index+1, consoleIn.length());
          int next_index = param_str.indexOf(' '); //index of next param start
          
          //iterate through parameters, splitting at ' ' characters, and add to list of params
          for(int i = 0; i < PARAM_MAX && param_index != -1; i++) {
            String param = param_str.substring(param_index+1, next_index);
            param_str = param_str.substring(next_index, param_str.length());
            cmd->addParameter(param); //add parameter to array of parameters
            param_index = next_index;
            next_index = param_str.indexOf(' '); //index of next param start
          }

          if(cmd->param_cnt < PARAM_MAX)
            cmd->addParameter(param_str); //add last parameter since another space will not be located
        }
        return found;
    }
};

class Reset : public Command {
  public:
    Reset() : Command("reset", 0xFF) {
      //additional init
    }
    void execute() {

    }
} c_reset;

class SetEnable : public Command {
  public:
    SetEnable() : Command("enable", 0x01) {
      //additional init
    }
    void execute() {

    }
} c_setEnable;

class SetMode : public Command {
  public:
    SetMode() : Command("set-mode", 0x02) {
      //additional init
    }
    void execute() {

    }
} c_setMode;

class SetSetpoint : public Command {
  public:
    SetSetpoint() : Command("set-setpoint", 0x03) {
      //additional init
    }
    void execute() {

    }
} c_setSetpoint;

class Status : public Command {
  public:
    Status() : Command("status", 0x04) {
      //additional init
    }
    void execute() {

    }
} c_status;

class Stop : public Command {
  public:
    Stop() : Command("stop", 0x05) {
      //additional init
    }
    void execute() {

    }
} c_stop;

Command Command::c_commands[COMMAND_CNT] = { c_reset, c_setEnable, c_setMode, c_setSetpoint, c_status, c_stop };

//=============================================================================
// TF Node Configuration
//=============================================================================

#define MUSCLE_CNT 3
#define SHIELD_VERSION "1.0"
#define FIRMWARE_VERSION "1.0-dev"
#define VRD_SCALE_FACTOR 1 //TO-DO
//pinout
#define VRD_PIN A4

//=============================================================================
// Diagnostic Variables
//=============================================================================

const int ERR_LOW_VOLT = 0; //low battery error
const int ERR_CURRENT_OF = 1; //current overflow error
byte error = 0b00000000; //error byte transmitted when requested by API call

unsigned long timeout_timer;


//=============================================================================
// Control Variables
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
        
    }

    void enable() {

    }

    void disable() {

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
//init muscles
} m_1, m_2, m_3; //update parameter MUSCLE_CNT when new muscle is added

TF_Muscle TF_Muscle::muscles[MUSCLE_CNT] = { m_1, m_2, m_3 };

void initMuscles() {

  m_1.mosfet_pin = 3;
  m_1.curr_pin = A0;

  m_2.mosfet_pin = 5;
  m_2.curr_pin = A1;

  m_3.mosfet_pin = 6;
  m_3.curr_pin = A2;

  //init mosfet trigger pins
  for(int m = 0; m < MUSCLE_CNT; m++) {
      TF_Muscle::muscles[m].init();
  }
}



//=============================================================================
// TF Node Arduino Operation Functions
//=============================================================================

void setup() {
  Serial.begin(115200);

  initMuscles();
}

String command_str; //for processing serial command

void loop() {
    
  updateMuscles();

  if(Serial.available()) {
    //implementation of string commands.  Will need to switch to code-based commands
    command_str = Serial.readStringUntil('/n');
    command_str.trim();
    command_str.toLowerCase();

    Command selected;
    bool found = Command::parseCommandStr(command_str, &selected);

    if(found) {
      selected.execute(); //call command's referenced method
    }
  }
}

void reset(String params[PARAM_MAX]) {

}


//=============================================================================
// Muscle Control/Config Functions
//=============================================================================

//disable all muscles
void disable() {
  for(int i=0; i<MUSCLE_CNT; i++) { muscles[i].disable(); };
}

//set mode for muscle m
void setMode(int m, ctrl_modes mode) {
  muslces[m].setMode(mode);
}

//set setpoint, specify mode
void setSetpoint(int m, ctrl_modes mode, float setpoint) {

}

//********************************************************************************************************************
void updateMuscles() {
  //loop through updating all muscles
  for(int m=0; m<MUSCLE_CNT; m++) {
    //write to muscle if enabled
    if(m_enabled[m]) {
      if(m_mode[m] == VOLTS) {
        analogWrite(MOSFET[m], percentToPWM(m_setpoint_volts[m])); //write pwm to mosfet m
      }
      else if(m_mode[m] == PULSE_CNT) {
        //unimplemented
      }
      else if(m_mode[m] == PULSE_INF) {
        //unimplemented
      }

      //note: CURRENT and TEMP modes are unimplemented
    }
    else {
      analogWrite(MOSFET[m], 0); //disabled means write 0% power
    }
  }
}

//=============================================================================
// Sensor Read Functions
//=============================================================================

//********************************************************************************************************************
//https://www.engineersgarage.com/acs712-current-sensor-with-arduino/
float getMuscleAmps() {
  float AcsValue=0.0,Samples=0.0,AvgAcs=0.0,raw=0.0;

  for (int x = 0; x < 30; x++){ //Get 150 samples
  AcsValue = analogRead(CURR_PIN);     //Read current sensor values   
  Samples = Samples + AcsValue;  //Add samples together
  }
  raw=Samples/30.0;//Taking Average of Samples

  float amps = (2.5 - (raw * (5.0 / 1024.0)))/0.100; //5.0/1024 is conversion ratio of volts/native unit
  return amps;
}

float getBatteryVolts() {
  return (analogRead(VRD_PIN) * VRD_SCALE_FACTOR);
}


//=============================================================================
// Unit Conversion Functions
//=============================================================================

//CONVERT PERCENT BETWEEN 0:1 TO POSITIVE PWM
int percentToPWM(float speed) {
  int pwm = abs(speed) * 255;
  return pwm;
}
