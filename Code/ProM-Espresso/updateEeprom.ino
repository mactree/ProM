void updateEeprom(){
  int adr = ((menu*2)-1);
  EEPROM.update(adr, highByte(doseTime[menu]));
  EEPROM.update(adr+1, lowByte(doseTime[menu]));
}
