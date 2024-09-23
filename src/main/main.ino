/*=============================================================================
*
* ThermoFlex Node Muscle Controller Firmware
* Author: Mark Dannemiller
*
*=============================================================================*/

#include "TFNode.h"

TFNode tfNode;

void setup() {
    tfNode.begin();
}

void loop() {
    tfNode.update();
}
