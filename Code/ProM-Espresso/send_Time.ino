void sendTime() {

   // doseTime[menu] *10 /18 
   int cTime = ((doseTime[menu] *10) / 18) ;
   byte byte7 = lowByte(cTime);
   byte byte8 = highByte(cTime);
   byte byte9 = (253 - byte8 - byte7);    



  Serial.write(240);
  Serial.write(6);
  Serial.write(12);
  Serial.write(1);
  Serial.write((byte)0);
  Serial.write((byte)0);
  Serial.write(byte7);
  Serial.write(byte8);
  Serial.write(byte9);
}
