  // heartbeat, response
void sendHeartbeat() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    //message  
    Serial.write(240);
    Serial.write(1);
    Serial.write(6);
    Serial.write(9);  
    }
}   
