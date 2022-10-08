void handleSetupMode() {
  // enter setup mode
  inSetupMode = !inSetupMode;
  doseCounter = EEPROM.read(4);
}
