#include "drivers/PWMSamplerDriver.h"

#define FREQUENCY 20
#define PWM_PIN D3
#define MEASURE_DELAY_US 300

PWMSamplerDriver* driver;
SMAController* instance;

// Instance not needed but required for the function signature
void measure(SMAController* instance) {
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("Measure time");
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  while (!Serial) {}   // This line stops script until serial monitor attached

  PWMSamplerDriver::
  // put your setup code here, to run once:
  driver = new PWMSamplerDriver(FREQUENCY, 0.5, PWM_PIN, MEASURE_DELAY_US, measure, instance);
  driver->setDutyCyclePercent(1.0);
  delay(2000);
  driver->setDutyCyclePercent(0.0);
  delay(2000);
  driver->setDutyCyclePercent(0.5);
}

void loop(){
  // Do nothing.  PWM and sensor sampling is handled by the class
}
