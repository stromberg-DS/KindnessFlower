/* 
 * Project myProject
 * Author: Your Name
 * Date: 
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);
 
const int batPin = A6;
double batVoltage;
bool isCharging;



// setup() runs once, when the device is first turned on
void setup() {
  Serial.begin(9600);
  Particle.variable("BatteryVolage", batVoltage);
  Particle.variable("isCharging", isCharging);

  pinMode(batPin, INPUT);
  pinMode(CHG, INPUT_PULLUP); //tells us if we are charging. Might not work well
                              //  according to Particle.
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  batVoltage = analogRead(batPin)/819.2;
  isCharging = digitalRead(CHG);
}
