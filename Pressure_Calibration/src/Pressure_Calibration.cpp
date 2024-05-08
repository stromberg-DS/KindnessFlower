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

//EEPROM Setup 
int len = EEPROM.length();
int baselineAddress = 0x0001;
int lastBaselines[SENSOR_COUNT];


int pressureSamples [SENSOR_COUNT][SAMPLE_COUNT];
int avgBaselines[SENSOR_COUNT];
bool isSampling = true;

int testArray[SAMPLE_COUNT] = {9,9,9,9,9,9,8,8,3,4};

void getSensorSamples();
int getArrayAverage(int array[], int size);
int get2DArrayAvg(int array[SENSOR_COUNT][SAMPLE_COUNT], int currentPin);


void setup() {
  Serial.begin(9600);
  delay(5000);
  pinMode(D7, OUTPUT);
  digitalWrite(D7, LOW);

  EEPROM.get(baselineAddress, lastBaselines);

  for(int i=0; i<SENSOR_COUNT; i++){
      pinMode(PRESSURE_PINS[i], INPUT);
      Serial.printf("Last Avg #%i: %i\n",i , lastBaselines[i]);
  } 
  Serial.printf("\n");

}

void loop() {
  
  if (isSampling){
    digitalWrite(D7, HIGH);
    getSensorSamples();
    Serial.printf("\n\n");

    avgBaselines[0] = get2DArrayAvg(pressureSamples, 0);

    Serial.printf("Averages: \n");
    for (int i; i<SENSOR_COUNT; i++){
      avgBaselines[i] = get2DArrayAvg(pressureSamples, i);
      Serial.printf("    #%i - %i\n", i, avgBaselines[i]);
    }
    EEPROM.put(baselineAddress, avgBaselines);

    Serial.printf("\n\n");
    // Serial.printf("Average of sensor 0: %i\n", avgBaselines[0]);
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
      delay(100);
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