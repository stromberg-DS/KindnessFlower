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

const int SENSOR_COUNT = 6;
const int PIXEL_COUNT = 144;
const int PIXEL_ZONES[SENSOR_COUNT] = {24, 48, 72, 96, 120,144};
const int PRESSURE_PINS[SENSOR_COUNT] = {A0, A1, A2, A5, A4, A3};
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

    for(int i=0; i<6; i++){
        pinMode(PRESSURE_PINS[i], INPUT);
        Serial.printf("Last Avg #%i: %i\n",i , pressureBaselines[i]);
    }
    Serial.printf("\n\n");
}

void loop() {
    for(int i=0; i<6; i++){
        pressureIn[i] = analogRead(PRESSURE_PINS[i]);
    }

    for(int j=0; j<6; j++){
        // Serial.printf("Pressure #%i: %i\n", j+1, pressureIn[j]);

        if(pressureIn[j] - pressureBaselines[j] > gripStrength){
            // Serial.printf("pressureIn: %i\nBaseline: %i\n\n", pressureIn[j], pressureBaselines[j]);
            ledFill(0x00FF00, j*24, (j+1)*24);
            // ledFill(0x00FF00);
        } else{
            ledFill(0, j*24, (j+1)*24);
            // ledFill(0);
        }
    }
    // Serial.printf("\n");    
    delay(5);

    

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
        pixel.setPixelColor(i, 0xFFFFFF);
        pixel.show();
        delay(20);
    }

    delay(1000);

    pixel.clear();
    pixel.show();
}