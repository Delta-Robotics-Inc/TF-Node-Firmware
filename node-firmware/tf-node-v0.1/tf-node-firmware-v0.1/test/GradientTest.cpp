/*=============================================================================
* A test script for the GradientTracker class.
* This script adds data to the tracker, then finds the local minimum of that data
*=============================================================================*/

#include "GradientTracker.cpp"
#include <stdio.h>

const int gradWidth = 3;          // Lower widths will detect smaller time intervals of change, but will be affected by noisy data
const int gradArrSize = 50;       // dataStore will be gradArrSize * gradWidth.  How large is memory of 
const float flatThreshold = 0.1;
const float timeStep_ms = 1;
GradientTracker* tracker; //timeStep = 25 ms


int main() {

    tracker = new GradientTracker(gradWidth, gradArrSize,  flatThreshold, timeStep_ms);

    // Test data.  We expect the local minimum to be found near 5 if traversing entire array
    int testData[] = {10, 15, 20, 25, 30, 20, 10, 5, 15, 25, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80};
    //int testData[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    //int testData[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

    // Add test data to the tracker
    for (int i = 0; i < sizeof(testData)/sizeof(testData[0]); i++) {
        tracker->addData(testData[i]);
        //printf("Gradient: %f\n", tracker->getCurrentGradient());
    }

    // Find the local minimum in the gradient array.
    // Change these values to find minimum within a range
    int startIndex = 0;
    int endIndex = 6; // Be aware that end index should not exceed beyond the frames that have been captured 

    float localMin = tracker->getLocalMinimum(startIndex, endIndex);
    printf("LocalMin: %f", localMin);
}