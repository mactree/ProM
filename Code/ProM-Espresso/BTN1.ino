void btn1(){
  if ( !inSetupMode && !inEditMode){
    // normal mode switch to manual gringing
    menu = DOSE3;
    // send time
    sendTime();
    // write menu to eeprom    
    EEPROM.write(7, menu);
  }
    
  updateMenu = true;
  
}
