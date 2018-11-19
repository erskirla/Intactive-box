# Intactive-box

This is a publish-broker-subscriber system where one device publish message on to the second device. 
The second device interpets the message and sends a new message to a third device that does something.
I used 1 huzzah feather esp8266(HFE8266) as publisher, 2 HFE8266 as subscribers and a raspberry pi as a broker.

knapper_mqtt.ino is the code for the publisher.
This device has 3 buttons with the opportunity to add more buttons. 

broker.py is the python code for raspberry pi.

LED_blink.ino is the code for one device. 
This device is a transparent anatomical correct heart that blinks with the heart rythm
