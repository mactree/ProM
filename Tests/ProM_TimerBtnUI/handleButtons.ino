byte handleButton() {
  static bool buttonWasPressed; // previous state
  static bool longButtonPress;
  static unsigned long longButtonPressedTime; // press running duration

  bool buttonNowPressed = !digitalRead(SELECT_PIN); // pin low -> pressed

  if (buttonNowPressed) {
    lastButtonTime = millis();
    if (inSleep){//wake up, skip button action
      display.sleepOff();
      inSleep = false;
      return BTN_NONE;
    }
    if (!buttonWasPressed) {
      longButtonPressedTime = millis() + LONGPRESS_TIME;
      buttonWasPressed = true;
    }
    else if (!longButtonPress && (millis() >= longButtonPressedTime)) {
      longButtonPress = true;
      return BTN_LONGPRESS;
    }
    return BTN_CONTPRESS;
  }
  else if (buttonWasPressed) {
    buttonWasPressed = false;
    longButtonPress = false;
    if (millis() < longButtonPressedTime)
      return BTN_SHORTPRESS;
  }
  

  return BTN_NONE;
}


void handleDoseButtons(){
  if (!digitalRead(INCDOSE_PIN)&& !digitalRead(DECDOSE_PIN)){ // prevent double button press
    doublePress = true;
    doubleErr();
    lastButtonTime = millis();
  }
  else {
    doublePress = false;
  }

  if (!digitalRead(INCDOSE_PIN)){
    if (inSleep){//wake up, skip button action
      display.sleepOff();
      inSleep = false;
      lastButtonTime = millis(); 
      return;
    }
    currentTime += 2;  
    lastButtonTime = millis(); 
    sendTime();
    EEPROM.write(menu, currentTime);
  }
  if (!digitalRead(DECDOSE_PIN)){
     if (inSleep){ //wake up, skip button action
      display.sleepOff();
      inSleep = false;
      lastButtonTime = millis(); 
      return;
    }
    currentTime -= 2; 
    lastButtonTime = millis();
    sendTime();
    EEPROM.write(menu, currentTime);
  }

  return;
}
