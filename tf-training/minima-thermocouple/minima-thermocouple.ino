/*
* https://randomnerdtutorials.com/arduino-k-type-thermocouple-max6675/
*/
#include <max6675.h>

int thermoDO = 4;
int thermoCS = 5;
int thermoCLK = 6;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

// Calibration offset in Celsius
float calibrationOffsetC = -3.75;  // New calibration offset

// Moving average filter parameters
const int numReadings = 10;  // Number of readings to average
float readings[numReadings]; // Array to store readings
int readIndex = 0;           // Current reading index
float total = 0.0;           // Running total
float average = 0.0;         // Moving average

void setup() {
  Serial.begin(115200);
  delay(500); // Wait for MAX6675 chip to stabilize
  
  // Initialize all readings to 0
  for (int i = 0; i < numReadings; i++) {
    readings[i] = 0.0;
  }
}

void loop() {
  // Read the temperature in Celsius from the MAX6675
  float tempC = thermocouple.readCelsius();
  
  // Apply the calibration offset
  float calibratedTempC = tempC + calibrationOffsetC;
  
  // Subtract the last reading from the total
  total -= readings[readIndex];

  // Add the current reading to the array and the total
  readings[readIndex] = calibratedTempC;
  total += readings[readIndex];

  // Advance to the next position in the array
  readIndex = (readIndex + 1) % numReadings; // Wrap around if necessary

  // Calculate the moving average
  average = total / numReadings;

  // Round the smoothed data to the nearest whole number
  int roundedAverage = round(average);

  // Print the values for the Serial Plotter
  Serial.print(tempC);
  Serial.print('\t');  // Tab character separates the values for Serial Plotter
  Serial.print(calibratedTempC);
  Serial.print('\t');  // Tab character separates the values for Serial Plotter
  Serial.println(roundedAverage);  // Newline to end the line for Serial Plotter

  // Delay for at least 250ms between readings for the MAX6675 to update
  delay(250);
}
