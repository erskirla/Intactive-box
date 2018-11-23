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
