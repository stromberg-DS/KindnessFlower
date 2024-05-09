/* 
 * LED Tests - trying out patterns
 * Author: Daniel Stromberg
 * Date: 5/9/24
*/

#include "Particle.h"
#include <neopixel.h>

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

const int SENSOR_COUNT =4;
const int PIXEL_COUNT = 144;
const int TOUCH_PINS[] = {4, 5, 6, 7};
const int PIXELs_PER_SEGMENT = PIXEL_COUNT/SENSOR_COUNT;

int brightness = 30;
int pixelDelay = 50;
int pixelCycle = 0;
unsigned long pixelPrevious = 0;

Adafruit_NeoPixel pixel(PIXEL_COUNT, SPI1, WS2812);

uint32_t wheel(byte wheelPos);
void rainbow(uint8_t wait);
void ledFill(int color, int firstLED=0, int lastLED=PIXEL_COUNT);

void setup() {
    Serial.begin(9600);

    pixel.begin();
    pixel.setBrightness(brightness);
    pixel.clear();
    pixel.show();

    for(int i=0; i<4; i++){
        pinMode(TOUCH_PINS[i], INPUT_PULLDOWN);
    }

    for (int i=0; i < PIXEL_COUNT; i++){
        pixel.clear();
        pixel.setPixelColor(i, 0xFFFFFF);   //Light up white
        pixel.show();
        delay(20);
    }
}

void loop() {
    static int lastPrint;
    static bool isPressed[4];

    if(millis() - lastPrint > 500){
        Serial.printf("Pressed Buttons: ");
        for(int i=0; i <4; i++){
            isPressed[i] = digitalRead(TOUCH_PINS[i]);
            Serial.printf("%i,", isPressed[i]);
        }
        Serial.printf("\n\n");
        lastPrint = millis();
    }

    
    if((isPressed[0]) && (isPressed[1]) && (isPressed[2]) && (isPressed[3])){
        if(millis() - pixelPrevious >= pixelDelay){
            pixelPrevious = millis();
            rainbow(10);
        }
    } else{
        for(int j=0; j<4; j++){
            if((isPressed[j])){
                ledFill(0x00FF00, j*PIXELs_PER_SEGMENT, (j+1)*PIXELs_PER_SEGMENT);
            } else{
                ledFill(0, j*PIXELs_PER_SEGMENT, (j+1)*PIXELs_PER_SEGMENT);
            }

        }
        pixel.show();
    }
}

void ledFill(int color, int firstLED, int lastLED) {
    for (int i=firstLED; i<lastLED; i++){
        pixel.setPixelColor(i, color);
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