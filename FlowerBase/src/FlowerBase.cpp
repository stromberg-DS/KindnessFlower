/* 
 * Flower Base - total passcounts and report to Adafruit
 * Author: Daniel Stromberg
 * Date: 6/7/24
 */

#include "Particle.h"
#include <Adafruit_MQTT.h>
#include "Adafruit_MQTT/Adafruit_MQTT_SPARK.h"
#include "Adafruit_MQTT/Adafruit_MQTT.h"
#include "credentials.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

//constants
const int OLED_RESET = -1;
const int OLED_ADDRESS = 0x3C;


//Variables
int flower1Count;
int flower2Count;
int flower3Count;
int flower4Count;
int flower5Count;
int flower6Count;
int totalPassCount = 0;
int lastPubTime = 0;
int lastPrintTime = 0;

void MQTT_connect();
bool MQTT_ping();
void adaPublish();
void updateDisplay(int count);

TCPClient TheClient;
Adafruit_MQTT_SPARK mqtt(&TheClient, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Subscribe flower1Sub = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/flower1passcount");
Adafruit_MQTT_Subscribe flower2Sub = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/flower2passcount");
Adafruit_MQTT_Subscribe flower3Sub = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/flower3passcount");
Adafruit_MQTT_Subscribe flower4Sub = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/flower4passcount");
Adafruit_MQTT_Subscribe flower5Sub = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/flower5passcount");
Adafruit_MQTT_Subscribe flower6Sub = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/flower6passcount");
Adafruit_MQTT_Publish totalPassPub = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/totalflowerpass");


// Adafruit_SSD1306 display(OLED_RESET);

String DateTime, TimeOnly, Day, MonthDate, Year;

void setup() {
    Serial.begin(9600);

    mqtt.subscribe(&flower1Sub);
    mqtt.subscribe(&flower2Sub);
    mqtt.subscribe(&flower3Sub);
    mqtt.subscribe(&flower4Sub);
    mqtt.subscribe(&flower5Sub);
    mqtt.subscribe(&flower6Sub);
    Time.zone(-7);
    Particle.syncTime();

    // display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS);
    // display.clearDisplay();
    // display.setTextColor(WHITE);
    // display.setTextSize(2);
    // display.setCursor(0,0);
    // display.printf("HI");
    // display.display();
}

void loop() {
    MQTT_connect();
    MQTT_ping();

    Adafruit_MQTT_Subscribe *subscription;
    while((subscription = mqtt.readSubscription(100))){    //at the moment, flowers publish every 30 sec.
        if(subscription == &flower1Sub){
            flower1Count = strtol((char *)flower1Sub.lastread, NULL, 10);
            Serial.printf("Incoming Flower1 Count: %i\n", flower1Count);
            totalPassCount = flower1Count + flower2Count + flower3Count + flower4Count + flower5Count + flower6Count;
            adaPublish();
            // updateDisplay(totalPassCount);
        }else if(subscription == &flower2Sub){
            flower2Count = strtol((char *)flower2Sub.lastread, NULL, 10);
            Serial.printf("Incoming Flower2 Count: %i\n", flower2Count);
            totalPassCount = flower1Count + flower2Count + flower3Count + flower4Count + flower5Count + flower6Count;
            adaPublish();
            // updateDisplay(totalPassCount);
        }else if(subscription == &flower3Sub){
            flower3Count = strtol((char *)flower3Sub.lastread, NULL, 10);
            Serial.printf("Incoming Flower3 Count: %i\n", flower3Count);
            totalPassCount = flower1Count + flower2Count + flower3Count + flower4Count + flower5Count + flower6Count;
            adaPublish();
            // updateDisplay(totalPassCount);
        }else if(subscription == &flower4Sub){
            flower4Count = strtol((char *)flower4Sub.lastread, NULL, 10);
            Serial.printf("Incoming Flower2 Count: %i\n", flower4Count);
            totalPassCount = flower1Count + flower2Count + flower3Count + flower4Count + flower5Count + flower6Count;
            adaPublish();
            // updateDisplay(totalPassCount);
        }else if(subscription == &flower5Sub){
            flower5Count = strtol((char *)flower1Sub.lastread, NULL, 10);
            Serial.printf("Incoming Flower1 Count: %i\n", flower1Count);
            totalPassCount = flower1Count + flower2Count + flower3Count + flower4Count + flower5Count + flower6Count;
            adaPublish();
            // updateDisplay(totalPassCount);
        }else if(subscription == &flower6Sub){
            flower6Count = strtol((char *)flower6Sub.lastread, NULL, 10);
            Serial.printf("Incoming Flower6 Count: %i\n", flower6Count);
            totalPassCount = flower1Count + flower2Count + flower3Count + flower4Count + flower5Count + flower6Count;
            adaPublish();
            // updateDisplay(totalPassCount);
        }
    }

    if(millis() - lastPrintTime > 1000){
        Serial.printf("Total Passes: %i\n", totalPassCount);
        lastPrintTime=millis();
    }

    if(millis() - lastPubTime>20000){
        if(mqtt.Update()){
            totalPassPub.publish(totalPassCount);
            lastPubTime = millis();
        }
    }
}

//Publish to Adafruit.io (publish infrequently)
void adaPublish(){
    // if((millis()-lastPubTime)>30000){     //only publish every 30 seconds max
        if(mqtt.Update()){
            totalPassPub.publish(totalPassCount);
            // lastPubTime = millis();
        }
    // }
}

// void updateDisplay(int count){
//     display.clearDisplay();
//     display.setCursor(0,0);
//     display.setTextSize(1);
//     display.printf("Total Passes:\n\n");
//     display.setTextSize(6);
//     display.printf(" %i", count);
//     display.display();
// }

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care of connecting.
void MQTT_connect(){
    int8_t ret;

    // Return if already connected.
    if (mqtt.connected()){
        return;
    }

    Serial.print("Connecting to MQTT... ");

    while((ret = mqtt.connect()) != 0){
        Serial.printf("Error Code %s\n", mqtt.connectErrorString(ret));
        Serial.printf("Retrying MQTT connection in 5 seconds...\n");
        mqtt.disconnect();
        delay(5000);
    }
    Serial.printf("MQTT Connected!\n");
}

//Keeps the connection open to Adafruit
bool MQTT_ping() {
    static unsigned int last;
    bool pingStatus;

    if ((millis()-last)>120000) {
        Serial.printf("Pinging MQTT \n");
        pingStatus = mqtt.ping();
        if(!pingStatus) {
        Serial.printf("Disconnecting \n");
        mqtt.disconnect();
        }
        last = millis();
    }
    return pingStatus;
}