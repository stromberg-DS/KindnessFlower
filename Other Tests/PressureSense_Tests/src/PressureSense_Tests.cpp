/* 
 * Pressure Sensor Test - uses Force-Sensitive Resitor from Adafruit (ID:1075)
 * Author: Daniel Stromberg
 * Date: 5/7/24
 * Use the Adafruit guide to set up FSRs
 *  https://learn.adafruit.com/force-sensitive-resistor-fsr/using-an-fsr
 */

#include "Particle.h"
#include <neopixel.h>

////////KEEEP AUTOMATIC///////////
SYSTEM_MODE(AUTOMATIC);
/////////////////////////////////
SYSTEM_THREAD(ENABLED);

const int SENSOR_COUNT = 4;
const int PIXEL_COUNT = 144;
const int PIXEL_ZONES[SENSOR_COUNT] = {36, 72, 108, 144};
const int PRESSURE_PINS[SENSOR_COUNT] = {A5, A0, A2, A1};
const int gripStrength = 200;     //how hard to squeeze to trigger LEDs. Higher=harder squeeze
int pressureBaselines[SENSOR_COUNT];

//EEPROM Setup 
int len = EEPROM.length();
int baselineAddress = 0x0001;


int brightness = 150;
int pressureIn[SENSOR_COUNT];

Adafruit_NeoPixel pixel(PIXEL_COUNT, SPI1, WS2812);

//functions
void ledStripStartup();
void ledFill(int color, int firstLED = 0, int lastLED = PIXEL_COUNT);

void setup() {
    EEPROM.get(baselineAddress, pressureBaselines);


    ledStripStartup();

    Serial.printf("#### Incoming Average Baselines ####");

    for(int i=0; i<SENSOR_COUNT; i++){
        pinMode(PRESSURE_PINS[i], INPUT);
        Serial.printf("Last Avg #%i: %i\n",i , pressureBaselines[i]);
    }
    Serial.printf("\n\n");
}

void loop() {
    for(int i=0; i<SENSOR_COUNT; i++){
        pressureIn[i] = analogRead(PRESSURE_PINS[i]);
    }

    for(int j=0; j<SENSOR_COUNT; j++){

        if(pressureIn[j] - pressureBaselines[j] > gripStrength){
            ledFill(0x00FF00, PIXEL_ZONES[j], PIXEL_ZONES[j+1]);
            Serial.printf("Pressure #%i: \n", j);
            Serial.printf("pressureIn: %i\nBaseline: %i\n\n", pressureIn[j], pressureBaselines[j]);
            ledFill(0xFF0000, j, j+1);
            // ledFill(0x00FF00);
        } else{
            ledFill(0, PIXEL_ZONES[j], PIXEL_ZONES[j+1]);
            ledFill(0, j, j+1);
            // ledFill(0);
        }
    }
    // Serial.printf("\n");    
    delay(250);

    

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