#ifndef SMA_CONTROLLER_H
#define SMA_CONTROLLER_H

#include "PWMSamplerDriver.h"

class SMAController {
public:
    SMAController(uint8_t portNumber, uint8_t pwmPin, uint8_t feedbackPin);
    void begin();
    void update();
    // Do I put executeCommand here or implement in a separate class/interface?
    //void executeCommand(uint8_t functionCode, const uint8_t* params, size_t paramLength);

private:
    uint8_t portNumber;
    uint8_t pwmPin;
    uint8_t feedbackPin;
    PWMSamplerDriver pwmDriver;
    // Other member variables (e.g., PID controller)
};

#endif // SMA_CONTROLLER_H
