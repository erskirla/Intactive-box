long heartBeatArray[] = {
    50, 100, 15, 1200};
int hbeatIndex = 1;   // this initialization is important or it starts on the "wrong foot"
long prevMillis;

void heartBeat(float tempo){
    if ((millis() - prevMillis) > (long)(heartBeatArray[hbeatIndex] * tempo)){
        hbeatIndex++;
        if (hbeatIndex > 3) hbeatIndex = 0;

        if ((hbeatIndex % 2) == 0){ 
            digitalWrite(LED_PIN, HIGH);
            digitalWrite(LED_blue, HIGH); 
            delay((int)heartBeatArray[hbeatIndex]) ;   
            digitalWrite(LED_PIN, LOW); 
            digitalWrite(LED_blue, LOW);
        }
        hbeatIndex++;
        // Serial.println(hbeatIndex);
        prevMillis = millis();
       

    }
}
