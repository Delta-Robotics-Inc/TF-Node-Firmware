#include "PWMSamplerDriver.h"
#include "SMAController.hpp"

volatile bool callback_in_progress = false;

// Initialize new object, configure pwm vars, and start timer
PWMSamplerDriver::PWMSamplerDriver(float frequency, float startDuty, int _pwm_pin, int _measure_delay, int _cycle_threshold, SMAController_Callback _measureFunc, SMAController *_instance) {
    pwm_freq_hz = frequency;
    measure_delay_us = _measure_delay;
    cycle_threshold = _cycle_threshold;
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
    setDutyCyclePercent(startDuty, false);
}

void PWMSamplerDriver::timer_callback(timer_callback_args_t __attribute((unused)) *p_args) {
    if (callback_in_progress) {
        // Skip if a previous callback is still in progress
        return;
    }

    callback_in_progress = true;


    if (!enabled) {
        digitalWrite(pwm_pin, LOW);
        timer.stop();
        callback_in_progress = false;
        return;
    }

    // Handle the 0% duty cycle case with periodic measurement using the state machine
    if (duty_cycle_percent == 0.0f && state != PWM_HIGH_PREMEASURE) {
        pulse_cycle_count++;  // Increment the pulse cycle count for 0% duty

        if (pulse_cycle_count >= cycle_threshold) {
            // Transition to PWM_HIGH_PREMEASURE to trigger a measurement
            state = PWM_HIGH_PREMEASURE;
            pulse_cycle_count = 0;  // Reset pulse counter
            digitalWrite(pwm_pin, HIGH);  // Temporarily set the pin high to perform the measurement
            timer.set_period_us(measure_delay_us);  // Wait for the measurement delay
            timer.reset();
        } else {
            // No measurement, keep the pin LOW and wait for the next cycle
            digitalWrite(pwm_pin, LOW);
            timer.set_period_us(1000000.0 / pwm_freq_hz);  // Set the timer for the next full cycle at 0% duty
            timer.reset();
        }
        callback_in_progress = false;
        return;
    }

    // Handle the PWM_LOW state for non-zero duty cycle
    if (state == PWM_LOW) {
        pulse_cycle_count++;  // Increment the pulse cycle count

        // Set pin high for the rising edge
        digitalWrite(pwm_pin, HIGH);

        // Determine the next state based on whether a measurement needs to occur
        if (duty_cycle_percent >= min_duty_percent) {
            // Transition to PWM_HIGH_PREMEASURE if measurement is needed
            state = PWM_HIGH_PREMEASURE;
            timer.set_period_us(measure_delay_us);  // Set the timer to wait for the measurement delay
        } else {
            // Handle low duty cycles with periodic measurement
            if (pulse_cycle_count >= cycle_threshold) {
                // Trigger a measurement and reset the cycle count
                state = PWM_HIGH_PREMEASURE;
                timer.set_period_us(measure_delay_us);  // Set the timer for the measurement delay
                pulse_cycle_count = 0;  // Reset pulse counter
            } else {
                // If no measurement is needed, transition directly to PWM_HIGH_POSTMEASURE
                state = PWM_HIGH_POSTMEASURE;
                timer.set_period_us(1000000.0 / pwm_freq_hz * duty_cycle_percent);  // Set the timer for the full on-time
            }
        }
        timer.reset();
    }

    // Handle the PWM_HIGH_PREMEASURE state
    else if (state == PWM_HIGH_PREMEASURE) {
        // Perform the measurement
        measureFunc(instance);

        if (duty_cycle_percent < min_duty_percent) {
            goto TRANSITIONLOW;  // Go to low state for low duty cycles
        }

        // Transition to PWM_HIGH_POSTMEASURE for the remainder of the high pulse
        state = PWM_HIGH_POSTMEASURE;
        timer.set_period_us(1000000.0 / pwm_freq_hz * duty_cycle_percent - measure_delay_us);  // Set remaining high time
        timer.reset();
    }

    // Handle the PWM_HIGH_POSTMEASURE state
    else if (state == PWM_HIGH_POSTMEASURE) {
    TRANSITIONLOW:
        digitalWrite(pwm_pin, LOW);  // Set pin low
        state = PWM_LOW;
        timer.set_period_us(1000000.0 / pwm_freq_hz * (1.0 - duty_cycle_percent));  // Set the timer for the off period
        timer.reset();
    }

    callback_in_progress = false;
}




// Set the duty cycle with special case for 0% and 100%
// Ensure that frequent calls don't affect the timer when duty cycle is the same
void PWMSamplerDriver::setDutyCyclePercent(float percent, bool _enabled) {
    enabled = _enabled;

    if (!enabled) {
        digitalWrite(pwm_pin, LOW);
        timer.stop();
        measureFunc(instance);
        return;
    }

    // If the percent hasn't changed, don't alter the timer or duty cycle
    if (percent == duty_cycle_percent && percent < 1.0) {
        // The duty cycle is the same as before and not 100%, so no changes
        return;
    }

    // Handle 0% duty cycle case separately
    if (percent <= 0.0) {
        duty_cycle_percent = 0.0;
        //pulse_cycle_count = 0;  // Reset the cycle counter for the 0% case
        state = PWM_LOW;  // Set to LOW state as the default
        digitalWrite(pwm_pin, LOW);  // Ensure the pin stays LOW
        timer.set_period_us(1000000.0 / pwm_freq_hz);  // Full cycle for 0% duty cycle
        timer.start();  // Keep timer running
        return;
    }

    // Handle 100% duty cycle case (constant HIGH)
    if (percent >= 1.0) {
        duty_cycle_percent = 1.0;
        state = PWM_HIGH_POSTMEASURE;  // Set to HIGH state
        digitalWrite(pwm_pin, HIGH);  // Keep the pin high
        timer.stop();  // No need to cycle the timer for 100% duty
        measureFunc(instance);  // Perform measurement at full duty, no timing needed
        return;
    }

    // Update duty cycle and timer for all other duty cycles
    duty_cycle_percent = percent;

    if (state == PWM_LOW) {
        // Set the off-time for the PWM_LOW state
        timer.set_period_us(1000000.0 / pwm_freq_hz * (1.0 - duty_cycle_percent));  // Off period based on duty cycle
    } else if (state == PWM_HIGH_PREMEASURE) {
        // Set the timer for the measurement delay in the PWM_HIGH_PREMEASURE state
        timer.set_period_us(measure_delay_us);
    } else if (state == PWM_HIGH_POSTMEASURE) {
        // Set the remaining on-time in the PWM_HIGH_POSTMEASURE state
        timer.set_period_us(1000000.0 / pwm_freq_hz * duty_cycle_percent - measure_delay_us);
    }

    timer.start();  // Ensure the timer continues running
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
    //Serial.println("PWMSamplerDriver::startTimer - Start Timer");
    return timer.start();
}

bool PWMSamplerDriver::stopTimer()
{
    //Serial.println("PWMSamplerDriver::stopTimer - Stop Timer");
    return timer.stop();
}

bool PWMSamplerDriver::resetTimer()
{
    //Serial.println("PWMSamplerDriver::resetTimer - Reset Timer");
    return timer.reset();
}

PWMSamplerDriver::~PWMSamplerDriver() {
    //Serial.println("PWMSamplerDriver destructor called");
    timer.stop();  // Stop the timer
    timer.end();   // Clean up the timer
}