/*
* This program simulates the PWM output to a muscle with a delayed measurement for the current and voltage sensors.
* I need the measurements to occur 300 us after the pwm pulse occurs.
*/


// const float FREQUENCY = 20; // hz
// const float DUTY_CYCLE = 0.5; // percentage from 0.0->1.0
// const int SENSOR_DELAY_TIME = 300; //us

// int pin = 3;
// int sensor_trigger = 13;

// void setup() {
//   pinMode(pin, OUTPUT);
//   pinMode(sensor_trigger, OUTPUT);
// }

// void loop() {
//   digitalWrite(pin, HIGH);
//   // code here for delay and measure
//   delayMicroseconds(SENSOR_DELAY_TIME);
//   digitalWrite(sensor_trigger, HIGH);
//   delayMicroseconds(1.0/FREQUENCY*1000000 * DUTY_CYCLE - SENSOR_DELAY_TIME);
//   digitalWrite(pin, LOW);
//   digitalWrite(sensor_trigger, LOW);
//   delayMicroseconds(1.0/FREQUENCY*1000000 * (1.0-DUTY_CYCLE) - SENSOR_DELAY_TIME);
// }


//=============================================================================
//=============================================================================
//=============================================================================


// #include "pwm.h"

// #define PWM_FREQUENCY_HZ 20
// #define MIN_ON_TIME_US 300

// //PwmOut pwm(D3);
// int sensor_trigger = 13;

// void setup() {
//     //period 50us = 20000hz; pulse 0 us = 0%
//     //pwm.begin(PWM_FREQUENCY_HZ, 0.0f);

//     // set 50%
//     //pwm.pulse_perc(50.0f);
    
    
//     pinMode(sensor_trigger, OUTPUT);
//     attachInterrupt(digitalPinToInterrupt(D3), writeHigh, CHANGE);
    
//     // Add callback to timer interrupt handler
//     //pwm.get_timer()->set_irq_callback(writeHigh, nullptr);
//     analogWrite(D3, 128);
// }

// void loop() {
//     // Your main code here
//     /*if (digitalRead(D3) == HIGH) {
//       delayMicroseconds(300);
//       digitalWrite(sensor_trigger, HIGH);
//     }
//     else {
//       digitalWrite(sensor_trigger, LOW);
//     }*/
// }

// void writeHigh() {
//   digitalWrite(sensor_trigger, HIGH);
// }


//=============================================================================
//=============================================================================
//=============================================================================


#include <Wire.h>          // I2C
#include <FspTimer.h>      // Timer Support
FspTimer timer;

#define PWM_FREQUENCY_HZ 20
#define MIN_ON_TIME_US 300
#define PWM_PIN D3
#define MEASURE_PIN D13

float duty_cycle_percent = 0.5;
const float min_duty_percent = (MIN_ON_TIME_US/1000000.0) / (1.0 / PWM_FREQUENCY_HZ); // Percentage of MIN_ON_TIME of the total pwm pulse

/******************************************************/
//                  Timer                            //
/******************************************************/
enum PWM_STATE { LOW_PWM, HIGH_PWM, HIGH_PWM_MEASURE };
PWM_STATE state = LOW_PWM;

void timer_callback(timer_callback_args_t __attribute((unused)) *p_args) {

  if (state == LOW_PWM) {
    //timer.begin(TIMER_MODE_ONESHOT, timer_type, tindex, dutyPercentToFrequency(duty_cycle_percent - min_duty_percent, PWM_FREQUENCY_HZ), 0.0f, timer_callback)
    timer.set_period_us(1000000.0 / PWM_FREQUENCY_HZ * min_duty_percent);
    timer.reset();
    state = HIGH_PWM;
    digitalWrite(PWM_PIN, HIGH);
  }
  else if (state == HIGH_PWM) {
    //timer.begin(TIMER_MODE_ONESHOT, timer_type, tindex, dutyPercentToFrequency(min_duty_percent, PWM_FREQUENCY_HZ), 0.0f, timer_callback)
    timer.set_period_us(1000000.0 / PWM_FREQUENCY_HZ * (duty_cycle_percent - min_duty_percent));
    timer.reset();
    state = HIGH_PWM_MEASURE;
    digitalWrite(MEASURE_PIN, HIGH);
  }
  else {
    timer.set_period_us(1000000.0 / PWM_FREQUENCY_HZ * (1.0 - duty_cycle_percent));
    timer.reset();
    state = LOW_PWM;
    digitalWrite(PWM_PIN, LOW);
    digitalWrite(MEASURE_PIN, LOW);
  }
}

void setDutyCyclePercent(float percent) {
  if(percent <= min_duty_percent) {
    percent = 0;
    timer.stop();
    digitalWrite(PWM_PIN, LOW);
  }
  else if(percent >= 1.0) { 
    percent = 1.0;
    timer.stop();
    digitalWrite(PWM_PIN, HIGH);
  }
  else {
    timer.set_period_us(1000000.0 / PWM_FREQUENCY_HZ); // Restart Pulse
    timer.reset();
    timer.start();
  }
  duty_cycle_percent = percent;
}

// Converts a duty cycle from 0.0->1.0 to Hertz based on PWM_FREQUENCY
float dutyPercentToFrequency(float duty, float pwm_frequency) {
  // 1 / pwm_Hz = period // period * duty = duty_period // 1 / duty_period = Hz // 1 / ((1.0/pwm_hz) * duty) = Hz
  return 1.0 / (duty * (1.0/pwm_frequency));
}

bool begintimer() {
  uint8_t timer_type = GPT_TIMER;
  int8_t tindex = FspTimer::get_available_timer(timer_type);
  if (tindex < 0){
    tindex = FspTimer::get_available_timer(timer_type, true);
  }
  if (tindex < 0){
    return false;
  }
  FspTimer::force_use_of_pwm_reserved_timer();

  Serial.print("timer Index = ");
  Serial.println(tindex);

  if(!timer.begin(TIMER_MODE_ONE_SHOT, timer_type, tindex, PWM_FREQUENCY_HZ, 0.0f, timer_callback)){
    return false;
  }

  if (!timer.setup_overflow_irq()){
    return false;
  }

  if (!timer.open()){
    return false;
  }

  if (!timer.start()){
    return false;
  }
  return true;
}

/******************************************************/
//                                                    //
//                  Setup                             //
//                                                    //
/******************************************************/
void setup() {

  Serial.begin(115200);
  while (!Serial) {}   // This line stops script until serial monitor attached
  Serial.println(begintimer());
   
  pinMode(PWM_PIN, OUTPUT);
  pinMode(MEASURE_PIN, OUTPUT);

  delay(3000);
  setDutyCyclePercent(0.1);
  delay(3000);
  setDutyCyclePercent(0.0);
  delay(3000);
  setDutyCyclePercent(1.0);
}
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
void loop() {
  
}

