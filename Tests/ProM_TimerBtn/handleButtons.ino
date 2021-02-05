byte handleButton() {
  static bool buttonWasPressed; // previous state
  static bool longButtonPress;
  static unsigned long longButtonPressedTime; // press running duration

  bool buttonNowPressed = digitalRead(SELECT_PIN); // pin low -> pressed

  if (buttonNowPressed) {
    lastbuttonTime = millis();
    if (!buttonWasPressed) {
      longButtonPressedTime = millis() + LONGPRESS_TIME;
      buttonWasPressed = true;
    }
    else if (!longButtonPress && (millis() >= longButtonPressedTime)) {
      longButtonPress = true;
      return BTN_LONGPRESS;
    }
  }
  else if (buttonWasPressed) {
    buttonWasPressed = false;
    longButtonPress = false;
    if (millis() < longButtonPressedTime)
      return BTN_SHORTPRESS;
  }

  return BTN_NONE;
}


bool handleDoseButtons(){
  if (digitalRead(INCDOSE_PIN)&& digitalRead(DECDOSE_PIN)){ // prevent double button press
    doublePress = true;
    doubleErr();
    lastbuttonTime = millis();
  }
  else {
    doublePress = false;
  }

  if (digitalRead(INCDOSE_PIN)){
    currentTime += 2;  
    lastbuttonTime = millis();  
  }
  if (digitalRead(DECDOSE_PIN)){
    currentTime -= 2; 
    lastbuttonTime = millis();
  }
  EEPROM.write(menu, currentTime);
  sendTime();
}
