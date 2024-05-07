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

const int PIXEL_COUNT = 100;

int brightness = 20;
int pressureIn[6];

Adafruit_NeoPixel pixel(PIXEL_COUNT, SPI1, WS2812);

//functions
void ledStripStartup();

void setup() {

    ledStripStartup();

}

void loop() {

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