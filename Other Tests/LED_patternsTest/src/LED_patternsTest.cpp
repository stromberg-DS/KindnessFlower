/* 
 * LED Tests - trying out patterns
 * Author: Daniel Stromberg
 * Date: 5/9/24
*/

#include "Particle.h"
#include <neopixel.h>

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

const int PIXEL_COUNT = 144;

int brightness = 30;
int pixelDelay = 50;
int pixelCycle = 0;
unsigned long pixelPrevious = 0;

Adafruit_NeoPixel pixel(PIXEL_COUNT, SPI1, WS2812);

uint32_t wheel(byte wheelPos);
void rainbow(uint8_t wait);

void setup() {
    Serial.begin(9600);

    pixel.begin();
    pixel.setBrightness(brightness);
    pixel.clear();
    pixel.show();

    for (int i=0; i < PIXEL_COUNT; i++){
        pixel.clear();
        pixel.setPixelColor(i, 0xFFFFFF);   //Light up white
        pixel.show();
        delay(20);
    }
}

void loop() {
    if(millis() - pixelPrevious >= pixelDelay){
        pixelPrevious = millis();
        rainbow(2);
    }
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