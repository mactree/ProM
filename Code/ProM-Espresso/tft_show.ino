void tftShow(){
  if(!hopperOff){
    if(updateMenu){
    // reset variable
    updateMenu = false;

    // do not reset screen while switchingColor 
    if(!(inData[3]==228)){
      tft.fillScreen(TFT_BLACK);    
    }
    if(inSetupMode){
       
    }
    else{
        //
        //tft.drawFastVLine(82,0,128, TFT_WHITE);
        //tft.drawFastHLine(0,70,164, TFT_WHITE);
        if(switchColor){
           switch(menu){
            case 1:
              tft.drawBitmap( 58, 25, cup, cup_width,  cup_height, TFT_WHITE);
              break;
            case 2:
              tft.drawBitmap( 50, 18, cups, cups_width,  cups_height, TFT_WHITE);
              break;
            case 3:
              tft.drawBitmap( 30, 40, portalfilter, portalfilter_width,  portalfilter_height, TFT_WHITE);
              break;  
            }          
        }
        else{
           switch(menu){
            case 1:
              tft.drawBitmap( 58, 25, cup, cup_width,  cup_height, TFT_BLUE);
              break;
            case 2:
              tft.drawBitmap( 50, 18, cups, cups_width,  cups_height, TFT_BLUE);
              break;
            case 3:
              tft.drawBitmap( 30, 40, portalfilter, portalfilter_width,  portalfilter_height, TFT_BLUE);
              break;  
            }          
          
        }



    }
  }
    else if(updateTime){
    // reset variable
    updateTime = false;
    tft.setTextPadding(60);
    tft.setTextDatum(TC_DATUM);
    int poX = 80;
    int poY = 85;
    int font = 4;
    
    if(!inEditMode){
      tft.setTextColor( TFT_BLUE, TFT_BLACK);
      if(menu==3){
        tft.drawFloat((18000-t)/1000, 1, poX, poY, font);            
      }
      else{
        tft.drawFloat(t/1000, 1, poX, poY, font);  
        //tft.drawNumber(PausedTime, 90, 115, 1);          
      }
    }
    else{
      tft.setTextColor( TFT_WHITE, TFT_BLACK );
      volatile float dT = doseTime[menu];
      tft.drawFloat(dT /100, 1, poX, poY, font);
    }

      //Debug Info
      if(showDebug){
      tft.setTextDatum(TR_DATUM);
      tft.setTextPadding(20);
      
      tft.drawNumber(inData[0], 25, 3, 1);
      tft.drawNumber(inData[1], 25, 12, 1);
      tft.drawNumber(inData[2], 25, 21, 1);
      tft.drawNumber(inData[3], 25, 30, 1);
      tft.drawNumber(inData[4], 25, 39, 1);
      
      tft.drawNumber(inData[5], 160, 3, 1);
      tft.drawNumber(inData[6], 160, 12, 1);
      tft.drawNumber(inData[7], 160, 21, 1);
      tft.drawNumber(inData[8], 160, 30, 1);
      tft.drawNumber(inData[9], 160, 39, 1);
      }
      
  }
    else if(updateButtons){
    if(!inEditMode){
      tft.drawBitmap( 15, 90, cup_small, cup_small_width,  cup_small_height, TFT_WHITE);
        tft.drawBitmap( 130, 90, cups_small, cups_small_width,  cups_small_height, TFT_WHITE);
      }
    else{
        tft.drawBitmap( 13, 90, minus, minus_width,  minus_height, TFT_WHITE);
        tft.drawBitmap( 130, 90, plus, plus_width,  plus_height, TFT_WHITE);
      }
  }
  }
  else{
    if(resetScreen){
      resetScreen = false;
      tft.fillScreen(TFT_BLACK);
    }
    /*
    tft.drawFastVLine(82,0,128, TFT_WHITE);
    tft.drawFastHLine(0,70,164, TFT_WHITE);
    */
    tft.drawBitmap( 40, 10, hopper, hopper_width,  hopper_height, TFT_WHITE);
    tft.drawBitmap( 115, 10, symbol, symbol_width,  symbol_height, TFT_BLUE);
    // draw version string
    tft.setTextPadding(0);
    tft.setTextDatum(TL_DATUM);
    tft.setTextColor( TFT_BLUE, TFT_BLACK);
    tft.drawString("HOPPER", 30, 100, 4);
  }
  
}
