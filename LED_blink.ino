/****************************** Feeds ***************************************/ 
// Setup a feed called 'esp8266_led' for subscribing to changes. 
Adafruit_MQTT_Subscribe esp8266_led = Adafruit_MQTT_Subscribe(&mqtt, MQTT_USERNAME "/led/esp8266"); 
/*************************** Sketch Code ************************************/

#define LED_PIN         15
#define LED_blue        12
uint32_t                period = 5000;

void setup() {
 pinMode(LED_PIN, OUTPUT); 
 digitalWrite(LED_PIN, LOW);
}

void loop() {
 // Ensure the connection to the MQTT server is alive (this will make the first 
 // connection and automatically reconnect when disconnected).  See the MQTT_connect 
 MQTT_connect(); 
 // this is our 'wait for incoming subscription' busy subloop 
 // Here it read the subscription 
 Adafruit_MQTT_Subscribe *subscription; 
 while ((subscription = mqtt.readSubscription())) { 
   if (subscription == &esp8266_led) { 
     char *message = (char *)esp8266_led.lastread; 
     Serial.print(F("Got: ")); 
     Serial.println(message); 
        // Checking for message 
     if (strncmp(message, "ON", 2) == 0) { 
       // Turn the LED on.
       for( uint32_t tStart = millis(); (millis()-tStart) < period; ){ 
       heartBeat(1.2);
       }
     } 
   } 
 } 
}
