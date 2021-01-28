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

#define MACHINE_NAME      F("Super Jolly")

const byte RELAY_PIN = 7;  // relay pin for controlling motor. pin 8 when using SPI, else change to D12.
const bool ON = HIGH;    // pin state for acting relay as ON
const bool OFF = LOW;    // pin state for acting relay as OFF

// rotary encoder must use pin D2 and D3 using UNO / NANO or pin D8 and D9 using LEONARDO / MICRO
const byte ENCODER_A_PIN = 2;
const byte ENCODER_B_PIN = 3;

const byte SELECT_BTN_PIN = 4; // pin to use as SELECT button (D4). Might be button of encoder switch

// Softserial
SoftwareSerial mySerial(5, 6, true);  // RX, TX,

// *** END OF CONFIG SECTION ***


#define EEPROM_VALUE_VERSION   9

#define ADD_GRINDING           0
#define DOSE1                  1
#define DOSE2                  2


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

byte inData[15];
int index = -1;
byte iByte;
float t = 0;
float ts;

byte doseTime[] = {0, 55, 93}; // time in 1/10s that is value 55 for 5.5s (this is done to prevent the memory consuming and slow usage of float/double)
// NOTE: this implicates that we support a maximum time of 25.5s!!!!!


// ***  BEGIN of UI handling

// online conversion tool: http://www.digole.com/tools/PicturetoC_Hex_converter.php

#define bean1_width 40
#define bean1_height 41
static unsigned char bean1_bits[] U8G_PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xff, 0x00, 0x00, 0x00,
  0xf8, 0xff, 0x03, 0x00, 0x00, 0xfe, 0xff, 0x07, 0x00, 0x80, 0xff, 0xff,
  0x0f, 0x00, 0xc0, 0xff, 0xff, 0x07, 0x00, 0xf0, 0xff, 0x7f, 0x30, 0x00,
  0xf8, 0xff, 0x3f, 0x3c, 0x00, 0xfc, 0xff, 0x07, 0x7f, 0x00, 0xfe, 0xff,
  0x83, 0x7f, 0x00, 0xff, 0xff, 0xe0, 0xff, 0x80, 0xff, 0xff, 0xf0, 0xff,
  0xc0, 0xff, 0x3f, 0xf8, 0xff, 0xc0, 0xff, 0x3f, 0xfc, 0xff, 0xe0, 0xff,
  0x1f, 0xfe, 0xff, 0xf0, 0xff, 0x0f, 0xfe, 0xff, 0xf0, 0xff, 0x0f, 0xff,
  0xff, 0xf8, 0xff, 0x87, 0xff, 0x7f, 0xf8, 0xff, 0x87, 0xff, 0x7f, 0xfc,
  0xff, 0xc7, 0xff, 0x7f, 0xfc, 0xff, 0xc3, 0xff, 0x7f, 0xfc, 0xff, 0xc3,
  0xff, 0x3f, 0xfe, 0xff, 0xe3, 0xff, 0x3f, 0xfe, 0xff, 0xe1, 0xff, 0x1f,
  0xfe, 0xff, 0xe1, 0xff, 0x1f, 0xfe, 0xff, 0xf0, 0xff, 0x0f, 0xfe, 0x7f,
  0xf0, 0xff, 0x0f, 0xfe, 0x3f, 0xf8, 0xff, 0x07, 0xfe, 0x1f, 0xfc, 0xff,
  0x03, 0xfe, 0x0f, 0xfe, 0xff, 0x01, 0xfe, 0x07, 0xff, 0xff, 0x01, 0xfe,
  0x83, 0xff, 0xff, 0x00, 0xfe, 0xe0, 0xff, 0x7f, 0x00, 0x3c, 0xf0, 0xff,
  0x1f, 0x00, 0x0c, 0xfc, 0xff, 0x0f, 0x00, 0x80, 0xff, 0xff, 0x07, 0x00,
  0xf0, 0xff, 0xff, 0x03, 0x00, 0xe0, 0xff, 0xff, 0x00, 0x00, 0xc0, 0xff,
  0x3f, 0x00, 0x00, 0x00, 0xff, 0x07, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00,
  0x00
};

