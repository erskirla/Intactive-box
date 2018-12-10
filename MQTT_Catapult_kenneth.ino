/*********************** Heart beat LED setup *******************************/
#include <ESP8266WiFi.h> 
#include "Adafruit_MQTT.h" 
#include "Adafruit_MQTT_Client.h"
#include <Servo.h>
/************************* WiFi Access Point *********************************/ 
#define WLAN_SSID       "Kenneths-net" 
#define WLAN_PASS       "kenneth0987" 
#define MQTT_SERVER     "192.168.43.239" // static ip address
#define MQTT_PORT       1883                    
#define MQTT_USERNAME   "" 
#define MQTT_PASSWORD        ""
// Here you define your GPIOs
#define ledPin          14
Servo armServo;  // creates servo object to control the armServo
Servo lockServo; // creates servo object to control the lockServo
int pos1 = 0;    // variable to store the servo1 position
int pos2 = 0;    // variable to store the servo2 position
/************ Global State ******************/ 
// Create an ESP8266 WiFiClient class to connect to the MQTT server. 
WiFiClient client; 
// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details. 
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_PORT, MQTT_USERNAME, MQTT_PASSWORD); 
/****************************** Feeds ***************************************/ 
// Setup a feed called 'esp8266_led' for subscribing to changes. 
// Change "/[device]/esp8266", change [device] to your device name.
Adafruit_MQTT_Subscribe esp8266_catapult = Adafruit_MQTT_Subscribe(&mqtt, MQTT_USERNAME "/catapult/esp8266"); 
/*************************** Sketch Code ************************************/ 

void MQTT_connect(); //connects to the server
void setup() { 
 Serial.begin(115200); 
 delay(10);
 //setup for the Catapult
 pinMode(ledPin, OUTPUT); // initialize the active-LED pin as an output
 armServo.attach(15);  // attaches the armServo on pin 15 to the servo object
  armServo.write(40);          // set servo to intial position
 lockServo.attach(16);   // attaches the lockServo on pin 16 to the servo object
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
 mqtt.subscribe(&esp8266_catapult); 
}  
uint32_t period= 5000; // 5 seconds
// This is where the magic happens
void loop() {
 // Ensure the connection to the MQTT server is alive (this will make the first 
 // connection and automatically reconnect when disconnected).  See the MQTT_connect.
 // MQTT_connect(); has to be in every code (void loop) you want to add to the system.
 MQTT_connect(); 
 // this is our 'wait for incoming subscription' busy subloop 
 // Here it reads the subscription 
 Adafruit_MQTT_Subscribe *subscription; 
 while ((subscription = mqtt.readSubscription())) { 
   if (subscription == &esp8266_catapult) { 
     char *message = (char *)esp8266_catapult.lastread; 
     Serial.print(F("Got: ")); 
     Serial.println(message);
        // Checking for message 
     if (strncmp(message, "THROW", 2) == 0) { 
       // Let the catapult throw, and arm for next shoot.
       digitalWrite(ledPin, HIGH);      // turn LED on
    Serial.println("Cycle started");
    // run cycle:
      for (pos2 = 86; pos2 <= 120; pos2 += 2) { // goes from "locked" to "unlocked"
      // in steps of 2 degrees
      lockServo.write(pos2);              // tell servo to go to position in variable 'pos2'
      delay(3);                       // waits 3ms for the servo to reach the position
      }      
      Serial.print("Unlock Servo: ");
      Serial.println(pos2);
      delay(1000);                   // waits before taking the arm down
      for (pos1 = 40; pos1 <= 160; pos1 += 1) { // goes from "up" to "down"
      // in steps of 1 degrees
      armServo.write(pos1);              // tell servo to go to position in variable 'pos1'
      delay(5);                       // waits 5ms for the servo to reach the position
      }
      Serial.print("Arm Servo: ");
      Serial.println(pos1);
      delay(1000);                   // waits before locking the arm down
      for (pos2 = 120; pos2 >= 86; pos2 -= 1) { // goes from "unlocked" to "locked"
      // in steps of 1 degrees
      lockServo.write(pos2);              // tell servo to go to position in variable 'pos2'
      delay(3);                       // waits 3ms for the servo to reach the position
      }
      Serial.print("Lock Servo: ");
      Serial.println(pos2);
      delay(2000);                   // waits before taking the arm-guide up again
      for (pos1 = 160; pos1 >= 40; pos1 -= 2) { // goes from "down" to "up"
      // in steps of 2 degrees
      armServo.write(pos1);              // tell servo to go to position in variable 'pos1'
      delay(5);                       // waits 5ms for the servo to reach the position
      }
      Serial.print("Reset ArmServo: ");
      Serial.println(pos1);
      digitalWrite(ledPin, LOW);      // turn LED off
      Serial.println("Cycle finished");
      delay(5000);                   // waiting for reload
      Serial.println("Ready!");
      } 
    } 
  } 
}
/** Function to connect and reconnect as necessary to the MQTT server. **/ 
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
