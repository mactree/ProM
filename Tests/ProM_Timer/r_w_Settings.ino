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
    doseTime[DOSE3] = EEPROM.read(DOSE3);

    // menu
    menu = EEPROM.read(4);
  }
  else {
    // first time init
    doseTime[DOSE1] = 11;
    EEPROM.write(DOSE1, doseTime[DOSE1]);
    doseTime[DOSE2] = 22;
    EEPROM.write(DOSE2, doseTime[DOSE2]);
    doseTime[DOSE3] = 33;
    EEPROM.write(DOSE3, doseTime[DOSE3]);

    menu = DOSE1;
    EEPROM.write(4, menu);

    EEPROM.write(0, EEPROM_VALUE_VERSION);
  }
}  