#define hand1_width 40
#define hand1_height 53
static unsigned char hand1_bits[] U8G_PROGMEM  = {
  0x00, 0xe0, 0x0f, 0x00, 0x00, 0x00, 0xfc, 0x7f, 0x00, 0x00, 0x00, 0xff,
  0xff, 0x01, 0x00, 0x80, 0x5f, 0xf0, 0x07, 0x00, 0xe0, 0x07, 0xc0, 0x0f,
  0x00, 0xe0, 0x01, 0x00, 0x1f, 0x00, 0xf0, 0x00, 0x00, 0x3e, 0x00, 0x78,
  0x00, 0x00, 0x3c, 0x00, 0x3c, 0x00, 0x00, 0x78, 0x00, 0x3c, 0x00, 0x00,
  0x70, 0x00, 0x1e, 0x00, 0x00, 0xf0, 0x00, 0x0e, 0x00, 0x00, 0xe0, 0x00,
  0x0e, 0x00, 0x00, 0xe0, 0x01, 0x0e, 0x00, 0x03, 0xc0, 0x01, 0x0f, 0xc0,
  0x07, 0xc0, 0x01, 0x07, 0xc0, 0x0f, 0xc0, 0x01, 0x07, 0xe0, 0x0f, 0xc0,
  0x01, 0x07, 0xe0, 0x0f, 0xc0, 0x01, 0x0e, 0xc0, 0x0f, 0xc0, 0x01, 0x0e,
  0xe0, 0x0f, 0xc0, 0x01, 0x0e, 0xe0, 0x0f, 0xe0, 0x00, 0x0e, 0xc0, 0x0f,
  0xe0, 0x00, 0x1e, 0xe0, 0x0f, 0xf0, 0x00, 0x1c, 0xc0, 0x0f, 0x00, 0x00,
  0x3c, 0xe0, 0x0f, 0x00, 0x00, 0x78, 0xe0, 0x0f, 0x00, 0x00, 0xf0, 0xc0,
  0x4f, 0x00, 0x00, 0xf0, 0xe1, 0xef, 0x01, 0x00, 0xe0, 0xc3, 0xff, 0x03,
  0x00, 0x80, 0xe3, 0xff, 0x03, 0x00, 0x00, 0xe3, 0xff, 0xf7, 0x00, 0x00,
  0xc0, 0xff, 0xff, 0x01, 0x00, 0xe0, 0xff, 0xff, 0x01, 0x00, 0xe0, 0xff,
  0xff, 0x39, 0x00, 0xc0, 0xff, 0xff, 0x7d, 0x00, 0xe0, 0xff, 0xff, 0x7f,
  0x00, 0xe0, 0xff, 0xff, 0xff, 0x00, 0xc0, 0xff, 0xff, 0xff, 0xf0, 0xe0,
  0xff, 0xff, 0x7f, 0xf8, 0xe1, 0xff, 0xff, 0xff, 0xf8, 0xc3, 0xff, 0xff,
  0x7f, 0xf8, 0xe7, 0xff, 0xff, 0xff, 0xf8, 0xe7, 0xff, 0xff, 0x7f, 0xf0,
  0xcf, 0xff, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xff, 0x7f, 0xc0, 0xff, 0xff,
  0xff, 0xff, 0xc0, 0xff, 0xff, 0xff, 0x7f, 0x80, 0xff, 0xff, 0xff, 0xff,
  0x00, 0xff, 0xff, 0xff, 0x7f, 0x00, 0xfe, 0xff, 0xff, 0xff, 0x00, 0xfc,
  0xff, 0xff, 0x7f, 0x00, 0xf8, 0xff, 0xff, 0x7f, 0x00, 0xe0, 0xff, 0xff,
  0x3f
};

void show(void (*content)()) {

  display.firstPage();
  do {
    content();
  } while (display.nextPage());
}



