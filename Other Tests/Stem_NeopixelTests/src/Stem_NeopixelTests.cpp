/* 
 * Kindness Flower Stem LED tests
 * Author: Daniel Stromberg
 * Date: 4/22/24
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include <neopixel.h>   
#include "Adafruit_MPR121.h"


SYSTEM_MODE(AUTOMATIC);   
SYSTEM_THREAD(ENABLED);

//Constants
const int LED_PIN = 2;
const int PIXEL_COUNT = 144;
const int touchPadCount = 8;
const int touchPins[touchPadCount] = {1,2,4,5,6,8,9,10};
//touch pins used:
//  1,2,4,5,6,8,9,10

//Variables
int brightness = 30;
int lastTouched = 0;
int currTouched = 0;
int newBase;
int newBaseCounter = 0;
int ledPerPad = PIXEL_COUNT/touchPadCount;
    

Adafruit_NeoPixel pixel(PIXEL_COUNT, SPI1, WS2812B);  
Adafruit_MPR121 capTouch = Adafruit_MPR121();

void pixelFill(int fillColor, int startPixel=0, int endPixel=PIXEL_COUNT);

void setup() {
  Serial.begin(9600);

  capTouch.begin(0x5A);
  
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

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  int randomPixelNumber;
  int randomPixelColor;


  currTouched = capTouch.touched();

  if(newBaseCounter < 10){
    Serial.printf("Counter: %i\n\n", newBaseCounter);
    newBase = capTouch.filteredData(2)-10;
    newBaseCounter++; //add one to counter
  }

  int touchCount = 0;
  for(int i=0; i<touchPadCount; i++){
    if(newBase - capTouch.filteredData(touchPins[i]) > 12){
      touchCount++;
    } 
  }
  pixel.clear();
  pixelFill(0x00FF00, 0, touchCount*ledPerPad);

  // if(newBase - capTouch.filteredData(2) > 12){
  //   digitalWrite(7, HIGH);
  //   pixelFill(0xFF0000);
  // } else{
  //   digitalWrite(7, LOW);
  //   pixelFill(0x00FF00);
  // }

  pixel.show();    //actually update the LED strip by showing the new color
}

void pixelFill(int fillColor, int startPixel, int endPixel){
  for(int i=startPixel; i<endPixel; i++){
    pixel.setPixelColor(i, fillColor);
  }
  pixel.show();
}