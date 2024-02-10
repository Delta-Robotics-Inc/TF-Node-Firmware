
//=============================================================================
// ThermoFlex constants
//=============================================================================

enum ctrl_modes { VOLTS, CURRENT, TEMP, PULSE_CNT, PULSE_INF };
#define SIGNAL_TIMEOUT 2.0 //amount of time between receiving master commands before auto-disable


//=============================================================================
// ThermoFlex configuration
//=============================================================================

#define MUSCLE_CNT 3
#define SHIELD_VERSION "1.0"
#define FIRMWARE_VERSION "1.0-dev"
#define VRD_SCALE_FACTOR 0 //**TODO
//pinout
#define VRD_PIN A4

const int MOSFET[MUSCLE_CNT] = { 3, 5, 6 }; //define the pinout of each muscle mosfet trigger
const int CURR_PIN[MUSCLE_CNT] = { A0, A1, A2 };

//=============================================================================
// Diagnostic variables
//=============================================================================

const int ERR_LOW_VOLT = 0; //low battery error
const int ERR_CURRENT_OF = 1; //current overflow error
byte error = 0b00000000; //error byte transmitted when requested by API call

unsigned long timeout_timer;


//=============================================================================
// Control variables
//=============================================================================

bool m_enabled[MUSCLE_CNT] = { false, false, false }; //eventually wrap into class?
ctrl_modes m_mode[MUSCLE_CNT] = { VOLTS, VOLTS, VOLTS };
float m_setpoint_volts[MUSCLE_CNT] = { 0, 0, 0 };
float m_setpoint_amps[MUSCLE_CNT] = { 0, 0, 0 };
float m_setpoint_degrees[MUSCLE_CNT] = { 0, 0, 0 };

bool m_pulse_val[MUSCLE_CNT] = { false, false, false }; //switches from on/off during pulse interval
float m_pulse_on = 0.0f; //duration of pulse on
float m_pulse_off = 0.0f; //duration of pulse off
int m_pulse_cnt; //number of pulses left when in PULSE_CNT mode
unsigned long m_pulse_timer;




//=============================================================================
// Arduino Operation Functions
//=============================================================================

void setup() {
    Serial.begin(115200);
    
    //init mosfet trigger pins
    for(int m = 0; m < MUSCLE_CNT; m++) {
        pinMode(m, OUTPUT);
    }
}

void loop() {
    
    updateMuscles();
}


//=============================================================================
// Muscle Control/Config Functions
//=============================================================================

//enable muscle at index m
void enable(int m) {
  m_enabled[m] = true;
}

//disable muscle at index m
void disable(int m) {
  m_enabled[m] = false;
}

//disable all muscles
void disable() {
  for(int i=0; i<MUSCLE_CNT; i++) { m_enabled[i] = false; };
}

//set mode for muscle m
void set_mode(int m, ctrl_modes mode) {
  m_mode[m] = mode;
}

//set setpoint, specify mode
void set_setpoint(int m, ctrl_modes mode, float setpoint) {
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