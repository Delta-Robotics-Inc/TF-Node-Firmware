/*=============================================================================
*
* ThermoFlex Node Muscle Controller Firmware
* Author: Mark Dannemiller
*
*=============================================================================*/

#include <Arduino.h>
#include "TFNode.hpp"

TFNode tfNode;

void setup() {
    tfNode.begin();
}

void loop() {
    tfNode.update();
}
