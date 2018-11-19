import time 
import RPi.GPIO as GPIO 
import paho.mqtt.client as mqtt

# Setup callback functions that are called when MQTT events happen like 
# connecting to the server or receiving data from a subscribed feed. 
def on_connect(client, userdata, flags, rc): 
   print("Connected with result code " + str(rc)) 
   # Subscribing in on_connect() means that if we lose the connection and 
   # reconnect then subscriptions will be renewed. 
   client.subscribe("/buttons/pi")
   
# The callback for when a PUBLISH message is received from the server. 
def on_message(client, userdata, msg): 
   print(msg.topic+" "+str( msg.payload)) 
   # Check if this is a message for the Pi. 
   if msg.topic == '/buttons/pi': 
       # Look at the message data and perform the appropriate action. 
       if msg.payload == b'Button 1': 
           print("First button was pressed")
           client.publish('/led/esp8266', 'ON') 
       elif msg.payload == b'Button 2': 
           print("Second button was pressed")
           client.publish('/catapult/esp8266', 'THROW')
       elif msg.payload == b'Button 3': 
           print("Third button was pressed!")
            
# Create MQTT client and connect to localhost, i.e. the Raspberry Pi running 
# this script and the MQTT server. 
client = mqtt.Client() 
client.on_connect = on_connect 
client.on_message = on_message 
client.connect('localhost', 1883, 60)
print('Script is running, press Ctrl-C to quit...')
# Connect to the MQTT server and process messages in a background thread.
# Loop goes forever until disconected or interupted 
client.loop_forever()
