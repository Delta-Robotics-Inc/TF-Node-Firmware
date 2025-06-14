#include "GradientTracker.hpp"

GradientTracker::GradientTracker(int gradWidth, int gradArrSize, float flatThreshold, float timeStep)
    : gradWidth(gradWidth), gradArrSize(gradArrSize), flatThreshold(flatThreshold), timeStep(timeStep), dataCount(0) {
    dataArrSize = gradArrSize * (gradWidth-1) + 1;
    dataStore = new float[dataArrSize]; //START/END POINTS SHOULD BE INCLUSIVE
    gradArr = new float[gradArrSize];
    reset();
}

GradientTracker::~GradientTracker() {
    delete[] dataStore;
    delete[] gradArr;
}

void GradientTracker::reset() {
    for (int i = 0; i < dataArrSize; i++) {
        dataStore[i] = -1; // will be empty value
    }
    for (int i = 0; i < gradArrSize; i++) {
        gradArr[i] = 0.00001; // will be empty value
    }
    dataCount = 0;
    oldestDataIndex = -1;
    oldestGradIndex = -1;
}

void GradientTracker::addData(int newData) {
    insertData(dataStore, dataArrSize, newData);  // Insert new data at the beginning of the dataStore array

    // Increment and reset after overflow
    oldestDataIndex++;
    if(oldestDataIndex >= dataArrSize)
        oldestDataIndex = 0;

    dataCount++;  // Increment dataCount and calculate gradient if we have enough data
    if (dataCount == gradWidth) {
        applyMovingAverage(dataStore, dataArrSize, gradWidth);
        float gradient = calculateAverageGradient(dataStore, gradWidth);
        insertData(gradArr, gradArrSize, gradient);  // Insert gradient into gradArr

        // Increment and reset after overflow
        oldestGradIndex++;
        if(oldestGradIndex >= gradArrSize)
            oldestGradIndex = 0;

        dataCount = 1;  // Reset dataCount. Reset to 1 after first run because next gradient shall include last point of this section.
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
    return getData(gradArr, gradArrSize, oldestGradIndex, index); //gradArr[index];
}

void GradientTracker::getDataSegment(int gradIndex, int* segment) const {
    if (gradIndex < 0 || gradIndex >= gradArrSize) {
        return; // Out of bounds, do nothing
    }
    for (int i = 0; i < gradWidth; i++) {
        segment[i] = getData(dataStore, dataArrSize, oldestDataIndex, gradIndex * (gradWidth-1) + 1 + i);
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

// Get data in rolling queue from oldest data.  Pass oldestIndex as indexFromStart to get the 0th element
float GradientTracker::getData(float* array, int size, int oldestIndex, int indexFromStart) const {
    // If index is larger than the size of the array, then exit
    if(size < 1 || indexFromStart >= size || indexFromStart < 0)
        return 0;

    // This array traverses backwards
    int index = oldestIndex - indexFromStart;

    // If data needs to roll over
    if(index < 0)
        index += size;
    return array[index];
}

int GradientTracker::getData(int* array, int size, int oldestIndex, int indexFromStart) const {
    if (size < 1 || indexFromStart >= size || indexFromStart < 0)  // Corrected bounds check
        return 0;

    int index = oldestIndex - indexFromStart;
    if (index < 0)
        index += size;  // Corrected to use += instead of -=
    return array[index];
}

/*void GradientTracker::applyMovingAverage(float* data, int size, int windowSize) const {
    float* temp = new float[size];
    for (int i = 0; i < size; i++) {
        temp[i] = data[i];
    }
    
    for (int i = 0; i < size; i++) {
        float sum = 0.0;
        int count = 0;
        for (int j = 0; j < windowSize; j++) {
            int index = i + j;
            if (index < size) {
                sum += temp[index];
                count++;
            } else {
                break;
            }
        }
        data[i] = sum / count;
    }

    delete[] temp;
}*/

void GradientTracker::applyMovingAverage(float* data, int size, int windowSize) const {
    if (windowSize <= 0 || size <= 0) {
        return; // Nothing to process
    }

    // Apply moving average only to the first windowSize elements
    for (int i = 0; i < windowSize; i++) {
        float sum = 0.0;
        int count = 0;

        // Calculate start and end indices for the current window
        int start = 0;
        int end = i;

        // Calculate sum and count within the window
        for (int j = start; j <= end; j++) {
            sum += data[j];
            count++;
        }

        // Calculate moving average
        if (count > 0) {
            data[i] = sum / count;
        }
    }
}


// Gradient will always calculate at the first part of the data (same behavior as getData())
float GradientTracker::calculateAverageGradient(float* array, int width) const{
    // Calculate the average gradient (slope of the best fit line) using least squares method
    float sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;
    for (int i = 0; i < width; i++) {
        //printf(" %d ", array[(width-1)-i]);
        sumX += i * timeStep; // Convert index to time
        sumY += array[(width-1)-i];
        sumXY += (i * timeStep) * array[(width-1)-i];
        sumX2 += (i * timeStep) * (i * timeStep);
    }
    float numerator = width * sumXY - sumX * sumY;
    float denominator = width * sumX2 - sumX * sumX;
    //printf("= Gradient: %f\n", numerator / denominator);
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


// /**
//  * Find a local minimum in the data based on a range within the gradArr.
//  * 1. Determine the gradient segment that is the lowest within the range.
//  * 2. Orient the gradient slop on the data based on the average value of the gradient data
//  * 3. Calculate the lowest value in the data by starting from the average(middle) point in the gradient section, and moving in the negative gradient direction
//  **/
// float GradientTracker::getLocalMinimum(int startIndex, int endIndex) const {
//     if (startIndex < 0 || endIndex >= gradArrSize || startIndex > endIndex) {
//         return 0.0; // Out of bounds, return 0.0
//     }

//     // Debug
//     /*for(int i = 0; i < dataArrSize; i++) {
//         printf("i:%d d:%d\n", i, dataStore[i]);
//     }*/
    
//     // Move position by each gradient, and find the lowest point by updating our minium position if position decreases.
//     float position = getData(gradArr, gradArrSize, oldestGradIndex, startIndex);
//     float minPosition = position;
//     int minSegIndex = startIndex;
//     //printf("pos: %f\n", position);
//     for (int i = startIndex + 1; i <= endIndex; i++) {
//         position += getData(gradArr, gradArrSize, oldestGradIndex, i); //gradArr[i];
//         //printf("pos: %f\n", position);
//         // Update the minimum position and the segment index of that position
//         if(position < minPosition) {
//             minPosition = position;
//             minSegIndex = i;
//         }
        
//     }

//     //printf("minSegIndex: %d\n", minSegIndex);
    
//     // Calculate the average of the data segment for the minimum gradient
//     float average = 0.0f;
//     for (int i = 0; i < gradWidth; i++) {
//         average += float(getData(dataStore, dataArrSize, oldestDataIndex, minSegIndex * (gradWidth - 1) + i)); //dataStore[minSegIndex * gradWidth + i];
//     }
//     average /= gradWidth;
//     //printf("avg: %f\n", average);
    
//     // Calculate the local minimum based on the gradient and average value
//     // To ensure that the local minimum is always calculated in the downward direction, we should use the absolute value of the gradient. 
//     // This way, regardless of whether the gradient is positive or negative, the calculation will always reflect the lowest point.
//     float gradient = abs(getData(gradArr, gradArrSize, oldestGradIndex, minSegIndex));
//     //printf("grad: %f\n", gradient);
//     float localMinimum = average - (gradient * (gradWidth - 1) / 2.0 * timeStep);

//     return localMinimum;
// }

#define FLT_MAX 99999999.0f

float GradientTracker::getLocalMinimum(int startIndex, int endIndex) const {
    if (startIndex < 0 || endIndex >= gradArrSize || startIndex > endIndex) {
        return 0.0; // Out of bounds, return 0.0
    }

    // Initialize to a high value to find the minimum
    float localMinimum = FLT_MAX;

    // Iterate through the specified range in gradArr
    for (int i = startIndex+0; i <= endIndex; i++) {
        // Calculate the average of the data segment for the current gradient
        float average = 0.0f;
        for (int j = 0; j < gradWidth; j++) {
            float dataValue = getData(dataStore, dataArrSize, oldestDataIndex, i * (gradWidth - 1) + j);
            average += dataValue;
            printf("Segment: %d , Data Value: %d : %f\n", i, j, dataValue);
        }
        average /= gradWidth;
        printf(" average: %f ", average);

        // Check if this average is the lowest we've found
        if (average < localMinimum) {
            localMinimum = average;
        }
    }

    //printf("\n MIN: %f \n", localMinimum);
    return localMinimum;
}