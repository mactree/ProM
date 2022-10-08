#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define SELECT_PIN 10
#define SINGLE_PIN 11
#define DOUBLE_PIN 12

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
SoftwareSerial mySerial(2, 3, true);  // RX, TX, 

byte inData[15];
int index= -1;
byte iByte;
float t = 0;
float testf = 5;
int selectState = 0;
int singleState = 0;
int doubleState = 0;

unsigned long previousMillis = 0;
const long interval = 50;  


bool sendT=0;

void setup() {
  pinMode(SELECT_PIN, INPUT);
  pinMode(SINGLE_PIN, INPUT);
  pinMode(DOUBLE_PIN, INPUT);
  
  mySerial.begin(19200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 30);
  // Display static text
  display.println("Hello, ProM!");
  display.display(); 
  delay(3000);
  display.clearDisplay();
  display.display();    
}

void loop() {
  
// testing time + and - on button, to be added to serial while
if (digitalRead(SINGLE_PIN)&& !singleState){// to be extended by && !selectState
  singleState = 1;
  testf -= 0.2;
}
if (digitalRead(DOUBLE_PIN)&& !doubleState){// to be extended by && !selectState
  doubleState = 1;
  testf += 0.2;
}
if (!digitalRead(SINGLE_PIN)&& singleState){// to be extended by && !selectState
  singleState = 0;
}
if (!digitalRead(DOUBLE_PIN)&& doubleState){// to be extended by && !selectState
  doubleState = 0;
}
display.clearDisplay();
display.setCursor(25, 0);
display.println("Single");
display.setCursor(40, 30);
display.print(testf,1);
display.print('s');  
display.display(); 
  
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
          // calculate time based on t=dec(byte8byte7)*0.0018
          t = ((inData[7] << 8) + inData[6]) * 18;
          float ts = t/1000;
          // print time to screen
          display.setCursor(0, 10);
          display.print(inData[3]);
          display.display();
          display.setCursor(0, 30);
          display.print(ts,1);  
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
  mySerial.write(31);// byte8 = 1, byte7 = 31 --> 5.17s
  mySerial.write(1);
  mySerial.write(221);
}
