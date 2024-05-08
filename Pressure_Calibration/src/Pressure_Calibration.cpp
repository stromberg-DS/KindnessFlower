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

const int SENSOR_COUNT = 6;
const int SAMPLE_COUNT =10;
const int PRESSURE_PINS[6] = {A0, A1, A2, A5, A4, A3};

int pressureSamples [SENSOR_COUNT][SAMPLE_COUNT];
int avgBaselines[SENSOR_COUNT];

int testArray[SAMPLE_COUNT] = {4,8,3,9,8,7,8,8,3,4};

float getArrayAverage(int array[], int size);

void setup() {
  delay(5000);
  Serial.printf("Array Avg: %f\n", getArrayAverage(testArray, SAMPLE_COUNT));

  for(int i=0; i<SENSOR_COUNT; i++){
      pinMode(PRESSURE_PINS[i], INPUT);
  } 

}

void loop() {
 static bool isSampling = true;

  digitalWrite(D7, HIGH);

  if (isSampling){
    for(int i=0; i<SAMPLE_COUNT; i++){
      Serial.printf("Sample #%i\n", i);
      for(int j=0; j<SENSOR_COUNT; j++){
        pressureSamples[j][i] = analogRead(PRESSURE_PINS[j]);
        Serial.printf("   #%i: %i\n", j, pressureSamples[j][i]);
      }
      Serial.printf("\n");
      
    } 
    digitalWrite(D7, LOW);
    isSampling = false;
  }

}

float getArrayAverage(int array[], int size){
  int j;
  float total = 0;
  for(j=0; j<size; j++){
    total += array[j];
  }

  return total/size;
}