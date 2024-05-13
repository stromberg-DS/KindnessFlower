/* 
 * Kindness Flower
 *      Portable flower that visitors take around the museum floor
 *      with them. When they pass the flower to someone else, they
 *      can light up the whole flower by grabbing all 4 pressure
 *      sensors. We tally up the number of passes as a proxy for
 *      kind acts. Future possibility of using BLE to find location
 *      throughout the museum and activate parts of the museum as 
 *      the flower gets closer.
 * Author: Daniel Stromberg
 * Date: 5/13/24
 * Parts:
 *      Use the Adafruit guide to set up FSRs from Adafruit (ID:1075)
 *      https://learn.adafruit.com/force-sensitive-resistor-fsr/using-an-fsr
 */

#include "Particle.h"
#include <neopixel.h>

////////KEEEP AUTOMATIC///////////
SYSTEM_MODE(AUTOMATIC);
/////////////////////////////////
SYSTEM_THREAD(ENABLED);

const int SENSOR_COUNT = 4;
const int PIXEL_COUNT = 144;
const int PIXELS_PER_SEGMENT = 36;
const int PRESSURE_PINS[SENSOR_COUNT] = {A5, A0, A2, A1};
const int gripStrength = 200;     //how hard to squeeze to trigger LEDs. Higher=harder squeeze
int pressureBaselines[SENSOR_COUNT];

//EEPROM Setup 
int len = EEPROM.length();
const int BASELINE_ADDRESS = 0x0001;

int brightness = 30;
int pixelDelay = 50;
unsigned long pixelPrevious = 0;
int pixelCycle = 0;
int pressureIn[SENSOR_COUNT];
bool areAllPressed = false;
bool isPadPressed[SENSOR_COUNT];

int passCount = 0;
bool isFirstPass = true;
double batVoltage;
int batPin = A6;

Adafruit_NeoPixel pixel(PIXEL_COUNT, SPI1, WS2812);

//functions
void ledStripStartup();
void ledFill(int color, int firstLED = 0, int lastLED = PIXEL_COUNT);
uint32_t wheel(byte wheelPos);
void rainbow(uint8_t wait);

void setup() {
    EEPROM.get(BASELINE_ADDRESS, pressureBaselines);
    pinMode(batPin, INPUT);
    ledStripStartup();

    Particle.variable("BatteryVoltage", batVoltage);

    Serial.printf("#### Incoming Average Baselines ####\n");
    for(int i=0; i<SENSOR_COUNT; i++){
        pinMode(PRESSURE_PINS[i], INPUT);
        Serial.printf("Last Avg #%i: %i\n",i , pressureBaselines[i]);
    }
    Serial.printf("\n\n");
}

void loop() {
    batVoltage = analogRead(batPin)/819.2;
    for(int i=0; i<SENSOR_COUNT; i++){
        pressureIn[i] = analogRead(PRESSURE_PINS[i]);
    }

    areAllPressed = true;
    for(int j=0; j<SENSOR_COUNT; j++){
        if(pressureIn[j] - pressureBaselines[j] > gripStrength){
            isPadPressed[j] = true;
        } else{
            isPadPressed[j] = false;
            areAllPressed = false;
        }
    }

    if(areAllPressed){
        if(millis() - pixelPrevious >= pixelDelay){
            pixelPrevious = millis();
            rainbow(1);
        }
    } else{
        isFirstPass = true;
        for(int k=0; k<SENSOR_COUNT; k++){
            if((isPadPressed[k])){
                ledFill(0x00FF00, k*PIXELS_PER_SEGMENT, (k+1)*PIXELS_PER_SEGMENT);
            } else{
                ledFill(0, k*PIXELS_PER_SEGMENT, (k+1)*PIXELS_PER_SEGMENT);
            }

        }
    }
    
    if(isFirstPass && areAllPressed){
        passCount++;
        isFirstPass = false;
        Serial.printf("Pass Count: %i\n", passCount);
        Particle.publish("Pass Count", String(passCount));
    }


    pixel.show();
}

void ledFill(int color, int firstLED, int lastLED) {
    for (int i=firstLED; i<lastLED; i++){
        pixel.setPixelColor(i, color);
    }
}

void ledStripStartup(){
    pixel.begin();
    pixel.setBrightness(brightness);
    pixel.clear();
    pixel.show();

    //Quick test of all LEDs to make sure the strip works. 
    for (int i=0; i < PIXEL_COUNT; i++){
        pixel.clear();
        pixel.setPixelColor(i, 0xFFFFFF);
        pixel.show();
        delay(20);
    }

    delay(1000);

    pixel.clear();
    pixel.show();
}

void rainbow(uint8_t wait){
    if(pixelDelay != wait){
        pixelDelay = wait;
    }

    for(uint16_t i=0; i<PIXEL_COUNT; i++){
        pixel.setPixelColor(i, wheel((i+pixelCycle) &255));
    }
    pixel.show();
    pixelCycle++;
    if(pixelCycle >= 256){
        pixelCycle =0;
    }
}

//No i don't understand exactly what this does
//input a value 0-255 to get a color value.
uint32_t wheel(byte wheelPos){
    wheelPos = 255 - wheelPos;
    if(wheelPos < 85){
        return pixel.Color(255-wheelPos*3, 0, wheelPos * 3);
    }

    if(wheelPos < 170){
        wheelPos -= 85;
        return pixel.Color(0, wheelPos*3, 255-wheelPos*3);
    }
    wheelPos -=170;
    return pixel.Color(wheelPos*3, 255-wheelPos*3, 0);
}