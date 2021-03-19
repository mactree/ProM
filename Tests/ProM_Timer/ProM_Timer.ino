/*

  This is a minimalistic rewrite of the original Mühlentimer V3.1.0 of Michael Hinderer.
  It has less features but uses far less code and an OLED support. Enjoy, experiment, go!

  NOTE: Compile using Arduino IDE 1.5.7 or newer only!

  By Matthias Mueller (user mamu of http://www.kaffee-netz.de )

*/

#define U8G_HW_SPI_2X 1

#include <EEPROM.h>
#include <Wire.h> // http://arduino.cc/de/Reference/Wire
#include <U8glib.h> // http://code.google.com/p/u8glib/wiki/userreference
#include <MsTimer2.h> // http://playground.arduino.cc/Main/MsTimer2
#include <SoftwareSerial.h>


// *** BEGIN OF CONFIG SECTION ***

#define MACHINE_NAME      F("ProM")

const byte RELAY_PIN = 7;  // relay pin for controlling motor. pin 8 when using SPI, else change to D12.
const bool ON = HIGH;    // pin state for acting relay as ON
const bool OFF = LOW;    // pin state for acting relay as OFF

// rotary encoder must use pin D2 and D3 using UNO / NANO or pin D8 and D9 using LEONARDO / MICRO
const byte ENCODER_A_PIN = 2;
const byte ENCODER_B_PIN = 3;

const byte SELECT_BTN_PIN = 4; // pin to use as SELECT button (D4). Might be button of encoder switch

// Softserial
SoftwareSerial mySerial(6, 5, true);  // RX, TX,

// *** END OF CONFIG SECTION ***


#define EEPROM_VALUE_VERSION   9

#define ADD_GRINDING           0
#define DOSE1                  1
#define DOSE2                  2
#define DOSE3                  3



U8GLIB_SSD1306_128X64 display(U8G_I2C_OPT_NONE);        // I2C / TWI SCA=a4, SCL=a5 , U8G_I2C_OPT_NONE U8G_I2C_OPT_FAST
//U8GLIB_SSD1306_128X64 display(U8G_PIN_NONE /*cs*/, 9 /*a0*/, 8 /*reset*/); // SPI HW: cs=10, a0=9, RST=8, MOSI=11, SCK=13 (note: some modules lack cs)
//U8GLIB_SSD1306_128X64 display(10, 9); // for heltec spi oled

/*
  Pin Names: http://code.google.com/p/u8glib/wiki/device
  sck: Clock
  mosi: Serial data
  cs, cs1, cs2: Chip select, datasheet names: CS, CS1, CS2 or CE
  a0, di: Data/instruction register select, datasheet names: A0, DI, CD, D/C or RS
  rw: Read/write line
  en: Read/write enable
  d0 - d7: Parallel data
  reset: Display reset, datasheet names: RST
  SPI: http://arduino.cc/en/pmwiki.php?n=Reference/SPI
*/

#define LONGPRESS_TIME    2000  // 2s for a long press
enum { BTN_NONE = 0, BTN_SHORTPRESS, BTN_LONGPRESS };

// encoder: http://playground.arduino.cc/Main/RotaryEncoders
volatile int encoderPos = 0;
int lastEncoderPos = 0;
unsigned long lastIdleTime;

volatile byte menu = DOSE1;
volatile bool relayOn = false;
volatile byte currentTime = 0;
volatile bool updateTime;
bool inSetupMode = false;
// interrupt service routine vars
bool A_set = false;
bool B_set = false;
static bool rotating = false;


unsigned long previousMillis = 0;
const long interval = 50;

byte inData[10];
int index = -1;
byte iByte;
float t = 0;
float ts;

byte doseTime[] = {0, 11, 22, 33}; // time in 1/10s that is value 55 for 5.5s (this is done to prevent the memory consuming and slow usage of float/double)
// NOTE: this implicates that we support a maximum time of 25.5s!!!!!


// ***  BEGIN of UI handling

// online conversion tool: http://www.digole.com/tools/PicturetoC_Hex_converter.php

