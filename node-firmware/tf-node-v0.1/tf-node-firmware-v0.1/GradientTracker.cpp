#include "GradientTracker.hpp"

GradientTracker::GradientTracker()
    : dataCount(0) {
    dataStore = new int[gradArrSize * gradWidth];
    gradArr = new float[gradArrSize];
    for (int i = 0; i < gradArrSize * gradWidth; i++) {
        dataStore[i] = -1; // Negative 1 will be empty value
    }
    for (int i = 0; i < gradArrSize; i++) {
        gradArr[i] = -1.0; // Negative 1 will be empty value
    }
}

void GradientTracker::addData(int newData) {
    insertData(dataStore, gradArrSize * gradWidth, newData);  // Insert new data at the beginning of the dataStore array
    dataCount++;  // Increment dataCount and calculate gradient if we have enough data
    if (dataCount == gradWidth) {
        float gradient = calculateAverageGradient(dataStore, gradWidth);
        insertData(gradArr, gradArrSize, gradient);  // Insert gradient into gradArr
        dataCount = 0;  // Reset dataCount
    }
}

// Returns the Current GradientDirection at the beginning of the array
GradientDirection GradientTracker::getCurrentDir() const {
    return getDirection(gradArr[0]);
}

float GradientTracker::getCurrentGradient() const {
    return gradArr[0];
}

float GradientTracker::getGradient(int index) const {
    if (index < 0 || index >= gradArrSize) {
        return 0.0; // Out of bounds, return 0.0
    }
    return gradArr[index];
}

void GradientTracker::getDataSegment(int gradIndex, int* segment) const {
    if (gradIndex < 0 || gradIndex >= gradArrSize) {
        return; // Out of bounds, do nothing
    }
    for (int i = 0; i < gradWidth; i++) {
        segment[i] = dataStore[gradIndex * gradWidth + i];
    }
}

void GradientTracker::insertData(int* array, int size, int newData) {
   // Shift all elements to the right
    for (int i = size - 1; i > 0; i--) {
        array[i] = array[i - 1];
    }
    array[0] = newData;  // Insert the new data at the beginning
}

void GradientTracker::insertData(float* array, int size, float newData) {
    // Shift all elements to the right
    for (int i = size - 1; i > 0; i--) {
        array[i] = array[i - 1];
    }
    array[0] = newData;  // Insert the new data at the beginning
}

float GradientTracker::calculateAverageGradient(int* array, int width) const{
    // Calculate the average gradient (slope of the best fit line) using least squares method
    float sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;
    for (int i = 0; i < width; i++) {
        sumX += i;
        sumY += array[i];
        sumXY += i * array[i];
        sumX2 += i * i;
    }
    float numerator = width * sumXY - sumX * sumY;
    float denominator = width * sumX2 - sumX * sumX;
    return numerator / denominator;
}

GradientDirection GradientTracker::getDirection(float gradient) const {
    if (abs(gradient) < flatThreshold) {
        return FLAT;
    } else if (gradient > 0) {
        return POSITIVE;
    } else {
        return NEGATIVE;
    }
}
