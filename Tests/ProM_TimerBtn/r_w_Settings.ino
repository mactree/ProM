void checkSettings() {
  int requestReset = digitalRead(SELECT_PIN);
  delay(2000);
  int stillRequestReset = digitalRead(SELECT_PIN);

  if (requestReset && stillRequestReset) {
    show(resetText);

    EEPROM.write(0, 0);

    while (digitalRead(SELECT_PIN)) // wait and proceed only when key is released
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
  }
  else {
    // first time init
    doseTime[DOSE1] = 50;
    EEPROM.write(DOSE1, doseTime[DOSE1]);
    doseTime[DOSE2] = 80;
    EEPROM.write(DOSE2, doseTime[DOSE2]);
    menu = DOSE1;
    EEPROM.write(0, EEPROM_VALUE_VERSION);
  }
}  
