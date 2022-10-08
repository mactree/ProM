void sendTime() {
  //'240','6','12','1','0','0','231','3','19'
  // die Summe von byte4 + byte5 + byte6 + byte7 + byte8 + byte9 = 254
  // byte9 ist die "Prüfsumme" byte9 = 254 - (byte4 + byte5 + byte6 + byte7 + byte8)
  // currentTime /18 *1000
  int cTime = ((currentTime * 100) / 18) + 1;
  Serial.print("cTIME ");
  Serial.print(currentTime);
  byte byte7 = lowByte(cTime);
  byte byte8 = highByte(cTime);
  byte byte9 = (253 - byte8 - byte7);
//
//  Serial.print(byte7);
//  Serial.print(" ");
//  Serial.print(byte8);
//  Serial.print(" ");
//  Serial.print(byte9);
//  Serial.print(" ");
//  Serial.print(cTime);
//  Serial.print(" ");
//  Serial.println(currentTime, 1);


  mySerial.write(240);
  mySerial.write(6);
  mySerial.write(12);
  mySerial.write(1);
  mySerial.write((byte)0);
  mySerial.write((byte)0);
  mySerial.write(byte7);
  mySerial.write(byte8);
  mySerial.write(byte9);
}
