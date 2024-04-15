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
bool isLedOn[12];
int ledPins[6] = {3,4,5,6,7,10};

// setup() runs once, when the device is first turned on
void setup() {
  Serial.begin(9600);
  cap.begin(0x5A);

  for (int i=0; i<6; i++){
    pinMode(ledPins[i], OUTPUT);
  }
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  currTouched = cap.touched();
  // Serial.printf("currtouched: %i\n", currTouched);

  //Below does a bitwise and between the binary version of currTouched number
  //  and 0-2048 in binary (starts 1 at 1's place, shifts left each loop).
  //  when 'and'ed together we only get true when i is at the location of
  //  a 1 in currTouched. 
  //  33 dec =      000000100001    currTouched (in binary)
  //  when i is 0:  000000000001    checking 1's place
  //  'and'ed =     000000000001    there is a 1, 0 pin touched
  //
  // .... Increment i (move 1 to left to check other places) ....
  //  
  //  33 dec =      000000100001    currTouched (in binary)
  //  i=5           000000100000    checking 32's place
  //  'and'ed       000000100000    There is a 1, pin 5 touched!
  //
  for (int i=0; i<12; i++){
    if((currTouched & _BV(i)) && !(lastTouched & _BV(i))){
      Serial.printf("%i Touched\n", i);
      Serial.printf("CurrTouched: %i\n", currTouched);
      Serial.printf("_BV(i) = %i\n", _BV(i));
      Serial.printf("(currTouched & _BV(i)) = %i\n\n", currTouched & _BV(i));
    }

    if((currTouched & _BV(i) && (lastTouched & _BV(i)))){
      // Serial.printf("%i Released\n");
      isLedOn[i] = true;
    } else{
      isLedOn[i] = false;
    }
  }

  for (int i=0; i<6; i++){
    digitalWrite(ledPins[i], isLedOn[i]);
  }

  lastTouched = currTouched;
}
