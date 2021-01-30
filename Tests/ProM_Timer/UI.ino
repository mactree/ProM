
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
    float tt = currentTime;
    float ct = tt/10;
    display.drawBox(0, 44, 128, 40);
    display.setColorIndex(0);
    //display.setPrintPos(0, 60);
    //display.print("SET=");
    display.setPrintPos(40, 60);
    display.print(ct,1 );
    display.print("s");
    display.setColorIndex(1);

  }
  else {
      display.setPrintPos(40, 60);
      display.print(ts,1);
      display.print("s");
  }
  


}

void actualMenu() {
  if (menu == DOSE1) {
    display.drawXBMP( 50, 4, cup_width, cup_height, cup);
  }
  else if(menu == DOSE2){ 
    display.drawXBMP( 50, 4, cups_width, cups_height, cups);
  }  
  else if (menu == DOSE3) {
      display.drawXBMP( 35, 15, portalfilter_width, portalfilter_height, portalfilter);
  }

  actualTime();

}
