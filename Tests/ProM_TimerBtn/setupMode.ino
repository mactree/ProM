void handleSetupMode() {
  // enter setup mode
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
