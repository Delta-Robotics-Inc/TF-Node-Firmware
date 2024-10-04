#include "TFNode.hpp"
#include "config.hpp"
#include "SMAController.hpp"

TFNode::TFNode() {
    smaController0 = new SMAController("M1", M1_MOS_TRIG, M1_CURR_RD, M1_VLD_RD, VLD_SCALE_FACTOR_M1, VLD_OFFSET_M1);
    smaController1 = new SMAController("M2", M2_MOS_TRIG, M2_CURR_RD, M2_VLD_RD, VLD_SCALE_FACTOR_M2, VLD_OFFSET_M2);
}

void TFNode::begin() {
    // TODO Initialize settings
    //EEPROM.begin(sizeof(NodeSettings));
    //settings.load();

    Serial.begin(115200);
    pinMode(STATUS_SOLID_LED, OUTPUT);
    digitalWrite(STATUS_SOLID_LED, LOW);  // This pin currently goes high on errors

    pinMode(AUX_BUTTON, INPUT_PULLUP); // Set button pin as input with internal pull-up resistor

    // Initialize controllers
    smaController0->begin();
    smaController1->begin();

    // Initialize command processor
    commandProcessor.begin();
    log_timer = millis();
}

void TFNode::update() {
    // Poll sensors
    n_vSupply = getSupplyVolts();
    pot_val = getPotVal();

    // Update controllers
    smaController0->update();
    smaController1->update();

    checkErrs();

    // Check auxillary gpio
    // TODO change to interrupt
    if(!digitalRead(AUX_BUTTON)) {
        optButtonStopFunc();
    }

    // Process commands
    commandProcessor.update();

    // TODO implement logger (should it be encapsulate within class or external function?)
    // Every certain interval (LOG_MS), log/report data to console
    if(millis() - log_timer > LOG_MS) {
        String status_str = status();
        // Only print if there is something to log
        if(status_str != " ") // Make sure that log data exceeds 90, otherwise it will never print
        Serial.print(status_str);
        log_timer = millis();
    }
}


//=============================================================================
// Node Status Functions
//=============================================================================

void TFNode::CMD_setStatusMode(int _mode)
{
}

String TFNode::status()
{
    return String();
}

String TFNode::statusCompact()
{
    return String();
}

String TFNode::statusDump()
{
    return String();
}

// Returns a readable string of the status dump.
// This is a large amount of data but useful when other side does not understand the TF Node messaging system
String TFNode::statusReadable()
{
    String stat_str = "========================================\nLOG TIME: " + String(millis() - log_start) + "\n"; // Display time since log start
    stat_str += "Battery Volts: " + String(n_vSupply, 6) + " V\n";
    stat_str += "Error State: " + String(n_error, BIN) + "\n";
    stat_str += "Pot Val: " + String(pot_val, 6) + "\n";
    return stat_str;
}



//=============================================================================
// Error Handling
//=============================================================================

void TFNode::optButtonStopFunc() {
  smaController0->CMD_setEnable(false);
  smaController1->CMD_setEnable(false);
  errRaise(ERR_EXTERNAL_INTERRUPT);
}

void TFNode::checkErrs() {
      // Low Power Condition
    if(n_vSupply < MIN_VSUPPLY && n_vSupply > IGNORE_SUPPLY) {
        //errRaise(ERR_LOW_VOLT);
        //TF_Muscle::setEnableAll(false);
        //light up a low battery LED
    }
}

// Raise Error by anding current error val with 0 bitshifted by index
void TFNode::errRaise(NodeError err_code) {
    n_error = n_error & (0b11111111 ^ (1<<err_code));
    digitalWrite(STATUS_SOLID_LED, HIGH);
}
void TFNode::errClear() {
    n_error = 0b11111111;
    digitalWrite(STATUS_SOLID_LED, LOW);
}
void TFNode::errClear(NodeError err_code) {
    n_error = n_error | (1<<err_code);
    digitalWrite(STATUS_SOLID_LED, LOW);
}


//=============================================================================
// Sensor Read Functions
//=============================================================================

float TFNode::getSupplyVolts() {
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
float TFNode::getPotVal() {
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

