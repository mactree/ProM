void handleEditMode() {
  // switch EditMode
  inEditMode = !inEditMode;

  if(inEditMode){
    grinderDisable();
  }
  else{
  // write time to eeprom
  updateEeprom();
  
  // send time
  sendTime();
//    grinderDisable();

  }
      

  // set variables
  updateMenu = true;
  updateTime = true;
}
