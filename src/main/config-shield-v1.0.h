#pragma once
/*
===============================================================================
    Configuration of the TF Node board that this firmware deploys to.  
    Make sure to keep "Shield Version" relevant.
===============================================================================
*/

//=============================================================================
// TF Node Board Configuration
//=============================================================================

#define MUSCLE_CNT 2
#define SHIELD_VERSION "1.0"

const uint8_t MUSCLE_CNT = 2;
const float VCC = 5.0;              // [V] Maximum readable value.  Should be 5.0 V but will vary slightly with the voltage regulator output


// Voltage Read Conversion Equations
#define VRD_SCALE_FACTOR 11 * VCC/1023.0  // Derived from resistor divider circuit with R1=10k and R2=1k
#define VRD_OFFSET 0
#define VLD_SCALE_FACTOR_M1 11 * VCC/1023.0
#define VLD_SCALE_FACTOR_M2 11 * VCC/1023.0
#define VLD_OFFSET_M1 0
#define VLD_OFFSET_M2 0  // Todo separate into muscleconfig struct

// Current Sense Amplifier -> TI INA301
#define R_SNS 0.0005  // [ohms]
#define AMP_GAIN 100  // [V/V] Vin/Vout gain of amplifier

// Pinout - make sure to match with physical board
#define VRD_PIN A4
#define STATUS_SOLID_LED 8
#define STATUS_RGB_RED 9
#define STATUS_RGB_GREEN 10
#define STATUS_RGB_BLUE 11

#define M1_MOS_TRIG 3
#define M1_CURR_RD A0
#define M1_VLD_RD A2
#define M1_ALERT 12

#define M2_MOS_TRIG 6
#define M2_CURR_RD A1
#define M2_VLD_RD A3
#define M2_ALERT 13

#define AUX_BUTTON 7
#define MANUAL_MODE_POT A5
#define MANUAL_MODE_THRESHOLD 0.02

// Device Limits
#define MAX_CURRENT 80.0  // [A] Maximum current through muscle in amps
#define MIN_VSUPPLY 7.0  // [V] Minimum battery voltage
#define IGNORE_SUPPLY 3.0 // [V] Treat levels below this as if the battery is disconnected