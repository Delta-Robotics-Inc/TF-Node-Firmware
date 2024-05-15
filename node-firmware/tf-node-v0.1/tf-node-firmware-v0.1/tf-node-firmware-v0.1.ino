/*=============================================================================
*
* ThermoFlex Node Muscle Controller Firmware
* Author: Mark Dannemiller
* Version: v0.1
*
*=============================================================================*/
#include "tf-node-firmware-v0.1.h"


//=============================================================================
// Control Variables
//=============================================================================

TF_Muscle* TF_Muscle::muscles[MUSCLE_CNT] = { &m_1, &m_2 };
Command* Command::c_commands[COMMAND_CNT] = { &c_default, &c_reset, &c_setEnable, &c_setMode, &c_setSetpoint, &c_status, &c_stop, &c_logMode};


//=============================================================================
// TF Node Arduino Operation Functions
//=============================================================================

void initMuscles() {
  // Initialize muscles with params and I/O pins
  TF_Muscle::muscles[0]->init("M1", 3, A0, A2, VLD_SCALE_FACTOR_M1, VLD_OFFSET_M1);
  TF_Muscle::muscles[1]->init("M2", 6, A1, A3, VLD_SCALE_FACTOR_M2, VLD_OFFSET_M2);

  m_1.setEnable(false);
  m_2.setEnable(false);
}

void setup() {
  Serial.begin(115200);

  /*while(!Serial.available()) { }
  Serial.println("TF-Node Device initializing...");
  Serial.print("Node shield version: ");
  Serial.println(SHIELD_VERSION);
  Serial.print("Node firmware version: ");
  Serial.println(FIRMWARE_VERSION);*/

  initMuscles();
  log_timer = millis();
}

String command_str; //for processing serial command

void loop() {
  
  // UPDATE OUTPUTS AND TAKE SENSOR MEASUREMENTS
  vBattery_val = getBatteryVolts();
  TF_Muscle::updateMuscles();

  if(Serial.available()) {
    // implementation of string commands.  Will need to switch to HEXcode-based commands
    command_str = Serial.readStringUntil('\n');
    command_str.trim();
    command_str.toLowerCase();

    Command selected; //initialize default command (will be overriden if user inputted the name or code of a real command)
    bool found = false;
    Command* cmd = Command::parseCommandStr(command_str, found);
    cmd->execute(); //for now, execute whatever command was found for debugging purposes

    //if the command was sucessfully found,
    if(found) {
      Serial.println("Command was valid.");
    }
    else {
      Serial.println("Error: unrecognized command.");
    }
  }

  // Every certain interval (LOG_MS), log/report data to console
  if(millis() - log_timer > LOG_MS) {
    String log_str = log();
    // Only print if there is something to log
    if(log_str != " ") // Make sure that log data exceeds 90, otherwise it will never print
      Serial.print(log());
    log_timer = millis();
  }
}

String deviceStatus() {
  String stat_str ="Battery Volts: " + String(vBattery_val) + " V\n";
  stat_str += "Error State: " + String(error, BIN);
  return stat_str;
}

// Logs data for the devices that have an active "logMode", including the node itself
String log() {
  // Determine if logging is necessary
  bool toLog = nodeLogMode == 1;
  for(int x=0; x<MUSCLE_CNT; x++) {
    if(toLog)
      break;
    toLog = TF_Muscle::muscles[x]->logMode == 1;
  }

  // Return empty if no device needs to log
  if(!toLog)
    return " ";

  String log_str = "========================================\nLOG TIME: " + String(millis() - log_start) + "\n"; // Display time since log start
  if(nodeLogMode == 1)
    log_str += deviceStatus() + "\n";

  for(int m=0; m<MUSCLE_CNT; m++) { log_str += TF_Muscle::muscles[m]->log(); };

  return log_str;
}
