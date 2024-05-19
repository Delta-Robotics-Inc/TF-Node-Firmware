#ifndef GRADIENT_TRACKER_HPP
#define GRADIENT_TRACKER_HPP

#include <Arduino.h>

enum GradientDirection { FLAT, POSITIVE, NEGATIVE };

// This class will store data points and track the gradient through them based on a width of gradient data to perform a least squares method gradient
class GradientTracker {
public:
    GradientTracker();
    void addData(int newData);
    GradientDirection getCurrentDir() const;
    GradientDirection getDirection(float gradient) const;
    float getCurrentGradient() const;
    float getGradient(int index) const;
    void getDataSegment(int gradIndex, int* segment) const;

private:
    void insertData(int* array, int size, int newData);
    void insertData(float* array, int size, float newData);
    float calculateAverageGradient(int* array, int width) const;

    // CONSTANT PARAMS
    const int gradWidth = 5;          // Lower widths will detect smaller time intervals of change, but will be affected by noisy data
    const int gradArrSize = 50;       // dataStore will be gradArrSize * gradWidth.  How large is memory of 
    const float flatThreshold = 0.1;  // How flat is considered "flat"?  Flat is important to keep track of so that small changes do not trigger state changes in the larger program

    int* dataStore;  // Stores data points to be analyzed by gradient function, and can be 
    float* gradArr;  // 
    int dataCount;   // 
};

#endif // GRADIENT_TRACKER_H
