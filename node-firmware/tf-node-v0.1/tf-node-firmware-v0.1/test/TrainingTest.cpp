/*=============================================================================
* A test script for the GradientTracker class.
* This script adds data to the tracker, then finds the local minimum of that data
*=============================================================================*/

#include "GradientTracker.cpp"
#include <stdio.h>

// RESISTANCE CONTROL MODE
enum TrainingState { MARTENSITE, PHASE_TRANSITION, POST_AF, TRAIN_FINISH };
TrainingState trainState = MARTENSITE;
const float timeStep_ms = 25.0f; // Time between measurement samples.  VERIFY THIS UNTIL MEASURE PERIOD IS CONSTANT!
float Af_mohms = -1; // RESISTANCE AT POINT OF Af (Austenite finished temperature)
float delta_mohms = 0;
///////////////////////// SET AS CHANGEALBE PARAMETERS IN FUTURE
const float At = 500;
const float Af = 89;
const float L_nitinol = (18.91f + 2.0f) * 2.54f; //inches to cm
const float D_nitinol = 0.2; //in cm
const float resistivity_per_c_nitinol = 0.05; //micro-ohms * cm / C
unsigned long TRAIN_TIMEOUT_MS = 4000;
unsigned long train_timer;
int dataPointsSinceAustenite = 0;
/////////////////////////
GradientTracker* resistTracker;
GradientDirection dir;
int gradWidth = 3;
int gradSize = 100;
float flatThreshold = 0.01;
int dir_tracker; // Keep track of direction and trigger into next state after detecting low/high
const int DIR_TRIGGER = 10; // Wait until direction is the same for 6 measurements
int currentGradIndex = -1;

//CONVERT PERCENT BETWEEN 0:1 TO POSITIVE PWM
int percentToPWM(float speed) {
  int pwm = abs(speed) * 255;
  return pwm;
}

void resetTraining() {
    trainState = MARTENSITE;
    resistTracker->reset();
    dir_tracker = 0;
    dataPointsSinceAustenite = 0;
    currentGradIndex = -1;
    //train_timer = millis(); //reeset timer
    printf("Reset Training\n");
}

// Will step through the state machine when in training mode
int updateTraining(float rld_mohms) {
    printf("Update: state: %d", trainState);

    switch(trainState) {
    case MARTENSITE: {  // STATE: Heating (Martensite phase)
        // I am thinking that a rolling average would be good. Track rolling average of 10 samples and when it has been decreasing for a few cycles then go to next state
        resistTracker->addData(rld_mohms);

        if(resistTracker->oldestGradIndex != currentGradIndex) {
            currentGradIndex = resistTracker->oldestGradIndex;
            // Get the gradient direction from data tracker
            dir = resistTracker->getCurrentDir();
            dir_tracker += dir == POSITIVE ? 1 :
                            dir == NEGATIVE ? -2 : 0;
            
            // Condition for next state (resistance decreases for DIR_TRIGGER intervals)
            if(dir_tracker > 0)
                dir_tracker = 0;
            else if(dir_tracker < -DIR_TRIGGER) {
                trainState = PHASE_TRANSITION;
                dir_tracker = 0;
            }
        }
        printf(" dir: %d tracker: %d ", dir, dir_tracker);
        printf("gradIndex: %d", resistTracker->oldestGradIndex);
        //return setpoint pwm value
        return percentToPWM(0.25f);
    }
    case PHASE_TRANSITION: {  // STATE: Heating (Pre Austenite Finished Temp)
        // The same thing with a rolling average; wait until it has been increasing for a few cycles and find bottom of dip (set bottom to Af_ohms)
        resistTracker->addData(rld_mohms);
        dataPointsSinceAustenite++;

        if(resistTracker->oldestGradIndex != currentGradIndex) {
            currentGradIndex = resistTracker->oldestGradIndex;
            // Get the gradient direction from data tracker
            dir = resistTracker->getCurrentDir();
            dir_tracker += dir == POSITIVE ? 2 :
                            dir == NEGATIVE ? -1 : 0;

            // Condition for next state (wait for dir_tracker to increase over threshold)
            if(dir_tracker < 0)
                dir_tracker = 0;
            else if(dir_tracker > DIR_TRIGGER) {
                trainState = POST_AF;
                // #TODO Convert this function to getLocalMinimum from current position backward.  This oldestGradIndex should get current position
                //dataPointsSinceAustenite / gradWidth + 1 should get the gradient just before entering this state
                int startIndex = resistTracker->oldestGradIndex - (dataPointsSinceAustenite / gradWidth + 1);
                startIndex = startIndex < 0 ? 0 : startIndex; // Get oldest data if transition was lost
                Af_mohms = resistTracker->getLocalMinimum(startIndex, resistTracker->oldestGradIndex);
                printf(" Af_mohms: %f \n", Af_mohms);
                dir_tracker = 0;
            }
        }
        printf(" dir: %d tracker: %d", dir, dir_tracker);
        printf("gradIndex: %d", resistTracker->oldestGradIndex);
        // Condition for next state (resistance increases for x intervals)
        // Calculate local minimum (Af_res)
        // Else return setpoint pwm value
        return percentToPWM(0.25f);
    }
    case POST_AF: {  // STATE: Heating (Post Austenite finished Temp)
        // Compare change in ohms to desired delta_mohms
        float delta_mohms_real = (rld_mohms - Af_mohms); //mohms
        printf(" delta_real: %f delta_mohms: %f", delta_mohms_real, delta_mohms);
        // When ohms exceeds (Af_ohms + delta_ohms), start training timer. Flash LED while at temp
        // Return PID (with max value) to mitigate error between desired temperature and set temperature

        // Timer Condition
        return 0; // Just cut power for now.
    }
    // STATE: Finished
    case TRAIN_FINISH: {
        //setEnable(false); // When time exceeds training time, hold LED solid, disable, and do not enable until told to do so
        // Set LED color
        return 0;
    }
}
    printf("\n");
    return 0;
}

