void readSerial() {
 
 while (Serial.available()) {
    byte iByte = Serial.read();

    if (iByte == 240) {
      index = 0;
      memset(inData, 0, sizeof(inData));
    }
    if (index >= 0) {
      inData[index++] = iByte;  // adding to message

      if (index >= 9 ) {
        index = 0;
        t = ((inData[7] << 8) + inData[6]) * 18;
        /*
        t = ((t+10)/10)*10; 
        int tt = t/100;
        t = tt * 100;
        */
        updateTime = true;
      }
    }
  }
}
