void grinderStatus() {
  
 // request Time
 if (inData[3] == 160 && !inEditMode ){
    sendTime();
    switchColor = false;
    updateMenu = true;
    updateTime = true;
  }
  // ready
  else if(inData[3] == 164){
    unsigned long currentPauseMillis = millis();
    if (currentPauseMillis - previousPauseMillis >= 200) {
      previousPauseMillis = currentPauseMillis;
        switchColor = !switchColor;
        updateTime = true;
    }
    switchColor = false;
  }
  // grinding / running
  else if(inData[3] == 167){
    updateTime = true;    
    switchColor = false;
  }
  // finished grinding
  else if(inData[3] == 165){
    switchColor = false;
//    updateMenu = true;
  }
  // paused grinding
  else if(inData[3] == 228){
    unsigned long currentPauseMillis = millis();
    if (currentPauseMillis - previousPauseMillis >= 1000) {
      previousPauseMillis = currentPauseMillis;
        switchColor = !switchColor;
        updateMenu = true;
    }

  }

}
