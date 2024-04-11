/* 
 * Project myProject
 * Author: Your Name
 * Date: 
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include "Adafruit_MPR121.h"

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(SEMI_AUTOMATIC);

// Run the application and system concurrently in separate threads
SYSTEM_THREAD(ENABLED);

// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'
SerialLogHandler logHandler(LOG_LEVEL_INFO);

Adafruit_MPR121 cap = Adafruit_MPR121();

int lastTouched = 0;
int currTouched = 0;

// setup() runs once, when the device is first turned on
void setup() {
  Serial.begin(9600);
  cap.begin(0x5A);
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  currTouched = cap.touched();
  // Serial.printf("currtouched: %i\n", currTouched);


  for (int i=0; i<12; i++){
    if((currTouched & _BV(i)) && !(lastTouched & _BV(i))){
      Serial.printf("%i Touched\n", i);
      Serial.printf("CurrTouched: %i\n", currTouched);
      Serial.printf("_BV(i) = %i\n", _BV(i));
      Serial.printf("(currTouched & _BV(i)) = %i\n\n", currTouched & _BV(i));
    }

    // if((currTouched & _BV(i) && (lastTouched & _BV(i)))){
    //   Serial.printf("%i Released\n");
    // }
  }

  lastTouched = currTouched;
}
