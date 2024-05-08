/* 
 * Pressure Sensor Calibration
 * Author: Daniel Stromberg
 * Date: 5/8/24
 * 
 * Get the input from all 4 (or 6?) FSR sensors for a few seconds
 *    then find the average and store it in EEPROM
*/

#include "Particle.h"

SYSTEM_MODE(MANUAL);
SYSTEM_THREAD(ENABLED);

const int arraySize =10;
int testArray[arraySize] = {4,8,3,9,8,7,8,8,3,4};

float getArrayAverage(int array[], int size);

void setup() {
  delay(10000);
  Serial.printf("Array Avg: %f\n", getArrayAverage(testArray, arraySize));

}

void loop() {


}

float getArrayAverage(int array[], int size){
  int j;
  float total = 0;
  for(j=0; j<size; j++){
    total += array[j];
  }

  return total/size;
}