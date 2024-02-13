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
const enum command_type  { SETMODE, SETSETPOINT, GETDATA, SETENABLE, STOP };
const String command_type_str[] = { "SETMODE", "SETSETPOINT", "GETDATA", "SETENABLE", "STOP"};

//Command is the primitive object sent from a master device to this Node
//Note, commands are sent top-down from (Master Device -> Master Node -> Slave Nodes)
struct Command {
    String name;                                  //used to identify command being run
    byte code;                                    //every command will be assigned an 8-bit code.  No code should repeat.
    byte params[PARAM_MAX];                       //parameters are passed into the referenced execute() function
    int param_cnt;                                //incremented when a new command is added
    void (*execute)(byte _params[PARAM_MAX]);     //reference function to execute when command is called
    void addParameter(byte _param) {
        params[param_cnt] = _param; param_cnt++; }//adds parameter to command "list"
} c_reset, c_setEnable, c_setMode, c_setSetpoint, c_status, c_stop;
//make sure to update COMMAND_CNT parameter when adding a new command

c_reset.name = "reset";
c_reset.code = 0xFF; //all 1 bits

c_setEnable.name = "enable";
c_setEnable.code = 0x01;

c_setMode.name = "set-mode";
c_setMode.code = 0x02;

c_setSetpoint.name = "set-setpoint";
c_setSetpoint.code = 0x03;

c_status.name = "status";
c_status.code = 0x04;

c_stop.name = "stop";
c_stop.code = 0x05;

Command c_commands[COMMAND_CNT] = { c_reset, c_setEnable, c_setMode, c_setSetpoint, c_status, c_stop }; //array of all commands to iterate over
//NOTE: would be nice to create a "Command Queue" for backlogging commands -> Need to create state machine for Node system

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
    floate setpoint_percent;
    float setpoint_volts;
    float setpoint_amps;
    float setpoint_degrees;

    //PULSE SETUP
    bool pulse_state = false; //on/off state of pulse cycle
    float pulse_on_time = 1.0f;
    float pulse_off_time = 1.0f;
    int pulse_cnt = -2; //-2 for continuous mode, -1 for infinite pulse, >0 for finite pulse
    unsigned long pulse_timer;

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

    void setSetpoint() {
        switch (mode)
        {
        case VOLTS:
            m_setpoint_volts[m] = setpoint;
            break;
        case CURRENT:
            m_setpoint_amps[m] = setpoint;
            break;
        case TEMP:
            m_setpoint_degrees[m] = setpoint;
            break;
        default:
            break;
        }
    }
//init muscles
} m_1, m_2, m_3; //update parameter MUSCLE_CNT when new muscle is added

m_1.mosfet_pin = 3;
m_1.curr_pin = A0;

m_2.mosfet_pin = 5;
m_2.curr_pin = A1;

m_3.mosfet_pin = 6;
m_3.curr_pin = A2;

TF_Muscle muscles[MUSCLE_CNT] = { m_1, m_2, m_3 };




//=============================================================================
// TF Node Arduino Operation Functions
//=============================================================================

void setup() {
    Serial.begin(115200);
    
    //init mosfet trigger pins
    for(int m = 0; m < MUSCLE_CNT; m++) {
        muscles[m].init();
    }
}

String command; //for processing serial command
String params;

void loop() {
    
  updateMuscles();

  if(Serial.available()) {
    command = Serial.readStringUntil(' ');
    command.trim();
    command = command.toUpperCase();

    params = Serial.readStringUntil("/n");
    params.trim();

    switch (command)
    {
    case command_type_str[SETMODE]:
      /* code */
      break;
    case command_type_str[SETSETPOINT]:
      /* code */
      break;
    case command_type_str[GETDATA]:
      /* code */
      break;
    case command_type_str[SETENABLE]:
      /* code */
      break;
    case command_type_str[STOP]:
      /* code */
      break;

    default:
      break;
    }
  }
}

void reset() {

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
