#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

#include <Arduino.h>

class CommandProcessor {
public:
    void begin();
    void update();

private:
    void processSerial();
    void processCAN();
    void processSPI();
    void dispatchCommand(uint8_t deviceId, uint8_t functionCode, const uint8_t* params, size_t paramLength);
    void sendAcknowledgement(uint8_t deviceId, uint8_t statusCode);
};

#endif // COMMAND_PROCESSOR_H
