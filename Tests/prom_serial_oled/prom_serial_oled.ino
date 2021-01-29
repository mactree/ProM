#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
SoftwareSerial mySerial(2, 3, true);  // RX, TX, 

byte inData[15];
int index= -1;
byte iByte;
float t = 0;

unsigned long previousMillis = 0;
const long interval = 50;  


bool sendT=0;

void setup() {
  mySerial.begin(19200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.println("Hello, ProM!");
  display.display(); 
  delay(7000);
  display.clearDisplay();
  display.display();    
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
          display.setCursor(0, 10);
          display.print(inData[3]);
          display.display();
          display.setCursor(0, 30);
          display.print(ts);  
          display.display();   
      }
  }
  if (inData[3] == 160){
    sendTime();
    display.clearDisplay();
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
