#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11, true);  // RX, TX, 

byte inData[15];
int index= -1;
byte iByte;
float t = 0;

unsigned long previousMillis = 0;
const long interval = 50;  


bool sendT=0;

void setup() {
    Serial.begin(19200);
    mySerial.begin(19200);
    
}

void loop() {


while (mySerial.available()) {  
  byte iByte = mySerial.read();

  if (iByte == 240){
//      Serial.println("start of msg");
      index = 0;
      memset(inData,0,sizeof(inData));

  }

  if (index>=0) {
      inData[index++] = iByte;  // adding to message

      if (index >= 9 ) {
          // call process function
          index = 0;
          /*
          Serial.print(inData[])
          for (int i = 0; i < 9; i++) Serial.print(inData[i]);
          Serial.println();
          */
          t = ((inData[7] << 8) + inData[6]) * 18;
          float ts = t/1000;
        Serial.print(inData[3]);
        Serial.print(" ");
        //Serial.print(inData[4]);
        //Serial.print(" ");
        //Serial.print(inData[5]);
        //Serial.print(" ");
        Serial.print(inData[6]);
        Serial.print(" ");
        Serial.print(inData[7]);
        Serial.print(" ");
        Serial.print(inData[8]);
        Serial.print(" ");
        //Serial.print(t);
        Serial.print(" ");
        Serial.print(ts,1);        
        Serial.println();   
      }
  }
}

unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

sendHeartbeat();
  }
  if(!sendT){
    sendTime();
    sendT=1;
  }

}

void sendHeartbeat() {
  mySerial.write(240);
  mySerial.write(1);
  mySerial.write(6);
  mySerial.write(9);
}


void sendTime() {
//'240','6','12','1','0','0','231','3','19'
// die Summe von byte4 + byte5 + byte6 + byte7 + byte8 + byte9 = 254
// byte9 ist die "Prüfsumme" byte9 = 254 - (byte4 + byte5 + byte6 + byte7 + byte8)
  
  mySerial.write(240);
  mySerial.write(6);
  mySerial.write(12);
  mySerial.write(1);
  mySerial.write((byte)0);
  mySerial.write((byte)0);
  mySerial.write(231);
  mySerial.write(3);
  mySerial.write(19);
}