#define cup_width 32
#define cup_height 24
static unsigned char cup[] U8G_PROGMEM = {
  0xFF, 0xFF, 0xFF, 0x01, 0xFF, 0xFF, 0xFF, 0x01, 0x03, 0x00, 0xC0, 0x3F, 
  0x03, 0x00, 0x80, 0x7F, 0x03, 0x00, 0x80, 0xE1, 0x03, 0x00, 0x80, 0xC1, 
  0x03, 0x00, 0x80, 0xC1, 0x03, 0x00, 0x80, 0x61, 0x03, 0x00, 0x80, 0x60, 
  0x03, 0x00, 0xC0, 0x30, 0x06, 0x00, 0xC0, 0x1C, 0x06, 0x00, 0xC0, 0x0E, 
  0x06, 0x00, 0xE0, 0x03, 0x06, 0x00, 0xE0, 0x01, 0x0E, 0x00, 0xE0, 0x00, 
  0x0C, 0x00, 0x70, 0x00, 0x0C, 0x00, 0x30, 0x00, 0x1C, 0x00, 0x10, 0x00, 
  0x18, 0x00, 0x18, 0x00, 0x18, 0x00, 0x18, 0x00, 0x18, 0x00, 0x08, 0x00, 
  0x30, 0x00, 0x0C, 0x00, 0xF0, 0xFF, 0x0F, 0x00, 0xF0, 0xFF, 0x07, 0x00, 
};

#define cups_width 44
#define cups_height 30
static unsigned char cups[] U8G_PROGMEM = {
  0xFE, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 
  0x03, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x03, 0x00, 0xC0, 0x00, 0x00, 0x00, 
  0x03, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x03, 0x00, 0xC0, 0x00, 0x00, 0x00, 
  0x03, 0xF0, 0xFF, 0xFF, 0x1F, 0x00, 0x03, 0xF0, 0xFF, 0xFF, 0x1F, 0x00, 
  0x03, 0x30, 0x00, 0x00, 0xFC, 0x03, 0x06, 0x30, 0x00, 0x00, 0xF8, 0x07, 
  0x06, 0x30, 0x00, 0x00, 0x18, 0x0E, 0x06, 0x30, 0x00, 0x00, 0x18, 0x0C, 
  0x06, 0x30, 0x00, 0x00, 0x18, 0x0C, 0x0C, 0x30, 0x00, 0x00, 0x18, 0x06, 
  0x0C, 0x30, 0x00, 0x00, 0x08, 0x06, 0x0C, 0x30, 0x00, 0x00, 0x0C, 0x03, 
  0x0C, 0x60, 0x00, 0x00, 0xCC, 0x01, 0x18, 0x60, 0x00, 0x00, 0xEC, 0x00, 
  0x18, 0x60, 0x00, 0x00, 0x3E, 0x00, 0x18, 0x60, 0x00, 0x00, 0x1E, 0x00, 
  0x18, 0xE0, 0x00, 0x00, 0x0E, 0x00, 0xF0, 0xFF, 0x00, 0x00, 0x07, 0x00, 
  0xF0, 0xFF, 0x00, 0x00, 0x03, 0x00, 0x00, 0xC0, 0x01, 0x00, 0x01, 0x00, 
  0x00, 0x80, 0x01, 0x80, 0x01, 0x00, 0x00, 0x80, 0x01, 0x80, 0x01, 0x00, 
  0x00, 0x80, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x03, 0xC0, 0x00, 0x00, 
  0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x7F, 0x00, 0x00, 
};

#define portalfilter_width 60
#define portalfilter_height 16
static unsigned char portalfilter[] U8G_PROGMEM = {
  0xFF, 0xFB, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xDF, 0xDF, 0x0D, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x08, 0x00, 0x40, 0x01, 0xFA, 0x07, 
  0x01, 0x00, 0x8C, 0xC4, 0xFF, 0xFF, 0xFF, 0x07, 0x01, 0x00, 0xF8, 0xFF, 
  0xFF, 0x6F, 0x40, 0x06, 0x03, 0x00, 0xF8, 0x2F, 0x00, 0x00, 0x00, 0x0C, 
  0x03, 0x00, 0x18, 0x07, 0x00, 0x00, 0x00, 0x04, 0x06, 0x02, 0x00, 0x07, 
  0x00, 0x00, 0x00, 0x0C, 0xFE, 0xFF, 0x1F, 0x07, 0x00, 0x00, 0x00, 0x0C, 
  0xC4, 0x3F, 0xBC, 0x07, 0x00, 0x00, 0x00, 0x04, 0xE0, 0x3F, 0xF8, 0x0F, 
  0x00, 0x00, 0x00, 0x0C, 0xE0, 0x70, 0x80, 0xFF, 0xFF, 0x1F, 0x20, 0x06, 
  0x70, 0x67, 0x00, 0xA1, 0xFF, 0xDF, 0xFF, 0x07, 0x10, 0xCF, 0x00, 0x00, 
  0x00, 0x00, 0xF8, 0x03, 0xB8, 0xF9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0xF0, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };

