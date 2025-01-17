#include "globals.hpp"
#include "SMAController.hpp"
#include "drivers/PWMSamplerDriver.h"

SMAController::SMAController(tfnode::Device _devicePort, String _name, uint8_t _m, uint8_t _currPin, uint8_t _vLdPin, float _scaleFactor, float _offset)
    : devicePort(_devicePort), name(_name), mosfet_pin(_m), curr_pin(_currPin), vld_pin(_vLdPin), vld_scaleFactor(_scaleFactor), vld_offset(_offset) {}

void SMAController::begin()
{
    resistTracker = new GradientTracker(gradWidth, gradSize, flatThreshold, timeStep_ms);
    resController = new ResistiveController(0.0, KP_rc, KI_rc, KD_rc);                                                                    // Initialize resistive PID controller
    driver = new PWMSamplerDriver(PWM_FREQUENCY, 0.0f, mosfet_pin, PWM_MEASURE_DELAY_US, PWM_MEASURE_CYCLE_THRESH, static_measure, this); // Initialize pwm driver which will handle pwm on the mosfet pin and sampling the sensors
}

// Test states for muscle algo
enum power_state { POWER_UNTIL_THRESH, UNPOWERED, UNPOWER_UNTIL_THRESH };
power_state pstate;

void SMAController::update()
{
    // #TODO: Handle pulse condition
    // Serial.println("Updating SMAController");

    // Write to muscle if enabled
    if (outputEnabled)
    {
        // Serial.println("Muscle enabled");
        // Handle behavior of each control mode
        switch (currentMode)
        {
        case tfnode::SMAControlMode::MODE_PERCENT:
            pwm_duty_percent = setpoint[(int)tfnode::SMAControlMode::MODE_PERCENT];
            break;
        case tfnode::SMAControlMode::MODE_VOLTS:
            pwm_duty_percent = setpoint[(int)tfnode::SMAControlMode::MODE_VOLTS] / master_tfNode->n_vSupply; // When controlling for volts, the ratio of setpoint/supply will be percentage of power to deliver
            break;
        // TODO: Implement other control modes
        case tfnode::SMAControlMode::MODE_AMPS:                                        // Need to implement PID loop
            pwm_duty_percent = setpoint[(int)tfnode::SMAControlMode::MODE_AMPS] / curr_val; // Simplest current control is just pulsing the peak current at a rate which matches setpoint current
            break;
        // case DEGREES:  // Need to implement equation to track/return muscle temp (temp will behave different based on 2 conditions: below/above Af)
        case tfnode::SMAControlMode::MODE_OHMS: // Need to implement PID loop (but how to implement with hysterisis curve?)
        {
            //resController->setSetpoint(setpoint[(int)tfnode::SMAControlMode::MODE_OHMS]); // Update setpoint of pid to setpoint of this device for OHMS mode
            //resController->update(rld_val);             // Update pid controller

            float diff = abs(rld_val - setpoint[(int)tfnode::SMAControlMode::MODE_OHMS]);
            float thresh = 10;
            float maximum = setpoint[(int)tfnode::SMAControlMode::MODE_OHMS] + 40;
            bool within_thresh = diff < thresh;

            if(pstate == POWER_UNTIL_THRESH && within_thresh) {
                pstate = UNPOWERED;
                Serial.println("UNPOWERED");
            } else if(pstate == UNPOWERED && !within_thresh) {
                pstate = POWER_UNTIL_THRESH;
                Serial.println("POWER_UNTIL_THRESH");
            }
            else if (pstate == UNPOWER_UNTIL_THRESH && within_thresh) {
                pstate = UNPOWERED;
                Serial.println("UNPOWERED");
            }
            else if (diff > maximum) {
                pstate = UNPOWER_UNTIL_THRESH;
                Serial.println("UNPOWER_UNTIL_THRESH");
            }

            if(pstate == POWER_UNTIL_THRESH) {
                pwm_duty_percent = max(1.0, 0.01*diff); // Simple proportional control
            } else {
                pwm_duty_percent = 0;
            }
        }
        break;
        // Eventually add position control (requires force to be known)
        case tfnode::SMAControlMode::MODE_TRAIN:
            pwm_duty_percent = updateTraining(rld_val); // Convert to mohms
            break;
        // Deprecating mode manual for now (not included in .proto)
        // case tfnode::SMAControlMode::MODE_MANUAL:
        //     pwm_duty_percent = master_tfNode.pot_val; // Manual mode uses the potentiometer if connected
        //     break;
        default:
            pwm_duty_percent = 0;
            break;
        }
    }
    else
    {
        pwm_duty_percent = 0; // Disabled means write 0% power
        // RESET TRAINING STATE
        // trainState = MARTENSITE;
    }

    // analogWrite(mosfet_pin, pwm_val);  // Write pwm to mosfet m
    // measure(); // Update sensor values
    driver->setDutyCyclePercent(pwm_duty_percent, outputEnabled); // The PWMDriver will hande analogWrite() and measuring sensors
    // Serial.println(driver->min_duty_percent);
    // Serial.println(driver->cycle_threshold);
    // Serial.println(driver->pulse_cycle_count);

    // CURRENT OVERFLOW ERROR CONDITION
    if (curr_val > MAX_CURRENT)
    {
        master_tfNode->errRaise(ERR_CURRENT_OF);
        // setEnable(false); //disable muscle
    }
}

