/*************************************************** 
  NodeMCU
****************************************************/ 
#include <ESP8266WiFi.h> 
#include "Adafruit_MQTT.h" 
#include "Adafruit_MQTT_Client.h" 
/************************* WiFi Access Point *********************************/ 
#define WLAN_SSID       "OsloMet MakerSpace" 
#define WLAN_PASS       "o5lom3et2018" 
#define MQTT_SERVER      "10.253.17.254" // static ip address
#define MQTT_PORT         1883                    
//#define MQTT_USERNAME    "" 
//#define MQTT_PASSWORD         "" 
#define KNAPP1_PIN     14                
#define KNAPP2_PIN     13
/************ Global State ******************/ 
// Create an ESP8266 WiFiClient class to connect to the MQTT server. 
WiFiClient client; 
// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details. 
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_PORT, MQTT_USERNAME, MQTT_PASSWORD); 
/****************************** Feeds ***************************************/ 
// Setup a feed called 'knapper' for publishing. 
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname> 
Adafruit_MQTT_Publish pi_knapp1 = Adafruit_MQTT_Publish(&mqtt, MQTT_USERNAME "/knapper/pi"); 
Adafruit_MQTT_Publish pi_knapp2 = Adafruit_MQTT_Publish(&mqtt, MQTT_USERNAME "/knapper/pi");
 
// Setup a feed called 'esp8266_led' for subscribing to changes. 
Adafruit_MQTT_Subscribe esp8266_led = Adafruit_MQTT_Subscribe(&mqtt, MQTT_USERNAME "/leds/esp8266"); 
/*************************** Sketch Code ************************************/ 

void MQTT_connect(); 
void setup() { 
 Serial.begin(115200); 
 delay(10); 
 pinMode(LED_PIN, OUTPUT); 
 digitalWrite(LED_PIN, LOW); 
 // Setup button as an input with internal pull-up. 
pinMode(KNAPP1_PIN, INPUT_PULLUP);
pinMode(KNAPP2_PIN, INPUT_PULLUP); 
 Serial.println(F("RPi-ESP-MQTT")); 
 // Connect to WiFi access point. 
 Serial.println(); Serial.println(); 
 Serial.print("Connecting to "); 
 Serial.println(WLAN_SSID); 
 WiFi.begin(WLAN_SSID, WLAN_PASS); 
 while (WiFi.status() != WL_CONNECTED) { 
   delay(500); 
   Serial.print("."); 
 } 
 Serial.println(); 
 Serial.println("WiFi connected"); 
 Serial.println("IP address: "); Serial.println(WiFi.localIP());
  
 // Setup MQTT subscription for esp8266_led feed. 
 mqtt.subscribe(&esp8266_led); 
} 

uint32_t x=0;
 
void loop() { 
  // Check if the button has been pressed by looking for a change from high to 
 // low signal (with a small delay to debounce). 
 int knapp1_first = digitalRead(KNAPP1_PIN); 
 int knapp2_first = digitalRead(KNAPP2_PIN); 
 // Ensure the connection to the MQTT server is alive (this will make the first 
 // connection and automatically reconnect when disconnected).  See the MQTT_connect 
 MQTT_connect(); 
 // this is our 'wait for incoming subscription packets' busy subloop 
 // try to spend your time here 
 // Here its read the subscription 
 Adafruit_MQTT_Subscribe *subscription; 
 while ((subscription = mqtt.readSubscription())) { 
   if (subscription == &esp8266_led) { 
     char *message = (char *)esp8266_led.lastread; 
     Serial.print(F("Got: ")); 
     Serial.println(message); 
        // Check if the message was ON, OFF, or TOGGLE. 
     if (strncmp(message, "ON", 2) == 0) { 
       // Turn the LED on. 
       digitalWrite(LED_PIN, HIGH); 
     } 
     else if (strncmp(message, "OFF", 3) == 0) { 
       // Turn the LED off. 
       digitalWrite(LED_PIN, LOW); 
     } 
     else if (strncmp(message, "TOGGLE", 6) == 0) { 
       // Toggle the LED. 
       digitalWrite(LED_PIN, !digitalRead(LED_PIN)); 
} 
   } 
 } 
 
 delay(20); 
 int knapp1_second = digitalRead(KNAPP1_PIN); 
 int knapp2_second = digitalRead(KNAPP2_PIN); 
 if ((knapp1_first == HIGH) && (knapp1_second == LOW)) { 
   // Knapp 1 ble trykt! 
   Serial.println("Knapp 1 trykt!"); 
   pi_led.publish("Knapp 1"); 
  } 
 if ((knapp2_first == HIGH) && (knapp2_second == LOW)) { 
   // Knapp 2 ble trykt! 
   Serial.println("Knapp 2 trykt!"); 
   pi_led.publish("Knapp 2"); 
  } 
} 


// Function to connect and reconnect as necessary to the MQTT server. 
void MQTT_connect() { 
 int8_t ret; 
 // Stop if already connected. 
 if (mqtt.connected()) { 
   return; 
 } 
 Serial.print("Connecting to MQTT... "); 
 uint8_t retries = 3; 
 while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected 
      Serial.println(mqtt.connectErrorString(ret)); 
      Serial.println("Retrying MQTT connection in 5 seconds..."); 
      mqtt.disconnect(); 
      delay(5000);  // wait 5 seconds 
      retries--; 
      if (retries == 0) { 
        // basically die and wait for WDT to reset me 
        while (1); 
      } 
 } 
 Serial.println("MQTT Connected!"); 
} 
