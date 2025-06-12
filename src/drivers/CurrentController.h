#ifndef CURRENTCONTROLLER_H
#define CURRENTCONTROLLER_H

#include "MiniPID.h"

class CurrentController {
public:
    CurrentController(float targetCurrent, float kp, float ki, float kd);
    
    void update(float newCurrent);
    float getOutput();
    void setSetpoint(float newTargetCurrent);
    void Reset();

private:
    float currentCurrent;
    float targetCurrent;
    float outputPWM_percent;
    MiniPID *pid;
};

#endif // CURRENTCONTROLLER_H 