void grinderStatus() {
  //
 
  inData4_prev = inData4_now;  
  inData4_now = inData[4];

  // request Time
  if (inData[3] == 160 && !inEditMode ){
    sendTime();
    switchColor = false;
    updateMenu = true;
    updateTime = true;
  }
  // ready
  else if(inData[3] == 164){
    disableButtons = false;
    unsigned long currentPauseMillis = millis();
    if (currentPauseMillis - previousPauseMillis >= 200) {
      previousPauseMillis = currentPauseMillis;
        switchColor = false;
        updateTime = true;
    }
    switchColor = false;
  }
  // grinding / running
  else if(inData[3] == 167){
    updateTime = true;    
    switchColor = false;
    disableButtons = true;
  }
  // finished grinding
  else if(inData[3] == 165){
    switchColor = false;
//    updateMenu = true;
  }
  // paused grinding
  else if(inData[3] == 228){
    disableButtons = true;
    unsigned long currentPauseMillis = millis();
    if (currentPauseMillis - previousPauseMillis >= 1000) {
      previousPauseMillis = currentPauseMillis;
        switchColor = !switchColor;
        updateMenu = true;
    }

  }  
  // hopper off
  if(inData[4] == 8 ){
    hopperOff = true;
    updateMenu = true;
    disableButtons = true;
  }
  // hopper on
  else if(inData[4] == 0 ){
    hopperOff = false;
    //updateMenu = true;
  }
  // did the hopper state change?
  if ((inData4_prev + inData4_now) == 8){
    updateMenu = true;
    resetScreen = true;
  }
}
