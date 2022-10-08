#include <SoftwareSerial.h>
#include <TFT_ST7735.h> // Graphics and font library for ILI9341 driver chip
#include <SPI.h>

SoftwareSerial mySerial(2, 3, true);  // RX, TX, 
TFT_ST7735 tft = TFT_ST7735();  // Invoke library, pins defined in User_Setup.h
// 1,8 zoll TFT
// GND => GND
// VCC => 3.3V
// SCL => D13
// SDA => D11
// RES => D8
// DC => D9
// CS => D10
// BL => 3.3V

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

  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);

  tft.drawString("ProM Test", 60, 120, 1); 
  delay(1000);
  
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor( TFT_WHITE, TFT_BLACK);
    
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
          
          t = ((inData[7] << 8) + inData[6]) * 18;
          float ts = t/1000;
          tft.drawNumber(inData[3], 50, 20, 2);
          tft.drawFloat(ts, 1, 50, 80, 6);     
      }
  }
  if (inData[3] == 160){
    sendTime();
    tft.fillScreen(TFT_BLACK);
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
  mySerial.write(31);
  mySerial.write(1);
  mySerial.write(221);
}