// *** END of UI handling







void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, OFF);

  pinMode(SELECT_BTN_PIN, INPUT);
  digitalWrite(SELECT_BTN_PIN, HIGH);

  pinMode(ENCODER_A_PIN, INPUT);
  digitalWrite(ENCODER_A_PIN, HIGH);

  pinMode(ENCODER_B_PIN, INPUT);
  digitalWrite(ENCODER_B_PIN, HIGH);

  pinMode(ENCODER_A_PIN, INPUT);
  digitalWrite(ENCODER_A_PIN, HIGH);

  pinMode(ENCODER_B_PIN, INPUT);
  digitalWrite(ENCODER_B_PIN, HIGH);

  attachInterrupt(0, doEncoderA, CHANGE);
  attachInterrupt(1, doEncoderB, CHANGE);

  //Serial.begin(9600);
  mySerial.begin(19200);

  // set display rotation
  //display.setRot180();

  show(splashScreen);

  checkSettings();

  currentTime = doseTime[menu];
  show(actualMenu);

  // setup timer
//  MsTimer2::set(100, updateRelayState);  // period to check relay state: every 100ms
//  MsTimer2::start();

  lastIdleTime = millis() + 1000;
}

void loop() {
  delay(10);
  rotating = true;  // reset the debouncer

  // handle button
  byte btnEvent = handleButton();

  //
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    sendHeartbeat();
  }
  while (mySerial.available()) {
    byte iByte = mySerial.read();

    if (iByte == 240) {
      //      Serial.println("start of msg");
      index = 0;
      memset(inData, 0, sizeof(inData));

    }

    if (index >= 0) {
      inData[index++] = iByte;  // adding to message

      if (index >= 9 ) {
        // call process function
        index = 0;

        t = ((inData[7] << 8) + inData[6]) * 18;
        ts = t / 1000;
        Serial.print(inData[3]);
        Serial.print(inData[6]);
        Serial.print(" ");
        Serial.println(ts, 1);
        
        show(actualMenu);
        //tft.drawNumber(inData[3], 50, 20, 2);
        //tft.drawFloat(ts, 1, 50, 80, 6);
      }
    }
    if (inData[3] == 160) {
      sendTime();
      show(actualMenu);
      //tft.fillScreen(TFT_BLACK);
    }
  }

  if ((btnEvent == BTN_SHORTPRESS) && !inSetupMode && menu != ADD_GRINDING) {
    relayOn = !relayOn; // toggle relay state depending on prev state: off -> on or on -> off
    //if (relayOn)
    //Serial.println("Start");
    //else
    //Serial.println("Pre Stop");
    lastEncoderPos = encoderPos;
  }


/*
  else if (!inSetupMode && menu == ADD_GRINDING) {
    // pin low -> pressed
    if (!digitalRead(SELECT_BTN_PIN)) {
      relayOn = true;
      //Serial.println("Start Add");
      lastEncoderPos = encoderPos;
    }
    else if (relayOn) {
      relayOn = false;
      //Serial.println("Stop Add");
      lastEncoderPos = encoderPos;
    }
  }
*/
  if (inData[3] == 165) {
    // prevents entering setup while relay is on
    if (updateTime) {
      updateTime = false;
      //Serial.println("relayOn");
      show(actualMenu);
    }
    lastIdleTime = millis() + 1000;
  }
  else if (lastEncoderPos != encoderPos) {
    // prevent changes in the first second afer motor stop
    if (millis() > lastIdleTime) {
      encoderChanged(encoderPos > lastEncoderPos);
      show(actualMenu);
    }
    lastEncoderPos = encoderPos;
  }
  else if ((btnEvent == BTN_LONGPRESS) && menu != ADD_GRINDING) {
    handleSetupMode();
    show(actualMenu);
  }
  /*
  else if (!inSetupMode && currentTime == 0 && menu != ADD_GRINDING) {
    currentTime = doseTime[menu];
    show(actualMenu);
    //Serial.println("Stop");
  }
*/
 
}
