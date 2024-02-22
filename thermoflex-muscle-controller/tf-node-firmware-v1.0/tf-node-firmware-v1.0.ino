/*=============================================================================
*
* ThermoFlex Node Muscle Controller Firmware
* Author: Mark Dannemiller
*
*=============================================================================*/
#include "tf-node-firmware.h"


//=============================================================================
// Control Variables
//=============================================================================

TF_Muscle TF_Muscle::muscles[MUSCLE_CNT] = { m_1, m_2, m_3 };
Command Command::c_commands[COMMAND_CNT] = { c_reset, c_setEnable, c_setMode, c_setSetpoint, c_status, c_stop };

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

    Command selected; //initialize default command (will be overriden if user inputted the name or code of a real command)
    bool found = Command::parseCommandStr(command_str, &selected);
    selected.execute(); //for now, execute whatever command was found for debugging purposes

    //if the command was sucessfully found,
    if(found) {
      //selected.execute(); //call command's referenced method
      Serial.println("Command was valid");
    }
  }
}


//=============================================================================
// Muscle Control/Config Functions
//=============================================================================

//enable/disable all muscles
void setEnableAll(bool state) {
  for(int m=0; m<MUSCLE_CNT; m++) { TF_Muscle::muscles[m].setEnable(state); };
}

void resetAll() {
  for(int m=0; m<MUSCLE_CNT; m++) { TF_Muscle::muscles[m].setEnable(false); };
}


//********************************************************************************************************************
void updateMuscles() {
  //loop through updating all muscles
  for(int m=0; m<MUSCLE_CNT; m++) {
    TF_Muscle::muscles[m].update();
  }
}


//=============================================================================
// Sensor Read Functions
//=============================================================================

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
