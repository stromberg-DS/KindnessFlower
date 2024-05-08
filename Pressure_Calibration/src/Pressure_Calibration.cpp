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
bool isSampling = true;

int testArray[SAMPLE_COUNT] = {9,9,9,9,9,9,8,8,3,4};

void getSensorSamples();
int getArrayAverage(int array[], int size);
int get2DArrayAvg(int array[SENSOR_COUNT][SAMPLE_COUNT], int currentPin);


void setup() {
  delay(5000);
  Serial.printf("Array Avg: %i\n", getArrayAverage(testArray, SAMPLE_COUNT));

  for(int i=0; i<SENSOR_COUNT; i++){
      pinMode(PRESSURE_PINS[i], INPUT);
  } 

}

void loop() {
  digitalWrite(D7, HIGH);

  if (isSampling){
    getSensorSamples();
    Serial.printf("\n\n");
    avgBaselines[0] = get2DArrayAvg(pressureSamples, 0);
    Serial.printf("Average of sensor 0: %i\n", avgBaselines[0]);
  }
  isSampling = false;
  digitalWrite(D7, LOW);


  // Serial.printf("Average of #0: %i\n", getArrayAverage(pressureSamples, SAMPLE_COUNT))

}

//Collect a bunch of samples from analog pins to be averaged later
void getSensorSamples(){
  for(int i=0; i<SAMPLE_COUNT; i++){
      Serial.printf("Sample #%i\n", i);
      for(int j=0; j<SENSOR_COUNT; j++){
        pressureSamples[j][i] = analogRead(PRESSURE_PINS[j]);
        Serial.printf("   #%i: %i\n", j, pressureSamples[j][i]);
      }
      Serial.printf("\n");
      
    } 
    
}

int get2DArrayAvg(int array[SENSOR_COUNT][SAMPLE_COUNT], int currentPin){
  int j;
  int average=0;
  int total = 0;

  for(j=0; j<SAMPLE_COUNT; j++){
    total += array[currentPin][j];
  }
  average = int(total/SAMPLE_COUNT);
  
  return average;
}

int getArrayAverage(int array[], int size){
  int j;
  int average = 0;
  int total = 0;
  for(j=0; j<size; j++){
    total += array[j];
  }
  average = int(total/size);

  return average;
}