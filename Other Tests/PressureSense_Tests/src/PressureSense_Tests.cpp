/* 
 * Pressure Sensor Test - uses Force-Sensitive Resitor from Adafruit (ID:1075)
 * Author: Daniel Stromberg
 * Date: 5/7/24
 * Use the Adafruit guide to set up FSRs
 *  https://learn.adafruit.com/force-sensitive-resistor-fsr/using-an-fsr
 */

#include "Particle.h"
#include <neopixel.h>

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

const int PIXEL_COUNT = 144;
const int PIXEL_ZONES[6] = {24, 48, 72, 96, 120,144};
const int PRESSURE_PINS[4] = {A0, A1, A2, A5};

int brightness = 150;
int pressureIn[6];

Adafruit_NeoPixel pixel(PIXEL_COUNT, SPI1, WS2812);

//functions
void ledStripStartup();
void ledFill(int color, int firstLED = 0, int lastLED = PIXEL_COUNT);

void setup() {

    ledStripStartup();
    pinMode(PRESSURE_PINS[0], INPUT);
}

void loop() {
    pressureIn[0] = analogRead(PRESSURE_PINS[0]);

    Serial.printf("Pressure: %i\n", pressureIn[0]);
    delay(100);

    if(pressureIn[0] > 3300){
        ledFill(0x00FF00, PIXEL_ZONES[0], PIXEL_ZONES[1]);
    } else{
        ledFill(0);
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
        pixel.setPixelColor(i, 0xFFFFFF);
        pixel.show();
        delay(20);
    }

    delay(1000);

    pixel.clear();
    pixel.show();
}