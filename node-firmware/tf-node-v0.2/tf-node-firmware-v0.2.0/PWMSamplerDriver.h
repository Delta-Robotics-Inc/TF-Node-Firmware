//=============================================================================
//
// This class handles custom PWM generation at a specified frequency with interrupt functionality
// at a specified time within the PWM "On" pulse for the purpose of sensor sampling.
//
// The system starts up a timer, conducts pwm on the specified pwm_pin, and calls the measureFunc()
// given at initialization when specified to do so.  The purpose of specifying the timing within the
// pulse for measurements is to ensure synchronization of PWM pulse and sensor sampling at a delay to
// give time for the sensors to settle as a response to the PWM state change.
//
//=========================================================
//         | sample      | sample      |
//        _v____        _v____         | 
//       |      |      |      |        | Ideal pulse
// ______|      |______|      |______  |
//                                     |
//=========================================================
//                                     |
//         _____         _____         |
//        /     \       /     \        | Realistic system response
// ______/       \_____/       \_____  |
//                                     |
//=============================================================================

#include <FspTimer.h>      // Timer Support

class TF_Muscle;

using TF_Muscle_Callback = void(*)(TF_Muscle *);

class PWMSamplerDriver {
    public:
        PWMSamplerDriver(float frequency, float startDuty, int _pwm_pin, int _measure_delay, TF_Muscle_Callback _measureFunc, TF_Muscle *_instance);
        ~PWMSamplerDriver();
        void setDutyCyclePercent(float percent);
        enum PWM_STATE { LOW_PWM, HIGH_PWM, HIGH_PWM_MEASURE };
        PWM_STATE state = LOW_PWM;
        bool startTimer();
        bool stopTimer();
        bool resetTimer();

        static void static_timer_callback(timer_callback_args_t* p_args) {
          //Serial.println("Hit");
          auto* instance = static_cast<const PWMSamplerDriver*>(p_args->p_context);
          if (instance) {
              const_cast<PWMSamplerDriver*>(instance)->timer_callback(p_args);
          }
        }

    private:
        FspTimer timer;
        float pwm_freq_hz = 20;
        int measure_delay_us = 300;
        int pwm_pin;
        TF_Muscle_Callback measureFunc;
        TF_Muscle *instance;
        //void (*measureFunc)();
        float duty_cycle_percent = -1.0; // Different so that first call @ 0% will work
        float min_duty_percent;
        void timer_callback(timer_callback_args_t __attribute((unused)) *p_args);
        float dutyPercentToFrequency(float duty);
        bool begintimer();
};