byte handleButton1() {
  static bool buttonWasPressed; // previous state
  static bool longButtonPress;
  static unsigned long longButtonPressedTime; // press running duration

  bool buttonNowPressed = !digitalRead(BTN1); // pin low -> pressed

  if (buttonNowPressed) {
    if (!buttonWasPressed) {
      longButtonPressedTime = millis() + LONGPRESS_TIME;
      buttonWasPressed = true;
    }
    else if (!longButtonPress && (millis() >= longButtonPressedTime)) {
      longButtonPress = true;
      return BTN1_LONGPRESS;
    }
  }
  else if (buttonWasPressed) {
    buttonWasPressed = false;
    longButtonPress = false;
    if (millis() < longButtonPressedTime)
      return BTN1_SHORTPRESS;
  }

  return BTN1_NONE;
}

byte handleButton2() {
  static bool buttonWasPressed; // previous state
  static bool longButtonPress;
  static unsigned long longButtonPressedTime; // press running duration

  bool buttonNowPressed = !digitalRead(BTN2); // pin low -> pressed

  if (buttonNowPressed) {
    if (!buttonWasPressed) {
      longButtonPressedTime = millis() + LONGPRESS_TIME;
      buttonWasPressed = true;
    }
    else if (!longButtonPress && (millis() >= longButtonPressedTime)) {
      longButtonPress = true;
      return BTN2_LONGPRESS;
    }
  }
  else if (buttonWasPressed) {
    buttonWasPressed = false;
    longButtonPress = false;
    if (millis() < longButtonPressedTime)
      return BTN2_SHORTPRESS;
  }

  return BTN2_NONE;
}


byte handleButton3() {
  static bool buttonWasPressed; // previous state
  static bool longButtonPress;
  static unsigned long longButtonPressedTime; // press running duration

  bool buttonNowPressed = !digitalRead(BTN3); // pin low -> pressed

  if (buttonNowPressed) {
    if (!buttonWasPressed) {
      longButtonPressedTime = millis() + LONGPRESS_TIME;
      buttonWasPressed = true;
    }
    else if (!longButtonPress && (millis() >= longButtonPressedTime)) {
      longButtonPress = true;
      return BTN3_LONGPRESS;
    }
  }
  else if (buttonWasPressed) {
    buttonWasPressed = false;
    longButtonPress = false;
    if (millis() < longButtonPressedTime)
      return BTN3_SHORTPRESS;
  }

  return BTN3_NONE;
}
