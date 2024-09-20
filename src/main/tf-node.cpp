#include "tf-node.h"
#include "config.h"

TFNode::TFNode()
    : smaController0(0, SMA_CONTROLLER_0_PWM_PIN, SMA_CONTROLLER_0_FEEDBACK_PIN),
      smaController1(1, SMA_CONTROLLER_1_PWM_PIN, SMA_CONTROLLER_1_FEEDBACK_PIN) {}

void TFNode::begin() {
    // Initialize settings
    EEPROM.begin(sizeof(Settings));
    settings.load();

    // Initialize controllers
    smaController0.begin();
    smaController1.begin();

    // Initialize command processor
    commandProcessor.begin();
}

void TFNode::update() {
    // Update controllers
    smaController0.update();
    smaController1.update();

    // Process commands
    commandProcessor.update();
}

