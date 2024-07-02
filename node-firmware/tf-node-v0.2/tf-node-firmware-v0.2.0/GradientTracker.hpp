#ifndef GRADIENT_TRACKER_HPP
#define GRADIENT_TRACKER_HPP

#include <stdio.h> // Remove for arduino
enum GradientDirection { FLAT, POSITIVE, NEGATIVE };

// undefine stdlib's or Arduino abs if encountered
#ifdef abs
#undef abs
#endif // abs

#define abs(x) ((x)>0?(x):-(x))

// This class will store data points and track the gradient through them based on a width of gradient data to perform a least squares method gradient
class GradientTracker {
public:
    GradientTracker(int gradWidth, int gradArrSize, float flatThreshold, float timeStep); //gradWidth CANNOT be 1. (otherwise gradient cannot calc)
    ~GradientTracker();
    void reset();

    void addData(int newData);
    float getData(float* array, int size, int oldestIndex, int indexFromStart) const;
    int getData(int* array, int size, int oldestIndex, int indexFromStart) const;

    GradientDirection getCurrentDir() const;
    GradientDirection getDirection(float gradient) const;
    float getCurrentGradient() const;
    float getGradient(int index) const;
    void getDataSegment(int gradIndex, int* segment) const;
    float getLocalMinimum(int startIndex, int endIndex) const;

    // CONSTANT PARAMS
    int gradWidth = 5;          // Lower widths will detect smaller time intervals of change, but will be affected by noisy data
    int gradArrSize = 50;       // dataStore will be gradArrSize * gradWidth.  How large is memory of 
    float flatThreshold = 0.1;  // How flat is considered "flat"?  Flat is important to keep track of so that small changes do not trigger state changes in the larger program
    float timeStep; // Time in ms between measured samples (VERIFY this constantly until samples are a constant period)
    int dataArrSize;
    int oldestDataIndex;
    int oldestGradIndex;

private:
    void insertData(int* array, int size, int newData);
    void insertData(float* array, int size, float newData);
    float calculateAverageGradient(float* array, int width) const;
    void applyMovingAverage(float* data, int size, int windowSize) const;

    float* dataStore;  // Stores data points to be analyzed by gradient function, and can be 
    float* gradArr;  // 
    int dataCount;   // 
};

#endif // GRADIENT_TRACKER_HPP
