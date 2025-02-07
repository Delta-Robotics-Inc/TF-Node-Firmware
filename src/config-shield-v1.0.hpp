#pragma once
/*
===============================================================================
    Configuration of the TF Node board that this firmware deploys to.  
    Make sure to keep "Shield Version" relevant.
===============================================================================
*/

#include <cstdint>

//=============================================================================
// TF Node Board Configuration
//=============================================================================

const uint8_t MUSCLE_CNT = 2;
const uint8_t SHIELD_VERSION_MAJOR = 1;
const uint8_t SHIELD_VERSION_MINOR = 0;

const uint8_t SMA_CONTROLLER_CNT = 2;
const float VCC = 5.0;              // [V] Maximum readable value.  Should be 5.0 V but will vary slightly with the voltage regulator output


// Voltage Read Conversion Equations
const float VRD_SCALE_FACTOR = 11 * VCC/1023.0;  // Derived from resistor divider circuit with R1=10k and R2=1k
const float VRD_OFFSET = 0.0f;
const float VLD_SCALE_FACTOR_M1 = 11 * VCC/1023.0;
const float VLD_SCALE_FACTOR_M2 = 11 * VCC/1023.0;
const float VLD_OFFSET_M1 = 0;
const float VLD_OFFSET_M2 = 0;  // Todo separate into muscleconfig struct

// Current Sense Amplifier -> TI INA301
const float R_SNS = 0.0005f;    // [ohms]
const float AMP_GAIN = 100.0f;  // [V/V] Vin/Vout gain of amplifier

// Pinout - make sure to match with physical board
#define VRD_PIN A4
#define STATUS_SOLID_LED 8
#define STATUS_RGB_RED 12
#define STATUS_RGB_GREEN 10
#define STATUS_RGB_BLUE 11

//SMAController0
#define M1_MOS_TRIG 9
#define M1_CURR_RD A0
#define M1_VLD_RD A2
#define M1_ALERT 2

//SMAController1
#define M2_MOS_TRIG 6
#define M2_CURR_RD A1
#define M2_VLD_RD A3
#define M2_ALERT 3

#define AUX_BUTTON 7
#define MANUAL_MODE_POT A5
const float MANUAL_MODE_THRESHOLD = 0.02f;

// Device Limits
const float MAX_CURRENT = 80.0f;  // [A] Maximum current through muscle in amps
const float MIN_VSUPPLY = 7.0f;   // [V] Minimum battery voltage
const float IGNORE_SUPPLY = 3.0f; // [V] Treat levels below this as if the battery is disconnected