/* 
 * Project myProject
 * Author: Your Name
 * Date: 
\*/

#include "Particle.h"

SYSTEM_MODE(MANUAL);
SYSTEM_THREAD(ENABLED);

const int SENSOR_PIN = A0;

void setup() {
    Serial.begin(9600);
    pinMode(SENSOR_PIN, INPUT);
}

void loop() {
    int fsrRead;
    fsrRead = analogRead(SENSOR_PIN);
    Serial.printf("Sensor Value: %i\n\n", fsrRead);
    
    delay(250);
}