void SMAController::CMD_setEnable(bool state)
{
    outputEnabled = state;
    pstate = POWER_UNTIL_THRESH;
    Serial.print("Setting enable: ");
    Serial.print((int)devicePort);
    Serial.print(" to ");
    Serial.println(state);
      // Clear the external disable error
      if(outputEnabled)
        master_tfNode->errClear(ERR_EXTERNAL_INTERRUPT);

      if((currentMode == tfnode::SMAControlMode::MODE_TRAIN || 
          currentMode == tfnode::SMAControlMode::MODE_OHMS) && outputEnabled) {
        Serial.println("SMAController Resetting Training");
        resetTraining();
        resController->Reset();
      }
}

void SMAController::CMD_setMode(tfnode::SMAControlMode _mode)
{
    currentMode = _mode;

    if(currentMode == tfnode::SMAControlMode::MODE_TRAIN) {
      trainState = MARTENSITE;
    }
}

void SMAController::CMD_setSetpoint(tfnode::SMAControlMode _mode, float val)
{
    setpoint[(int)_mode] = val;

    // Update delta milliohms for training mode
    if(_mode == tfnode::SMAControlMode::MODE_TRAIN)
        delta_mohms = ((At - Af) * L_nitinol / (PI * pow(D_nitinol / 2, 2)) * resistivity_per_c_nitinol) * 0.001; //results in milliohms
}

void SMAController::CMD_reset()
{
    CMD_setEnable(false);
    CMD_setMode(tfnode::SMAControlMode::MODE_PERCENT);
    // Reset timer and pulse vals*****************************************
}

// TODO Change "mode" to represent status message type and implement a "repeating" var
void SMAController::CMD_setStatusMode(tfnode::DeviceStatusMode _mode, bool repeating, NetworkInterface *iface)
{
    statusMode = _mode;
}


//=============================================================================
// SMAController Status Functions
//=============================================================================

tfnode::SMAStatusCompact SMAController::getSMAStatusCompact()
{
  tfnode::SMAStatusCompact status;
  
  status.set_device_port(devicePort);
  status.set_enabled(outputEnabled);
  status.set_mode(currentMode);
  status.set_setpoint(setpoint[(int)currentMode]);
  status.set_output_pwm(pwm_duty_percent);
  status.set_load_amps(curr_val);
  status.set_load_mohms(rld_val);
  status.set_load_vdrop(vld_val);
  
  return status;
}

tfnode::SMAStatusDump SMAController::getSMAStatusDump()
{
  tfnode::SMAStatusDump status;
  // Set compact status
  tfnode::SMAStatusCompact compactStatus = getSMAStatusCompact();
  status.mutable_compact_status() = compactStatus;

  // Get loaded settings (if any)
  status.mutable_loaded_settings() = settings;
    // TODO Queue outbound response message

  status.set_vld_scalar(vld_scaleFactor);
  status.set_vld_offset(vld_offset);
  status.set_r_sns_ohms(R_SNS);
  status.set_amp_gain(AMP_GAIN);
  status.set_af_mohms(Af_mohms);
  status.delta_mohms();//I am unsure of what going on here
  status.set_trainState(trainState);

  return status;
}

String SMAController::getSMAStatusReadable()
{
    String stat_str = 
                    "========================================\n";
    stat_str += "Port: " + String((int)devicePort) + "\n"; 
    stat_str += "Enable: " + String(outputEnabled) + " \n";
    stat_str += "Mode: " + String((int)currentMode) + "\n";
    stat_str += "Set Point: " + String(setpoint[(int)currentMode]) + "\n";
    stat_str += "Output_PWM: " + String(pwm_duty_percent) + "\n";
    stat_str += "Load_Amps: " + String(curr_val) + " A\n";
    stat_str += "Load_vdrop: " + String(rld_val) + " V\n";  
    stat_str += "Load_mohms: " + String(vld_val) + "Ohms\n"; 

    return stat_str;
}


