/*************************************************** 
  NodeMCU
****************************************************/ 
#include <ESP8266WiFi.h> 
#include "Adafruit_MQTT.h" 
#include "Adafruit_MQTT_Client.h" 
/************************* WiFi Access Point *********************************/ 
#define WLAN_SSID       "OsloMet MakerSpace" 
#define WLAN_PASS       "o5lom3et2018" 
#define MQTT_SERVER     "10.253.27.226" // static ip address
#define MQTT_PORT       1883                    
#define MQTT_USERNAME   "" 
#define MQTT_PASSWORD   "" 
#define BUTTON1_PIN     14                    
#define BUTTON2_PIN     12
/************ Global State ******************/ 
// Create an ESP8266 WiFiClient class to connect to the MQTT server. 
WiFiClient client; 
// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details. 
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_PORT, MQTT_USERNAME, MQTT_PASSWORD); 
/****************************** Feeds ***************************************/ 
// Setup a feed called 'buttons' for publishing. 
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname> 
Adafruit_MQTT_Publish pi_buttons = Adafruit_MQTT_Publish(&mqtt, MQTT_USERNAME "/buttons/pi"); 
//Adafruit_MQTT_Publish pi_button2 = Adafruit_MQTT_Publish(&mqtt, MQTT_USERNAME "/buttons/pi"); 
/*************************** Sketch Code ************************************/ 

void MQTT_connect(); 
void setup() { 
 Serial.begin(115200); 
 delay(10); 
 // Setup buttons as an input with internal pull-up. 
 pinMode(BUTTON1_PIN, INPUT_PULLUP);
 pinMode(BUTTON2_PIN, INPUT_PULLUP); 
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
} 
uint32_t x=0;
 
void loop() { 
 // Check if the button has been pressed by looking for a change from high to 
 // low signal (with a small delay to debounce). 
 int button1_first = digitalRead(BUTTON1_PIN); 
 int button2_first = digitalRead(BUTTON2_PIN); 
 // Ensure the connection to the MQTT server is alive (this will make the first 
 // connection and automatically reconnect when disconnected).  See the MQTT_connect 
 MQTT_connect();   
 delay(20); 
 int button1_second = digitalRead(BUTTON1_PIN); 
 int button2_second = digitalRead(BUTTON2_PIN); 
 if ((button1_first == HIGH) && (button1_second == LOW)) { 
   // First button was pressed! 
   Serial.println("First button pressed!"); 
   pi_buttons.publish("Button 1"); 
  } 
 else if ((button2_first == HIGH) && (button2_second == LOW)) { 
   // Second button was pressed! 
   Serial.println("Second button pressed!"); 
   pi_buttons.publish("Button 2"); 
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
