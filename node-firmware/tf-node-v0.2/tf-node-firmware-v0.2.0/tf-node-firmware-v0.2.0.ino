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
  TF_Muscle::muscles[0]->init("M1", M1_MOS_TRIG, M1_CURR_RD, M1_VLD_RD, VLD_SCALE_FACTOR_M1, VLD_OFFSET_M1);
  TF_Muscle::muscles[1]->init("M2", M2_MOS_TRIG, M2_CURR_RD, M2_VLD_RD, VLD_SCALE_FACTOR_M2, VLD_OFFSET_M2);

  m_1.setEnable(false);
  m_2.setEnable(false);
}

void setup() {
  Serial.begin(115200);
  pinMode(STATUS_SOLID_LED, OUTPUT);
  digitalWrite(STATUS_SOLID_LED, LOW);  // This pin currently goes high on errors

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
  nodeUpdate();
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
}

void nodeUpdate() {
  n_vSupply = getSupplyVolts();
  pot_val = getPotVal();

  // Only allow control when PC is in the loop
  // TODO: add option for 
  if(!Serial) {
    TF_Muscle::setEnableAll(false);
  }

  // Low Power Condition
  if(n_vSupply < MIN_VSUPPLY && n_vSupply > IGNORE_SUPPLY) {
    errRaise(ERR_LOW_VOLT);
    //TF_Muscle::setEnableAll(false);
    //light up a low battery LED
  }

  // Every certain interval (LOG_MS), log/report data to console
  if(millis() - log_timer > LOG_MS) {
    String log_str = log();
    // Only print if there is something to log
    if(log_str != " ") // Make sure that log data exceeds 90, otherwise it will never print
      Serial.print(log_str);
    log_timer = millis();
  }
}

String devStatusFormatted() {
  String stat_str = "========================================\nLOG TIME: " + String(millis() - log_start) + "\n"; // Display time since log start
  stat_str += "Battery Volts: " + String(n_vSupply) + " V\n";
  stat_str += "Error State: " + String(n_error, BIN) + "\n";
  stat_str += "Pot Val: " + String(pot_val) + "\n";
  return stat_str;
}

String devStatusQuick() {
  String stat_str = String(millis() - log_start) + " ";
  stat_str += String(n_vSupply) + " ";
  stat_str += String(n_error, BIN) + " ";
  stat_str += String(pot_val) + " ";
  return stat_str;
}

// Logs data for the devices that have an active "logMode", including the node itself
String log() {
  // Determine if logging is necessary
  bool toLog = nodeLogMode != 0;
  for(int x=0; x<MUSCLE_CNT; x++) {
    if(toLog)
      break;
    toLog = TF_Muscle::muscles[x]->logMode != 0;
  }

  // Return empty if no device needs to log
  if(!toLog)
    return " ";

  String log_str;
  if(nodeLogMode == 1)
    log_str += devStatusFormatted();
  else if(nodeLogMode == 2)
    log_str += devStatusQuick();

  for(int m=0; m<MUSCLE_CNT; m++) { log_str += TF_Muscle::muscles[m]->log(); };

  return log_str + "\n";
}