//=============================================================================
// Sensor Value Getters
//=============================================================================

float SMAController::getBatteryVolts()
{
    return master_tfNode->n_vSupply;
}

float SMAController::getMuscleAmps()
{
    return curr_val;
}

float SMAController::getLoadVolts()
{
    return vld_val;
}

float SMAController::getResistance()
{
    return rld_val;
}


//=============================================================================
// Sensor Measurement Functions
//=============================================================================

// void SMAController::measure()
// {
//     // Read a single sample of load voltage
//     float vld_adc_value = analogRead(vld_pin);
//     float vld_sample = vld_adc_value * vld_scaleFactor + vld_offset;
//     if (vld_sample < 0.0f && vld_sample > -2.0f)
//         vld_sample = 0.0f;

//     // Read a single sample of current
//     float curr_adc_value = analogRead(curr_pin);
//     float curr_sample = curr_adc_value * VCC / (1023 * AMP_GAIN * R_SNS);
//     if (curr_sample < 0.0f && curr_sample > -6.0f)
//         curr_sample = 0.0f;

//     // Store samples in history arrays
//     voltage_history[sample_index] = vld_sample;
//     current_history[sample_index] = curr_sample;

//     // Update sample index (circular buffer)
//     sample_index = (sample_index + 1) % 10;

//     // Update vld_val and curr_val with current samples if needed
//     vld_val = vld_sample;
//     curr_val = curr_sample;

//     // Calculate the average resistance based on the last 10 samples
//     rld_val = calcResistance(master_tfNode->n_vSupply);
// }

// float SMAController::calcResistance(float V1)
// {
//     float res_sum = 0.0f;
//     int valid_samples = 0;

//     for (int i = 0; i < 10; i++)
//     {
//         float V2 = voltage_history[i];
//         float I = current_history[i];

//         if (I == 0.0f)
//             continue; // Skip if current is zero

//         float res = 1000.0f * fabs((V1 - V2) / I); // Convert to milliohms if needed
//         res_sum += res;
//         valid_samples++;
//     }

//     if (valid_samples > 0)
//         return res_sum / valid_samples; // Return average resistance
//     else
//         return 9999999999.0f; // Return a high value if no valid samples
// }

// Runs all sensor measurmentes
void SMAController::measure()
{
    // ONLY MEASURE ON HIGH VALUE OF PWM
    //Serial.print("Measuring ");
    //Serial.println(name);
    vld_val = readLoadVoltage();    
    curr_val = readMuscleAmps();
    rld_val = calcResistance(master_tfNode->n_vSupply, vld_val, curr_val);
}

// Callable by PWMSamplerDriver with a reference to a TF_Muscle
void SMAController::static_measure(SMAController *m)
{
    //Serial.println("measure");
    m->measure();
}

// Using current amplifier: https://www.ti.com/product/INA301?bm-verify=AAQAAAAJ_____3vvQVqhDs6dN-q2F7TfcduLcXGPYHN_yemLpkjLOSFozABq8zjBc6aZ1bHQMXuQcVRel2S374cevoKQ14rUg6LyRHkRgLS507wvwpDNZhvE-ZWK9hVspPwTC71ayCc3-WGCwn-CquFEWjRJKDJvsJXJVh4eel_qHn8Kcueux4PnQ39dUIMN51MZvpS9lZ7o7K9nCUaPUoq0s-TIxAvmatQPs4K61R7LI2pVSV_YWOOVyAT6TcwQ1i3h7ZRzdiO2DlOvNC6KJ8hoXFBhaxkZCcEV-xI63CAnv-xaEy2seyI
float SMAController::readMuscleAmps()
{
    float AcsValue=0.0,Samples=0.0,AvgAcs=0.0,raw=0.0;
    
      for (int x = 0; x < 10; x++){       // Get 10 samples
        //waitForPWMHigh();
        AcsValue = analogRead(curr_pin);  // Read current amp sensor values   
        Samples = Samples + AcsValue;     // Add samples together
      }
      raw = Samples / 10.0;  // Taking Average of Samples

      // For some unknown reason, there is an offset current of 5 amps...
      float amps = raw * VCC / (1023 * AMP_GAIN * R_SNS); //- 5; // Formula derived from voltage drop across sense resistor amplified and read from 0-1023

      if(amps < 0.0 && amps > -6.0f)
        return 0.0f;  // Return 0 if close to 0 (prevent negative resistance)
      return amps;
}

