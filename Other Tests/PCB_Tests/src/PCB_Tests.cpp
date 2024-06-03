/* 
 * Project myProject
 * Author: Your Name
 * Date: 
\*/

#include "Particle.h"

SYSTEM_MODE(MANUAL);
SYSTEM_THREAD(ENABLED);

const int SENSOR_PINS[4] = {A5, A0, A2, A1};

void setup() {
    Serial.begin(9600);
    for (int i=0; i<4; i++){
        pinMode(SENSOR_PINS[i], INPUT);
    }

}

void loop() {
    int fsrRead[4];
    for (int i=0; i<4; i++){
        fsrRead[i] = analogRead(SENSOR_PINS[i]);
        Serial.printf("Sensor %i Value: %i\n", i, fsrRead[i]);
    }
    Serial.printf("\n");
    delay(250);
}
