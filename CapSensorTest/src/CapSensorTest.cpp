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
SYSTEM_MODE(AUTOMATIC);

// Run the application and system concurrently in separate threads
SYSTEM_THREAD(ENABLED);

Adafruit_MPR121 cap = Adafruit_MPR121();

int lastTouched = 0;
int currTouched = 0;
bool isLedOn[12];
int ledPins[6] = {3,4,5,6,7,10};    //Pins to attach LEDs to
int lastPubTime = 0;
int NewBase;
int NewBaseCounter = 0;
int touch = 150;
int release = 150;


// setup() runs once, when the device is first turned on
void setup() {
  Serial.begin(9600);
  cap.begin(0x5A);

  for (int i=0; i<6; i++){
    pinMode(ledPins[i], OUTPUT);

  }
  cap.setThreshholds(touch, release);
  
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  currTouched = cap.touched();
  if(NewBaseCounter < 10){
    Serial.printf("Counter: %i\n\n", NewBaseCounter);
    NewBase = cap.filteredData(2) - 10;
    NewBaseCounter = NewBaseCounter + 1;
  }
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
      // Serial.printf("%i Touched\n", i);
      // Serial.printf("CurrTouched: %i\n", currTouched);
      // Serial.printf("_BV(i) = %i\n", _BV(i));
      // Serial.printf("(currTouched & _BV(i)) = %i\n\n", currTouched & _BV(i));
    }

    if((currTouched & _BV(i) && (lastTouched & _BV(i)))){
      // Serial.printf("%i Released\n");
      isLedOn[i] = true;
    } else{
      isLedOn[i] = false;
    }
  

  for (int i=0; i<6; i++){
    digitalWrite(ledPins[i], isLedOn[i]);
  }
  
  if(millis()-lastPubTime > 1000){
    Serial.printf("Currently touched\n0:%i\n1:%i\n2:%i\n3:%i\n4:%i\n5:%i\n\n", isLedOn[0],isLedOn[1],isLedOn[2],isLedOn[3],isLedOn[4],isLedOn[5]);
    
    lastPubTime = millis();
  } 
  // for (int i=0; i<6; i++){
  //   digitalWrite(ledPins[i], isLedOn[i]);
  // }

  lastTouched = currTouched;
  if(NewBase - cap.filteredData(2) > 12){
    for (int i=0; i<6; i++){
      digitalWrite(ledPins[i], HIGH);
    }
    Serial.printf("Touched\n");
  }else {
    for (int j=0; j<6; j++){
      digitalWrite(ledPins[i], LOW);
    }
  }


Serial.printf("NewBase %i \n", NewBase);
// Serial.printf("Filtered: %i \n", cap.filteredData(2));
// Serial.printf("Difference: %i \n\n", (NewBase - cap.filteredData(2)));

 // comment out this line for detailed data from the sensor!
  //return;
  
  // debugging info, what
  // Serial.print("\t\t\t\t\t\t\t\t\t\t\t\t\t 0x"); Serial.println(cap.touched(), HEX);
  // Serial.print("Filt: ");
  // for (uint8_t i=0; i<12; i++) {
  //   Serial.print(cap.filteredData(i)); Serial.print("\t");
  // }
  // Serial.println();
  // Serial.print("Base: ");
  // for (uint8_t i=0; i<12; i++) {
  //   Serial.print(cap.baselineData(i)); Serial.print("\t");
  // }
  // Serial.println();
  // delay(500);

}}