// Returns the voltage (VLD) after the load (muscle) -> used with readMuscleAmps() and getBatteryVolts() to calculate resistance
float SMAController::readLoadVoltage()
{
    float value=0.0,samples=0.0,avg_value=0.0,raw=0.0;

    for (int x = 0; x < 10; x++){   // Get 10 samples
    //waitForPWMHigh();
    //delayMicroseconds((1.0/490.0*1000000) * pwm_val/255.0 - 10); // Delay until almost the end of the pulse (10 microseconds before)
    value = analogRead(vld_pin);  // Read voltage divider values   
    samples += value;          // Add samples together
    }
    raw = samples / 10.0;  // Taking Average of Samples

    float volts = raw * vld_scaleFactor + vld_offset; // Values are attained experimentally with known input voltages
    if(volts < 0.0 && volts > -2.0)
    return 0.0f; // Return 0 volts for values close to 0
    return volts;
    //return raw;
}

// V=IR -> R=V/I -> Returns resistance in milliohms
float SMAController::calcResistance(float V1, float V2, float I)
{
    if(I == 0.0f)
        return 9999999999.0f; // Really high number
      return 1000 * abs((V1-V2) / I);
}


//=============================================================================
// Muscle Training Functions
//=============================================================================

void SMAController::resetTraining()
{
    trainState = MARTENSITE;
    resistTracker->reset();
    dir_tracker = 0;
    // Af_mohms = 0;
    dataPointsSinceAustenite = 0;
    currentGradIndex = resistTracker->oldestGradIndex;
    train_timer = millis(); // reeset timer
}

// Will step through the state machine when in training mode
float SMAController::updateTraining(float rld_mohms)
{
    switch(trainState) {
        case MARTENSITE: {  // STATE: Heating (Martensite phase)
          // I am thinking that a rolling average would be good. Track rolling average of 10 samples and when it has been decreasing for a few cycles then go to next state
          resistTracker->addData(rld_mohms);

          if(resistTracker->oldestGradIndex != currentGradIndex) {
            currentGradIndex = resistTracker->oldestGradIndex;
            // Get the gradient direction from data tracker
            GradientDirection dir = resistTracker->getCurrentDir();
            dir_tracker += dir == POSITIVE ? 1 :
                            dir == NEGATIVE ? -1 : 0;

            // Condition for next state (resistance decreases for DIR_TRIGGER intervals)
            if(dir_tracker > 0)
              dir_tracker = 0;
            else if(dir_tracker < -DIR_TRIGGER) {
              trainState = PHASE_TRANSITION;
              dir_tracker = 0;
            }
          }
          //return setpoint pwm value
          return setpoint[(int)tfnode::SMAControlMode::MODE_TRAIN];
        }
        case PHASE_TRANSITION: {  // STATE: Heating (Pre Austenite Finished Temp)
          // The same thing with a rolling average; wait until it has been increasing for a few cycles and find bottom of dip (set bottom to Af_mohms)
          resistTracker->addData(rld_mohms);
          dataPointsSinceAustenite++;

          if(resistTracker->oldestGradIndex != currentGradIndex) {
            currentGradIndex = resistTracker->oldestGradIndex;
            // Get the gradient direction from data tracker
            GradientDirection dir = resistTracker->getCurrentDir();
            dir_tracker += dir == POSITIVE ? 1 :
                          dir == NEGATIVE ? -1 : 0;

            // Condition for next state (wait for dir_tracker to increase over threshold)
            if(dir_tracker < 0)
              dir_tracker = 0;
            else if(dir_tracker > DIR_TRIGGER) {
              trainState = POST_AF;
              // #TODO Convert this function to getLocalMinimum from current position backward.  This oldestGradIndex should get current position
              int startIndex = resistTracker->oldestGradIndex + ((dataPointsSinceAustenite / (gradWidth-1)) + 1);
              startIndex = startIndex < 0 ? 0 : startIndex; // Get oldest data if transition was lost
              Af_mohms = resistTracker->getLocalMinimum(startIndex, resistTracker->oldestGradIndex);
              dir_tracker = 0;
            }
          }
          // Return setpoint pwm value
          return setpoint[(int)tfnode::SMAControlMode::MODE_TRAIN];
        }
        case POST_AF: {  // STATE: Heating (Post Austenite finished Temp)
          // Compare change in ohms to desired delta_mohms
          float delta_mohms_real = (rld_mohms - Af_mohms); // mohms
          // When ohms exceeds (Af_ohms + delta_ohms), start training timer. Flash LED while at temp
          // Return PID (with max value) to mitigate error between desired temperature and set temperature

          // Timer Condition
          return 0.01; // Just cut power for now.
          // #TODO add resistance control measures at this point
        }
        // STATE: Finished
        case TRAIN_FINISH: {
          CMD_setEnable(false); // When time exceeds training time, hold LED solid, disable, and do not enable until told to do so
          // Set LED color
          return 0;
        }
      }
      return 0;
}
