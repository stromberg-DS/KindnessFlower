/* 
 * Pressure Sensor Calibration
 * Author: Daniel Stromberg
 * Date: 5/8/24
 * 
 * Get the input from all 4 (or 6?) FSR sensors for a few seconds
 *    then find the average and store it in EEPROM
*/

#include "Particle.h"

////////KEEEP AUTOMATIC///////////
SYSTEM_MODE(AUTOMATIC);
/////////////////////////////////
SYSTEM_THREAD(ENABLED);

const int SENSOR_COUNT = 4;
const int SAMPLE_COUNT =100;
const int PRESSURE_PINS[SENSOR_COUNT] = {A0, A1, A2, A5};  //, A4, A3};

//EEPROM Setup 
int len = EEPROM.length();
int baselineAddress = 0x0001;
int lastBaselines[SENSOR_COUNT];


int pressureSamples [SENSOR_COUNT][SAMPLE_COUNT];
int avgBaselines[SENSOR_COUNT];
bool isSampling = true;

void getSensorSamples();
int getArrayAverage(int array[], int size);
int get2DArrayAvg(int array[SENSOR_COUNT][SAMPLE_COUNT], int currentPin);
int lastMillis = 0;
bool isLEDOn = true;


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
  }else{
    digitalWrite(D7, LOW);
  }
  isSampling = false;

  if(millis()-lastMillis> 1000){
    isLEDOn = !isLEDOn;
    lastMillis = millis();
  }

  digitalWrite(D7, isLEDOn);
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