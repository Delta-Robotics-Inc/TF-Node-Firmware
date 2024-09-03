#include "ResistiveController.h"

//#TODO Add delta_time update for system to update itself with correct timings

ResistiveController::ResistiveController(float targetResistance, float kp, float ki, float kd)
  : targetResistance(targetResistance), currentResistance(0.0), outputPWM_percent(0.0) {

    pid = new MiniPID(kp, ki, kd);

    pid->setOutputLimits(0.0, 1.0);
    pid->setOutputRampRate(10);
    // ... add any other configuration options for the PID here. 
}

void ResistiveController::update(float newResistance) {
    currentResistance = newResistance;

    //Run through the PID calculations, and get the desired output values
    outputPWM_percent = pid->getOutput(newResistance, targetResistance);
}

void ResistiveController::Reset() {
    pid->Reset();
}

// Return output of PID constrained from 0.0->1.0
float ResistiveController::getOutput() {
    float result = outputPWM_percent;

    result = result < 0.0 ? 0.0 :
             result > 1.0 ? 1.0 : result;

    return result;
}

void ResistiveController::setSetpoint(float newTargetResistance) {
    targetResistance = newTargetResistance;
}
