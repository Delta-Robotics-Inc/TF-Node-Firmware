#ifndef RESISTIVECONTROLLER_H
#define RESISTIVECONTROLLER_H

#include "MiniPID.h"

class ResistiveController {
public:
    ResistiveController(float targetResistance, float kp, float ki, float kd);
    
    void update(float newResistance);
    float getOutput();
    void setSetpoint(float newTargetResistance);
    void Reset();

private:
    float currentResistance;
    float targetResistance;
    float outputPWM_percent;
    MiniPID *pid;
};

#endif // RESISTIVECONTROLLER_H
