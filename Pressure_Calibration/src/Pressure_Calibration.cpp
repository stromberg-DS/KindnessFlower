/* 
 * Pressure Sensor Calibration
 * Author: Daniel Stromberg
 * Date: 5/8/24
 * 
 * Get the input from all 4 FSR sensors for a few seconds
 *    then find the average and store it in EEPROM
*/

#include "Particle.h"
#include "neopixel.h"

////////KEEEP AUTOMATIC///////////
SYSTEM_MODE(AUTOMATIC);
/////////////////////////////////
SYSTEM_THREAD(ENABLED);

const int SENSOR_COUNT = 4;
const int SAMPLE_COUNT =100;
const int PIXEL_COUNT = 112;
//Below MUST match the order in the final code
const int PRESSURE_PINS[SENSOR_COUNT] = {A5, A0, A2, A1}; 

//EEPROM Setup 
int len = EEPROM.length();
int baselineAddress = 0x0001;
int lastBaselines[SENSOR_COUNT];


int pressureSamples [SENSOR_COUNT][SAMPLE_COUNT];
int avgBaselines[SENSOR_COUNT];
bool isSampling = true;
String pubText[4] = {"Baseline #1", "Baseline #2", "Baseline #3", "Baseline #4"};

void getSensorSamples();
int get2DArrayAvg(int array[SENSOR_COUNT][SAMPLE_COUNT], int currentPin);
int lastMillis = 0;
bool isLEDOn = true;

Adafruit_NeoPixel pixel(PIXEL_COUNT, SPI1, WS2812);

void setup() {
  Serial.begin(9600);
  pixel.begin();
  pixel.setBrightness(50);
  pixel.clear();
  pixel.show();

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
  
  if (isSampling){    //only go through this once
    getSensorSamples();
    Serial.printf("\n\n");

    // avgBaselines[0] = get2DArrayAvg(pressureSamples, 0);

    Serial.printf("Averages: \n");
    for (int i=0; i<SENSOR_COUNT; i++){
      avgBaselines[i] = get2DArrayAvg(pressureSamples, i);
      Serial.printf("    #%i - %i\n", i, avgBaselines[i]);
    }

    EEPROM.put(baselineAddress, avgBaselines);
    Serial.printf("\n\n");

    //Send baselines to Particle Cloud
    Particle.publish("Done Sampling!");
    for (int j=0; j<SENSOR_COUNT; j++){
      Particle.publish(String(pubText[j]), String(avgBaselines[j]));
      delay(1000);
    }
    for(int k=0; k<5; k++){
      pixel.setBrightness(5);
      pixel.show();
      delay(250);
      pixel.setBrightness(50);
      pixel.show();
      delay(250);
    }
    pixel.clear();
    pixel.show();
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
  int lastPixelToLight = 0;
  for(int i=0; i<SAMPLE_COUNT; i++){      
    lastPixelToLight = map(i, 0, SAMPLE_COUNT, 0, PIXEL_COUNT);
    pixel.setPixelColor(lastPixelToLight, 0x00FFFF);
    pixel.show();
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