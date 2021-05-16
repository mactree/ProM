void readSettings() {
  
unsigned char ucHigh ;
unsigned char ucLow ;
  
  // version check
  bool readFromEEPROM = (EEPROM.read(0) == EEPROM_VALUE_VERSION);

  if (readFromEEPROM) {
    // dose time values
    doseTime[DOSE1] = (EEPROM.get ( 1, ucHigh) << 8) + EEPROM.get ( 2, ucLow);
    doseTime[DOSE2] = (EEPROM.get ( 3, ucHigh) << 8) + EEPROM.get ( 4, ucLow);
    doseTime[DOSE3] = (EEPROM.get ( 5, ucHigh) << 8) + EEPROM.get ( 6, ucLow);

    // menu
    menu = EEPROM.read(7);

    // settings
    settingSet[SET1] = EEPROM.read(8);
    settingSet[SET2] = EEPROM.read(9);
    settingSet[SET3] = EEPROM.read(10);
    settingSet[SET4] = EEPROM.read(11);


  }
  else {
    // first time init
    doseTime[DOSE1] = 500;
    EEPROM.put(1, highByte(doseTime[DOSE1]));
    EEPROM.put(2, lowByte(doseTime[DOSE1]));

    doseTime[DOSE2] = 1000;
    EEPROM.put(3, highByte(doseTime[DOSE2]));
    EEPROM.put(4, lowByte(doseTime[DOSE2]));

    doseTime[DOSE3] = 1800;
    EEPROM.put(5, highByte(doseTime[DOSE3]));
    EEPROM.put(6, lowByte(doseTime[DOSE3]));


    menu = DOSE1;
    EEPROM.write(7, menu);

    settingSet[SET1] = 3;
    EEPROM.put(8, settingSet[SET1]);

    settingSet[SET2] = 0;
    EEPROM.put(9, settingSet[SET2]);

    settingSet[SET3] = 0;
    EEPROM.put(10, settingSet[SET3]);

    EEPROM.write(0, EEPROM_VALUE_VERSION);
  }
}