void splashScreen() {
  display.setFont(u8g_font_fub14r); // u8g_font_fub17r?
  display.drawStr((128 - display.getStrWidth(MACHINE_NAME)) / 2, 26, MACHINE_NAME);

  char buffer[4]; // note: do not change to static declaration! this will not work within the picture loop of u8glib
  buffer[0] = 'v';
  buffer[1] = '.';
  buffer[2] = toChar((EEPROM_VALUE_VERSION));
  buffer[3] = '\0';
  display.drawStr((128 - display.getStrWidth(buffer)) / 2, 60, buffer);
}

void resetText() {
  display.drawStr((128 - display.getStrWidth(F("RESET"))) / 2, 26, F("RESET"));

  //Serial.println("Reset");
}

char toChar(byte val) {
  return '0' + val;
}

void actualTime() {

  if (inSetupMode) {
    float ct = currentTime/10;
    display.setPrintPos(0, 60);
    display.print("SET=");
    display.setPrintPos(60, 60);
    display.print(currentTime );
    display.print("s");

  }
  else {
      display.setPrintPos(60, 60);
      display.print(ts,1);
      display.print("s");
  }
  


}

void actualMenu() {
  if (menu == ADD_GRINDING) {
    display.drawXBMP( 0, 0, hand1_width, hand1_height, hand1_bits);
  }
  else {
    display.drawXBMP( 0, 0, bean1_width, bean1_height, bean1_bits);
    if (menu == DOSE2) {
      display.drawXBMP( 40, 0, bean1_width, bean1_height, bean1_bits);
    }
  }

  actualTime();

}

// *** END of UI handling


// heartbeat, response
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
  // currentTime /18 *1000
  int cTime = ((currentTime * 100) / 18) + 1;
  byte byte7 = lowByte(cTime);
  byte byte8 = highByte(cTime);
  byte byte9 = (253 - byte8 - byte7);

  Serial.print(byte7);
  Serial.print(" ");
  Serial.print(byte8);
  Serial.print(" ");
  Serial.print(byte9);
  Serial.print(" ");
  Serial.print(cTime);
  Serial.print(" ");
  Serial.println(currentTime, 1);


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

  Serial.begin(9600);
  mySerial.begin(19200);

  // set display rotation
  //display.setRot180();

  show(splashScreen);

  checkSettings();

  currentTime = doseTime[menu];
  show(actualMenu);

  // setup timer
  MsTimer2::set(100, updateRelayState);  // period to check relay state: every 100ms
  MsTimer2::start();

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
        Serial.print(inData[6]);
        Serial.print(" ");
        Serial.println(ts, 1);
        

        //tft.drawNumber(inData[3], 50, 20, 2);
        //tft.drawFloat(ts, 1, 50, 80, 6);
      }
    }
    if (inData[3] == 160) {
      sendTime();
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

  if (relayOn) {
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
  else if (!inSetupMode && currentTime == 0 && menu != ADD_GRINDING) {
    currentTime = doseTime[menu];
    show(actualMenu);
    //Serial.println("Stop");
  }
}




void encoderChanged(bool addValue) {
  if (inSetupMode) {
    currentTime += addValue ? 1 : -1;
    if (currentTime < 0)
      currentTime = 0;
    else if (currentTime > 180)
      currentTime = 180;
  }
  else {
    if (addValue) {
      if (menu < DOSE2) {
        menu++;
      }
      else {
        menu = ADD_GRINDING;
      }
    }
    else {
      if (menu > ADD_GRINDING) {
        menu--;
      }
      else {
        menu = DOSE2;
      }
    }
    EEPROM.write(3, menu);
    currentTime = doseTime[menu];
    sendTime();
  }
}

void handleSetupMode() {
  // enter setup mode
  Serial.print("setupmode: ");
  Serial.println(inSetupMode);
  inSetupMode = !inSetupMode;
  if (inSetupMode) {
    // get old values
    currentTime = doseTime[menu];
  }
  else {
    // get new values
    doseTime[menu] = currentTime;
    EEPROM.write(menu, currentTime);
  }
}

void updateRelayState() {
  if (relayOn) {
    updateTime = true;
    if (menu == ADD_GRINDING) {
      currentTime++;
    }
    else {
      currentTime--;
      relayOn = currentTime > 0;
    }
  }

  digitalWrite(RELAY_PIN, relayOn ? ON : OFF);
}