/*float testData[] = {350.0, 540.0, 590.0, 580.0, 580.0, 580.0, 580.0, 580.0, 590.0, 580.0, 
                    580.0, 580.0, 580.0, 570.0, 570.0, 570.0, 580.0, 570.0, 560.0, 560.0, 
                    560.0, 550.0, 550.0, 550.0, 560.0, 540.0, 540.0, 540.0, 540.0, 540.0, 
                    540.0, 530.0, 540.0, 520.0, 520.0, 520.0, 520.0, 510.0, 510.0, 510.0, 
                    520.0, 520.0, 510.0, 510.0, 510.0, 510.0, 510.0, 500.0, 510.0, 510.0, 
                    510.0, 500.0, 500.0, 500.0, 510.0, 510.0, 510.0, 510.0, 500.0, 500.0, 
                    510.0, 510.0, 510.0, 500.0, 510.0, 510.0, 510.0, 510.0, 510.0, 510.0, 
                    510.0, 510.0, 510.0, 510.0, 500.0, 500.0, 500.0, 500.0, 510.0, 510.0, 
                    510.0, 510.0, 510.0, 510.0, 510.0, 510.0, 510.0, 510.0, 520.0, 510.0, 
                    510.0, 510.0, 510.0, 510.0, 510.0, 510.0, 520.0, 510.0, 510.0, 510.0, 
                    510.0, 510.0, 510.0, 510.0, 520.0, 510.0, 510.0, 510.0, 510.0, 510.0, 
                    510.0, 510.0, 520.0, 520.0, 520.0, 510.0, 520.0, 520.0, 520.0, 520.0, 
                    520.0, 520.0 };*/

float testData[] = { 4190.0, 4140.0, 4230.0, 4270.0, 4340.0, 4210.0, 4210.0, 4270.0, 4250.0, 4180.0,
                     4250.0, 3820.0, 4270.0, 4100.0, 4180.0, 4150.0, 4059.9999999999995, 4250.0, 4120.0, 4210.0, 
                     4059.9999999999995, 4059.9999999999995, 4059.9999999999995, 3950.0, 3970.0, 3950.0, 4140.0, 3980.0, 3820.0, 3920.0, 
                     4090.0, 3870.0, 3920.0, 3880.0, 3950.0, 3900.0, 3920.0, 3850.0, 4000.0, 4170.0, 
                     3790.0, 3840.0, 3840.0, 3800.0, 3870.0, 3750.0, 3840.0, 3870.0, 3720.0, 3830.0, 
                     3800.0, 3910.0, 3870.0, 3950.0, 3790.0, 3820.0, 3780.0, 3780.0, 3780.0, 3900.0, 
                     3850.0, 3820.0, 3690.0, 3850.0, 3750.0, 3810.0, 3850.0, 3780.0, 3850.0, 3780.0, 
                     3880.0, 3720.0, 3820.0, 3820.0, 3820.0, 5290.0, 8290.0 };

int main() {
    delta_mohms = ((At - Af) * L_nitinol / (3.14159f * (D_nitinol / 2)*(D_nitinol / 2)) * resistivity_per_c_nitinol) * 0.001; //results in milliohms
    printf("\ndelta_momhs: %f\n", delta_mohms);
    resistTracker = new GradientTracker(gradWidth, gradSize,  flatThreshold, timeStep_ms);
    resetTraining();

    // Add test data to the tracker
    for (int i = 0; i < sizeof(testData)/sizeof(testData[0]); i++) {
        //tracker->addData(testData[i]);
        printf("data: %f ", testData[i]);
        updateTraining(testData[i]);
        printf(" Gradient: %f\n", resistTracker->getCurrentGradient());
    }

    // Find the local minimum in the gradient array.
    // Change these values to find minimum within a range
    //int startIndex = 0;
    //int endIndex = 6; // Be aware that end index should not exceed beyond the frames that have been captured 

    //float localMin = tracker->getLocalMinimum(startIndex, endIndex);
    //printf("LocalMin: %f", localMin);
}