#include "ResistiveController.h"

//#TODO Add delta_time update for system to update itself with correct timings

ResistiveController::ResistiveController(float targetResistance, float kp, float ki, float kd)
  : targetResistance(targetResistance), currentResistance(0.0), outputPWM_percent(0.0) {

    pid = new QuickPID(&currentResistance, &outputPWM_percent, &targetResistance);
    pid->SetTunings(kp, ki, kd);
    pid->SetMode(QuickPID::Control::automatic);
}

void ResistiveController::update(float newResistance) {
    currentResistance = newResistance;
    pid->Compute();
}

void ResistiveController::Reset() {
    pid->Reset();
}

// Return output of PID constrained from 0.0->1.0
float ResistiveController::getOutput() {
    float result = outputPWM_percent;

    /*result = result < 0.0 ? 0.0 :
             result > 1.0 ? 1.0 : result;*/

    return result;
}

void ResistiveController::setSetpoint(float newTargetResistance) {
    targetResistance = newTargetResistance;
}
