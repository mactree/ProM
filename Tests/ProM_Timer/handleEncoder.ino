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
      if (menu < DOSE3) {
        menu++;
      }
      else {
        menu = DOSE1;
      }
    }
    else {
      if (menu > DOSE1) {
        menu--;
      }
      else {
        menu = DOSE3;
      }
    }
    EEPROM.write(4, menu);
    currentTime = doseTime[menu];
    sendTime();
  }
}
