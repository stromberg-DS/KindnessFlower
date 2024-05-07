/* 
 * OLED & Clear Conductive Sheet Capacitive test
 * Author: Daniel Stromberg
 * Date: 
*/

#include "Particle.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MPR121.h>


const int OLED_RESET = -1;

Adafruit_SSD1306 lrgDisplay(OLED_RESET);
Adafruit_SSD1306 smlDisplay(OLED_RESET);

Adafruit_MPR121 capSensor = Adafruit_MPR121();

int lastTouched = 0;
int currTouched = 0;


SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

void setup() {
    Serial.begin(9600);
    capSensor.begin(0x5A);

    lrgDisplay.begin(SSD1306_SWITCHCAPVCC, 0x3D);
    lrgDisplay.setRotation(2);
    smlDisplay.begin();

    lrgDisplay.clearDisplay();


    lrgDisplay.setTextColor(WHITE);
    smlDisplay.setTextColor(WHITE);
    lrgDisplay.setTextSize(2);
    smlDisplay.setTextSize(2);


    lrgDisplay.setCursor(0,1);
    smlDisplay.setCursor(0,1);

    lrgDisplay.printf("Big Boi!");

    lrgDisplay.display();

    smlDisplay.clearDisplay();
    smlDisplay.printf("Little guy");
    smlDisplay.display();
}

void loop() {
    currTouched = capSensor.touched();
    // Serial.printf("Current: %i\n", currTouched);
    lrgDisplay.clearDisplay();
    lrgDisplay.setCursor(0,0); 
    
    lrgDisplay.printf("Touch Pins\n");
    for(int i=0; i<12; i++){
        if((currTouched & (0x01<<i)) && !(lastTouched & (0x01 << i))){
            lrgDisplay.printf("%i-", i);
        }
    }
    // lrgDisplay.printf("\n");
    lrgDisplay.display();
    
}
