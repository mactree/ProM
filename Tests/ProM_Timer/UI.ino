
void show(void (*content)()) {

  display.firstPage();
  do {
    content();
  } while (display.nextPage());
}



void splashScreen() {
  display.setFont(u8g_font_fub14r); // u8g_font_fub17r?
  display.drawStr((128 - display.getStrWidth(MACHINE_NAME)) / 2, 26, MACHINE_NAME);

  char buffer[4]; // note: do not change to static declaration! this will not work within the picture loop of u8glib
  buffer[0] = 'v';
  buffer[1] = '.';
  buffer[2] = toChar((EEPROM_VALUE_VERSION));
  buffer[3] = '\0';
  display.drawStr((128 - display.getStrWidth(buffer)) / 2, 60, buffer);
}

void resetText() {
  display.drawStr((128 - display.getStrWidth(F("RESET"))) / 2, 26, F("RESET"));

  //Serial.println("Reset");
}

char toChar(byte val) {
  return '0' + val;
}

void actualTime() {

  if (inSetupMode) {
    float ct = currentTime/10;
    display.setPrintPos(0, 60);
    display.print("SET=");
    display.setPrintPos(60, 60);
    display.print(currentTime );
    display.print("s");

  }
  else {
      display.setPrintPos(60, 60);
      display.print(ts,1);
      display.print("s");
  }
  


}

void actualMenu() {
  if (menu == ADD_GRINDING) {
    display.drawXBMP( 0, 0, hand1_width, hand1_height, hand1_bits);
  }
  else {
    display.drawXBMP( 0, 0, bean1_width, bean1_height, bean1_bits);
    if (menu == DOSE2) {
      display.drawXBMP( 40, 0, bean1_width, bean1_height, bean1_bits);
    }
  }

  actualTime();

}
