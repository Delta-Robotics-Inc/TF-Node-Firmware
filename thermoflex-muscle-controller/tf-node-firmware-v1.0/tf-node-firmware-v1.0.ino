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

TF_Muscle* TF_Muscle::muscles[MUSCLE_CNT] = { &m_1, &m_2, &m_3 };
Command* Command::c_commands[COMMAND_CNT] = { &c_default, &c_reset, &c_setEnable, &c_setMode, &c_setSetpoint, &c_status, &c_stop };

void initMuscles() {
  //init mosfet trigger pins
  TF_Muscle::muscles[0]->init("M1", 3, A0);
  TF_Muscle::muscles[1]->init("M2", 5, A1);
  TF_Muscle::muscles[2]->init("M3", 6, A2);

  m_1.setEnable(false);
}


//=============================================================================
// TF Node Arduino Operation Functions
//=============================================================================

void setup() {
  Serial.begin(115200);

  /*while(!Serial.available()) { }
  Serial.println("TF-Node Device initializing...");
  Serial.print("Node shield version: ");
  Serial.println(SHIELD_VERSION);
  Serial.print("Node firmware version: ");
  Serial.println(FIRMWARE_VERSION);*/

  initMuscles();
}

String command_str; //for processing serial command

void loop() {
    
  TF_Muscle::updateMuscles();

  if(Serial.available()) {
    //implementation of string commands.  Will need to switch to code-based commands
    command_str = Serial.readStringUntil('\n');
    command_str.trim();
    command_str.toLowerCase();

    Command selected; //initialize default command (will be overriden if user inputted the name or code of a real command)
    bool found = false;
    Command* cmd = Command::parseCommandStr(command_str, found);
    cmd->execute(); //for now, execute whatever command was found for debugging purposes

    //if the command was sucessfully found,
    if(found) {
      //selected.execute(); //call command's referenced method
      Serial.println("Command was valid.");
    }
    else {
      Serial.println("Error: unrecognized command.");
    }
  }
  else {
    if(TF_Muscle::muscles[0]->enabled)
      Serial.println(TF_Muscle::muscles[0]->getMuscleAmps());
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
