#include "PWMSamplerDriver.h"

// Initialize new object, configure pwm vars, and start timer
PWMSamplerDriver::PWMSamplerDriver(float frequency, float startDuty, int _pwm_pin, int _measure_delay, TF_Muscle_Callback _measureFunc, TF_Muscle *_instance) {//void (*_measureFunc)()) {
    pwm_freq_hz = frequency;
    measure_delay_us = _measure_delay;
    pwm_pin = _pwm_pin;
    measureFunc = _measureFunc;
    instance = _instance;

    min_duty_percent = (measure_delay_us / 1000000.0) / (1.0 / pwm_freq_hz);

    //Serial.print("Min duty percent: ");
    //Serial.println(min_duty_percent);
    
    pinMode(pwm_pin, OUTPUT);
    digitalWrite(pwm_pin, LOW);

    bool timerStarted = begintimer();
    //Serial.print("Timer started: ");
    //Serial.println(timerStarted);
    setDutyCyclePercent(startDuty);
}

// Triggered by interrupt of FSPTimer, switches between 3 states
void PWMSamplerDriver::timer_callback(timer_callback_args_t __attribute((unused)) *p_args) {

  //Serial.println("excuse me, i have pwm to generate.");  // Debug

  if (state == LOW_PWM) {
    //timer.begin(TIMER_MODE_ONESHOT, timer_type, tindex, dutyPercentToFrequency(duty_cycle_percent - min_duty_percent, pwm_freq_hz), 0.0f, timer_callback)
    timer.set_period_us(1000000.0 / pwm_freq_hz * min_duty_percent);
    timer.reset();
    state = HIGH_PWM;
    digitalWrite(pwm_pin, HIGH);
  }
  else if (state == HIGH_PWM) {
    //timer.begin(TIMER_MODE_ONESHOT, timer_type, tindex, dutyPercentToFrequency(min_duty_percent, pwm_freq_hz), 0.0f, timer_callback)
    timer.set_period_us(1000000.0 / pwm_freq_hz * (duty_cycle_percent - min_duty_percent));
    timer.reset();
    state = HIGH_PWM_MEASURE;
    // Call measure function
    measureFunc(instance);
  }
  else {
    timer.set_period_us(1000000.0 / pwm_freq_hz * (1.0 - duty_cycle_percent));
    timer.reset();
    state = LOW_PWM;
    digitalWrite(pwm_pin, LOW);
  }
}

// Converts a duty cycle from 0.0->1.0 to Hertz based on pwm_freq_hz
float PWMSamplerDriver::dutyPercentToFrequency(float duty)
{
    // 1 / pwm_Hz = period |||| period * duty = duty_period |||| 1 / duty_period = Hz |||| 1 / ((1.0/pwm_hz) * duty) = Hz
    return 1.0 / (duty * (1.0/pwm_freq_hz));
}

// Set the duty cycle with special case for 0% and 100%
void PWMSamplerDriver::setDutyCyclePercent(float percent) {

  //Serial.print("Setting duty cycle percent to: ");
  //Serial.println(percent);
  if(percent == duty_cycle_percent && percent >= 0 && percent != 1.0) { // If percent is 0 or 1.0, then we need the code below to run
    return;
  }

  // Catch minimum value and floor to 0
  if(percent <= min_duty_percent) {
    percent = 0;
    timer.stop();
    digitalWrite(pwm_pin, LOW);
    measureFunc(instance);  // Measure since it does not matter when measurement occurs at 0%
  }
  else if(percent >= 1.0) {
    percent = 1.0;
    timer.stop();
    digitalWrite(pwm_pin, HIGH);
    measureFunc(instance);  // Measure since it does not matter when measurement occurs at 100%
  }
  else {
    timer.set_period_us(1000000.0 / pwm_freq_hz); // Restart Pulse
    timer.reset();
    timer.start();
  }
  duty_cycle_percent = percent;
}

// Allocate and start a new timer in the correct mode (One Shot mode)
bool PWMSamplerDriver::begintimer()
{
    uint8_t timer_type = GPT_TIMER;
    int8_t tindex = FspTimer::get_available_timer(timer_type);
    if (tindex < 0){
        tindex = FspTimer::get_available_timer(timer_type, true);
    }
    if (tindex < 0){
        return false;
    }
    FspTimer::force_use_of_pwm_reserved_timer();

    // Debug
    //Serial.print("timer Index = ");
    //Serial.println(tindex);


    if(!timer.begin(TIMER_MODE_ONE_SHOT, timer_type, tindex, pwm_freq_hz, 0.0f, static_timer_callback, this)){
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

bool PWMSamplerDriver::startTimer()
{
    return timer.start();
}

bool PWMSamplerDriver::stopTimer()
{
    return timer.stop();
}

bool PWMSamplerDriver::resetTimer()
{
    return timer.reset();
}

PWMSamplerDriver::~PWMSamplerDriver() {
    timer.end();
}
