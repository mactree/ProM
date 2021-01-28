byte handleButton() {
  static bool buttonWasPressed; // previous state
  static bool longButtonPress;
  static unsigned long longButtonPressedTime; // press running duration

  bool buttonNowPressed = !digitalRead(SELECT_BTN_PIN); // pin low -> pressed

  if (buttonNowPressed) {
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
