#include "CurrentController.h"

// TODO Add delta_time update for system to update itself with correct timings

CurrentController::CurrentController(float targetCurrent, float kp, float ki, float kd)
  : targetCurrent(targetCurrent), currentCurrent(0.0), outputPWM_percent(0.0) {

    pid = new MiniPID(kp, ki, kd);

    // Set output limits for PWM duty cycle (0 to 75% max for safety)
    pid->setOutputLimits(0.0, 0.75);
    
    // Set output ramp rate to prevent sudden changes
    pid->setOutputRampRate(10);
    
    // Additional PID configuration for current control
    // Current control typically needs faster response than resistance control
    pid->setSetpointRange(0.0);  // No setpoint limiting
    pid->setOutputFilter(0.1);   // Small amount of output filtering
    
    // Set max I output to prevent windup
    pid->setMaxIOutput(0.2);     // Limit integral contribution to 20% PWM
}

void CurrentController::update(float newCurrent) {
    currentCurrent = newCurrent;

    // Run through the PID calculations, and get the desired output values
    outputPWM_percent = pid->getOutput(newCurrent, targetCurrent);
}

void CurrentController::Reset() {
    pid->reset();
}

// Return output of PID constrained from 0.0->1.0
float CurrentController::getOutput() {
    float result = outputPWM_percent;

    // Additional safety bounds checking
    result = result < 0.0 ? 0.0 :
             result > 1.0 ? 1.0 : result;

    return result;
}

void CurrentController::setSetpoint(float newTargetCurrent) {
    targetCurrent = newTargetCurrent;
} 