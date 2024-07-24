#ifndef RESISTIVECONTROLLER_H
#define RESISTIVECONTROLLER_H

#include <QuickPID.h>

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
    QuickPID* pid;
};

#endif // RESISTIVECONTROLLER_H
