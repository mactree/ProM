void checkSettings() {
  int requestReset = !digitalRead(SELECT_BTN_PIN);
  delay(2000);
  int stillRequestReset = !digitalRead(SELECT_BTN_PIN);

  if (requestReset && stillRequestReset) {
    show(resetText);

    EEPROM.write(0, 0);

    while (!digitalRead(SELECT_BTN_PIN)) // wait and proceed only when key is released
      ;
  }

  readSettings();
}

void readSettings() {
  // version check
  bool readFromEEPROM = EEPROM.read(0) == EEPROM_VALUE_VERSION;
  if (readFromEEPROM) {
    // dose time values
    doseTime[DOSE1] = EEPROM.read(DOSE1);
    doseTime[DOSE2] = EEPROM.read(DOSE2);

    // menu
    menu = EEPROM.read(3);
  }
  else {
    // first time init
    doseTime[DOSE1] = 55;
    EEPROM.write(DOSE1, doseTime[DOSE1]);
    doseTime[DOSE2] = 93;
    EEPROM.write(DOSE2, doseTime[DOSE2]);

    menu = DOSE1;
    EEPROM.write(3, menu);

    EEPROM.write(0, EEPROM_VALUE_VERSION);
  }
}  
