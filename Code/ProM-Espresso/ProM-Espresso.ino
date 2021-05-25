#include <EEPROM.h>
#include <TFT_ST7735.h> // Graphics and font library for ILI9341 driver chip
#include <SPI.h>

#include "graphic.h"

//Display
TFT_ST7735 tft = TFT_ST7735();  // Invoke library, pins defined in User_Setup.h
// 1,8 zoll TFT GND => GND | VCC => 3.3V | SCL => D13 | SDA => D11 | RES => D8 | DC => D9 | CS => D10 | BL => D7
const byte BL_PIN = 7;   // backlight pin
const bool BL_ON = HIGH;    // pin state for acting backlight as ON
const bool BL_OFF = LOW;    // pin state for acting backlight as OFF

// buttons
const byte BTN1 = 3; // Timerselect and Setup
const byte BTN2 = 4; // minus / cup
const byte BTN3 = 2; // plus / double cup

enum { BTN1_NONE = 0, BTN1_SHORTPRESS, BTN1_LONGPRESS };
enum { BTN2_NONE = 0, BTN2_SHORTPRESS, BTN2_LONGPRESS };
enum { BTN3_NONE = 0, BTN3_SHORTPRESS, BTN3_LONGPRESS };

#define LONGPRESS_TIME    2000  // 2s for a long press


// EEPROM
#define EEPROM_VALUE_VERSION   8

#define SETTINGS               0
#define DOSE1                  1 // one cup
#define DOSE2                  2 // double cup
#define DOSE3                  3 // portalfilter manual mode 18s

#define SET1                   1 // cleaning alarm
#define SET2                   2 // counter all
#define SET3                   3 // counter burs
#define SET4                   4 // enable debug



volatile byte menu = DOSE1;
volatile bool updateEpr = false;
volatile byte setting = SET1;

bool inSetupMode = false;
bool inEditMode = false;

int doseTime[] = {0, 310, 520, 1800};
int settingSet[] = {0, 0, 0, 0};

bool updateMenu = true;
bool updateTime = true;
bool updateButtons = true;


bool switchColor = false;
bool hopperOff = false;
bool resetScreen = false;
bool disableButtons = false;

int inData4_now;
int inData4_prev;


// pause grinding inData[3] == 228
unsigned long previousPauseMillis = 0;
unsigned long PausedTime;

// heartbeattimer
unsigned long previousMillis = 0;
const long interval = 50;

// grind counter
unsigned int counterTimer1;
unsigned int counterTimer2;
unsigned int counterTimer3;
unsigned int counterTotal;
unsigned int counterCleaning;

// serial buffer
byte inData[10];
int index = -1;
byte iByte;
float t = 0;

// Debug
bool showDebug = false;

void setup() {

  // start serial
  Serial.begin(19200);

  // setup button 1
  pinMode(BTN1, INPUT);
  digitalWrite(BTN1, HIGH);

  // setup button 2
  pinMode(BTN2, INPUT);
  digitalWrite(BTN2, HIGH);

  // setup button 3
  pinMode(BTN3, INPUT);
  digitalWrite(BTN3, HIGH);

  // setup backlightpin
  pinMode(BL_PIN, OUTPUT);
  digitalWrite(BL_PIN, BL_ON);

  // init display
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);

  // draw version string
  tft.setTextColor( TFT_BLUE, TFT_BLACK);
  tft.drawString("v102.105.025", 40, 80, 2);
  delay(1000);
  tft.fillScreen(TFT_BLACK);

  // read settings
  readSettings();

  // send time
  sendTime();

}

void loop() {

  // handle buttons
  byte btn1Event = handleButton1();
  byte btn2Event = handleButton2();
  byte btn3Event = handleButton3();


  // shortpress
  if (btn1Event == BTN1_SHORTPRESS && !disableButtons) {
    btn1();
  }
  // shortpress btn1 while inEditMode
  else if (btn1Event == BTN1_SHORTPRESS && inEditMode) {
    handleEditMode();
  }

  // shortpress cup
  //  else if(btn2Event == BTN2_SHORTPRESS && inData[3] == (164)){
  else if (btn2Event == BTN2_SHORTPRESS  && !disableButtons) {
    btn2();
  }
  // shortpress double cup
  //  else if(btn3Event == BTN3_SHORTPRESS && inData[3] == (164){
  else if (btn3Event == BTN3_SHORTPRESS && !disableButtons) {
    btn3();
  }
  // longpress cup or double cup
  else if (((menu == 1 && btn2Event == BTN2_LONGPRESS) || (menu == 2 && btn3Event == BTN3_LONGPRESS)) && !disableButtons) {
    handleEditMode();
  }
  // longpress cup or double cup
  else if ((btn1Event == BTN1_LONGPRESS)  && !disableButtons) {
    showDebug = !showDebug;
    updateMenu = true;
  }
  readSerial();

  tftShow();

  sendHeartbeat();

  grinderStatus();

}
