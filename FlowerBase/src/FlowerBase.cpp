/* 
 * Flower Base - total passcounts and report to Adafruit
 * Author: Daniel
 * Date: 6/7/24
 */

#include "Particle.h"
#include <Adafruit_MQTT.h>
#include "Adafruit_MQTT/Adafruit_MQTT_SPARK.h"
#include "Adafruit_MQTT/Adafruit_MQTT.h"
#include "credentials.h"

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

//Variables
int flower1Count;
int flower2Count;
int flower3Count;
int flower4Count;
int flower5Count;
int flower6Count;
int totalPassCount = 14;
int lastPubTime = 0;

void MQTT_connect();
bool MQTT_ping();
void adaPublish();

TCPClient TheClient;
Adafruit_MQTT_SPARK mqtt(&TheClient, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Subscribe flower4Sub = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/flower4passcount");
Adafruit_MQTT_Publish totalPassPub = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/totalflowerpass");


void setup() {
    Serial.begin(9600);

    mqtt.subscribe(&flower4Sub);
}

void loop() {
    MQTT_connect();
    MQTT_ping();

    Adafruit_MQTT_Subscribe *subscription;
    while((subscription = mqtt.readSubscription(100))){    //at the moment, flowers publish every 30 sec.
        if(subscription == &flower4Sub){
            flower4Count = strtol((char *)flower4Sub.lastread, NULL, 10);
            Serial.printf("Incoming Flower2 Count: %i\n", flower4Count);
            adaPublish();
        }
    }
}

//Publish to Adafruit.io (publish infrequently)
void adaPublish(){
    if((millis()-lastPubTime)>30000){     //only publish every 30 seconds max
        if(mqtt.Update()){
            totalPassPub.publish(totalPassCount);
            lastPubTime = millis();
        }
    }
}

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