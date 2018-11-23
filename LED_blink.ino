uint32_t period= 5000;

void loop() {
 // Ensure the connection to the MQTT server is alive (this will make the first 
 // connection and automatically reconnect when disconnected).  See the MQTT_connect 
 MQTT_connect(); 
 // this is our 'wait for incoming subscription' busy subloop 
 // Here its read the subscription 
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
