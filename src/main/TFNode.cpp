#include "TFNode.h"
#include "config.h"

TFNode::TFNode()
    : smaController0(0, SMA_CONTROLLER_0_PWM_PIN, SMA_CONTROLLER_0_FEEDBACK_PIN),
      smaController1(1, SMA_CONTROLLER_1_PWM_PIN, SMA_CONTROLLER_1_FEEDBACK_PIN) {}

void TFNode::begin() {
    // Initialize settings
    EEPROM.begin(sizeof(Settings));
    settings.load();

    Serial.begin(115200);
    pinMode(STATUS_SOLID_LED, OUTPUT);
    digitalWrite(STATUS_SOLID_LED, LOW);  // This pin currently goes high on errors

    pinMode(AUX_BUTTON, INPUT_PULLUP); // Set button pin as input with internal pull-up resistor

    // Initialize controllers
    //smaController0.begin();
    //smaController1.begin();

    // Initialize command processor
    commandProcessor.begin();
    log_timer = millis();
}

void TFNode::update() {
    // Poll sensors
    n_vSupply = getSupplyVolts();
    pot_val = getPotVal();

    // Update controllers
    // smaController0.update();
    // smaController1.update();

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
        String log_str = log();
        // Only print if there is something to log
        if(log_str != " ") // Make sure that log data exceeds 90, otherwise it will never print
        Serial.print(log_str);
        log_timer = millis();
    }
}

void TFNode::checkErrs() {
      // Low Power Condition
    if(n_vSupply < MIN_VSUPPLY && n_vSupply > IGNORE_SUPPLY) {
        //errRaise(ERR_LOW_VOLT);
        //TF_Muscle::setEnableAll(false);
        //light up a low battery LED
    }
}
