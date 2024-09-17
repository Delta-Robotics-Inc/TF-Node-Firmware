#include "PWMSamplerDriver.h"

#define FREQUENCY 20
#define PWM_PIN D3
#define MEASURE_DELAY_US 300

PWMSamplerDriver* driver;

void measure() {
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("Measure time");
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  while (!Serial) {}   // This line stops script until serial monitor attached

  // put your setup code here, to run once:
  driver = new PWMSamplerDriver(FREQUENCY, 0.5, PWM_PIN, MEASURE_DELAY_US, measure);
  driver->setDutyCyclePercent(1.0);
  delay(2000);
  driver->setDutyCyclePercent(0.0);
  delay(2000);
  driver->setDutyCyclePercent(0.5);
}

void loop(){
  // Do nothing.  PWM and sensor sampling is handled by the class
}
