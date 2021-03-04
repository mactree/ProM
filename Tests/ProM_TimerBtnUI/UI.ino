
void show(void (*content)()) {

  display.firstPage();
  do {
    content();
  } while (display.nextPage());
}



void splashScreen() {
    display.drawXBMP( 0, 0, logo_width, logo_height, logo);
}

void resetText() {
  display.drawStr((128 - display.getStrWidth(F("RESET"))) / 2, 26, F("RESET"));

  //Serial.println("Reset");
}

char toChar(byte val) {
  return '0' + val;
}

void actualTime() {
  //display.setFont(u8g_font_helvB18); //TODO Remove letters from lib!
  display.setFont(u8g_font_fub17);
  float tt = 0;
  if(grindRunning) {
    tt = grindTime;
  }
  else{
    tt = currentTime;
  }
  float ct = tt/10;
  display.setPrintPos(67, 28);
  //display.print(ts,1);  //test only
  display.print(ct,1);
  display.print("s");
}

void setupMenu() {  //Show Dose Counter! REVERT BACK and PAINT BOX OVER CUPS
      display.setFont(u8g_font_fub14);
      display.drawStr((128 - display.getStrWidth(F("Grind Count:"))) / 2, 26, F("Grind Count:"));
      display.setPrintPos(20,52);
      display.print(doseCounter);
}

void actualMenu() {
  if (menu == DOSE1) {
    display.drawXBMP( 17, 9, cup_width, cup_height, cup);
  }
  else if(menu == DOSE2){ 
    display.drawXBMP( 12, 4, cups_width, cups_height, cups);
  } 

  buttonUI();  
  actualTime();

}

void buttonUI(){ //Show button tags at bottom of display
  display.setFont(u8g_font_tpssb);
  display.drawDisc(9,56,7);
  display.drawDisc(117,56,7);
  display.drawHLine(0, 43,128);
  display.setPrintPos(48, 61);
  display.print("Shots"); 
  display.setColorIndex(0);
  display.setFont(u8g_font_fub17);
  display.setPrintPos(4, 63);
  display.print("-"); 
  display.setPrintPos(7, 62);
  display.print("-"); 
  display.setPrintPos(4, 62);
  display.print("-"); 
  display.setPrintPos(7, 63);
  display.print("-"); 
  display.setFont(u8g_font_fub14);
  display.setPrintPos(107,62);
  display.print("+");
  display.setPrintPos(107,63);
  display.print("+");
  display.setPrintPos(108,62);
  display.print("+");
  display.setPrintPos(108,63);
  display.print("+");
  display.setColorIndex(1);  
}

void doubleErr(){
  display.println("Press only one Button!");
}
