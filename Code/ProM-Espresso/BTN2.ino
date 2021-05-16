void btn2(){
  if ( !inSetupMode && !inEditMode){
    // normal mode switch to manual gringing
    menu = DOSE1; 
    // send time
    sendTime(); 
    // write menu to eeprom    
    EEPROM.write(7, menu);

  }
  else if(inEditMode){
    doseTime[menu] = doseTime[menu] -10;
    updateTime = true;
  }
  updateMenu = true;
}
